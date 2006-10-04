# -*- ruby -*-
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Stechec project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the Stechec project; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  Copyright (C) 2005, 2006 Prologin
#

class RuleTest
  private

  #
  # check if 'prg' is in the path
  #
  def check_prog_path(prg)
    `which #{prg} > /dev/null 2>&1`
    if $?.exitstatus != 0
      puts prg + " no found in PATH. Aborting."
      abort
    end
  end

  #
  # check if 'prg' exit successfully, otherwise abort.
  #
  def check_prog_exec(prg, *args)
    `#{prg} #{args.join(' ')}`
    if $?.exitstatus != 0
      puts prg + " " + args.join(' ') + " failed with exit status " \
        + $?.exitstatus.to_s + ". Aborting."
      abort
    end
  end

  #
  # compile the required source
  # return the path to the champion_n.so
  #
  def compile(lang, src_file, index)
    if not @file_lang[lang]
      puts "don't know anything about language '" + lang + "'"
      abort
    end

    check_prog_exec('ruby',  ENV['RUBYLIB'] + '/generator.rb', 'player', ARGV[1], @tmpdir)
    check_prog_exec('cp', Pathname.new(ENV['srcdir']) + lang + src_file, 
                    @tmpdir + lang + @file_lang[lang])
    check_prog_exec("make", "-C", @tmpdir + lang, "clean all")
    check_prog_exec('mv', @tmpdir + lang + (@player_lib + ".so"),
                    @tmpdir + (@player_lib + "_#{index}.so"))
    File.expand_path(@tmpdir + (@player_lib + "_#{index}"))
  end

  #
  # check options in client_X, compile if necessary...
  #
  def prepare_client(conf)
    @nb_client = 0
    (1..99).each do |n|
      c = conf['client_' + n.to_s]
      break if not c
      @nb_client = n
      if c['source'] then
        source = c['source'].split('/')
        c['champion'] = compile source[0], source[1], n
      else
        c['champion'] = @player_lib
      end
    end
    abort if @nb_client == 0
  end

  #
  # generate the xml file, in the _hard_ way
  #
  def generate_xml(conf)
    f = File.new(@tmpdir + 'conf.xml', 
                 File::CREAT | File::TRUNC | File::WRONLY)

    conf['map'] = 'UNKNOWN' if conf['map'].nil?
    conf['max_turn'] = 10 if conf['max_turn'].nil?

    f.puts <<-EOF
<?xml version="1.0" ?>
<config>

  <game>
    <nb_team>#{@nb_client}</nb_team>
    <max_turn>#{conf['max_turn']}</max_turn>
    <map>#{conf['map']}</map>
  </game>

  <server>
    <rules>#{@server_rule_lib}</rules>
    <options persistent="false" start_game_timeout="30" />
    <listen port="25151" />
    <log enabled="false" file="match.log" />
    <nb_spectator>0</nb_spectator>
    <debug verbose="3" printloc="false" />
    <server_debug verbose="3" printloc="false" />
  </server>

  <client>
    <rules>#{@client_rule_lib}</rules>
    <connect val="network" host="localhost" port="25151"
             game_uid="42" connect_on_startup="true" />
  </client>

     EOF

     (1..@nb_client).each do |n|
       c = conf['client_' + n.to_s]
       stdin_redir = ''
       stdin_redir = '<stdin>' + c['input'] + '</stdin>' if c['input']
       f.puts <<-EOF
  <client_#{n}>
    <champion>#{c['champion']}</champion>
    <team>#{c['team']}</team>
    <mode replay="false" spectator="false" />
    <limit memory="10000" time="500" time_reserve="2500" />
    <debug valgrind="false" verbose="4" printloc="false" />
    #{stdin_redir}
  </client_#{n}>

      EOF
    end

    f.puts "</config>"
    f.close
  end
  
  #
  # test the specified section of test_pool.yml.
  #
  def test_stub(conf)
    
    # get and compile all clients from source, if needed.
    prepare_client conf

    # generate xml
    generate_xml conf

    # eventually run the test.
    ENV['LD_LIBRARY_PATH'] = ENV['LD_LIBRARY_PATH'] + ":" + File.expand_path(@tmpdir)
    lala=`bash #{ENV['BASHLIB']}/run.sh #{@tmpdir}/conf.xml`
    puts lala
    exit($?.exitstatus)
  end

  public

  def initialize
    # open contest yaml file
    case ARGV[1]
    when /\//
      conf = YAML::load(File.open(ARGV[1]))['conf']
    else
      path = Pathname.new(PKGDATADIR) + ARGV[1]
      conf = YAML::load(File.open(path + (ARGV[1] + ".yml")))['conf']
    end

    @client_rule_lib = conf['client_rule_lib']
    @server_rule_lib = conf['server_rule_lib']
    @player_lib = conf['player_lib']
    @player_filename = conf['player_filename']
    @file_lang = {
      'c' => @player_filename + '.c',
      'cxx' => @player_filename + '.cc',
      'caml' => @player_filename + '.ml',
      'java' => @player_filename.capitalize + '.java',
      'pascal' => @player_filename + '.pas',
      'haskell' => @player_filename + '.hs'
    }

    # do some sanity check
    check_prog_path('tbt')
    check_prog_path('tbt_server')

    # clean our temporary directory at exit
    # (or keep it if DEBUG is defined in environnment)
    at_exit do
      if ENV['DEBUG']
        puts @test_name + ": temporary directory left at " + @tmpdir
        File.open(@tmpdir + "debug_me.sh", "w") do |f|
          f.puts "#!/bin/sh"
          f.puts "export PATH=#{ENV['PATH']}"
          f.puts "export RUBYLIB=#{ENV['RUBYLIB']}"          
          f.puts "export LD_LIBRARY_PATH=#{ENV['LD_LIBRARY_PATH']}"
          f.puts "export xml_parser_path=#{ENV['BASHLIB']}/"
          f.puts "bash #{ENV['BASHLIB']}/run.sh conf.xml"
        end
        `chmod +x #{@tmpdir + "debug_me.sh"}`
      else
        system('rm', '-rf', @tmpdir)
      end
    end
  end


  def run
    pool = YAML::load(File.open(ARGV[0]))
    @test_name = File.basename(ARGV[2])

    # temporary directory
    @tmpdir = Pathname.new(@test_name + "-rules-test")
    @tmpdir.rmtree
    @tmpdir.mkdir

    test_stub pool[@test_name]
  end
end

if (ARGV.length != 3)
  puts "usage: ./rules_test.rb <yaml-test-pool> <yaml-contest-file> <test-name>"
  exit 1
end

require "pathname"
require "yaml"
require "conf"

RuleTest.new.run
