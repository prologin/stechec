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
  # print a log banner.
  #
  def print_banner(text = nil)
    puts "===================================================================="
    puts "== " + text if text
    puts "===================================================================="
    puts
    STDOUT.flush
  end

  #
  # print a log result.
  #
  def cmd_result(is_ok, text = nil)
    if text then
      puts "*** Result: " + text
    else
      puts "*** Result: OK." if is_ok
      puts "*** Result: FAILED." if not is_ok
    end
    puts
    abort if not is_ok
  end

  #
  # check if 'prg' is in the path
  #
  def check_prog_path(prg)
    `which #{prg} > /dev/null 2>&1`
    if $?.exitstatus != 0
      puts prg + " no found in PATH. Aborting."
      cmd_result false
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
      cmd_result false
    end
  end

  #
  # compile the required source
  # return the path to the champion_n.so
  #
  def compile(lang, src_file, index)
    print_banner 'Compile ' + src_file + ' in ' + lang
    if not @file_lang[lang]
      puts "don't know anything about language '" + lang + "'"
      cmd_result false
    end

    check_prog_exec('ruby',  ENV['RUBYLIB'] + '/generator.rb', 'player', ARGV[1], @tmpdir)
    check_prog_exec('cp', Pathname.new(ENV['srcdir']) + lang + src_file, 
                    @tmpdir + lang + @file_lang[lang])
    check_prog_exec("make", "-C", @tmpdir + lang, "clean all")
    check_prog_exec('mv', @tmpdir + lang + (@player_lib + ".so"),
                    @tmpdir + (@player_lib + "_#{index}.so"))
    cmd_result true
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
      if c['input'] then
        check_prog_exec('cp', ENV['srcdir'] + c['input'], @tmpdir.to_s)
        c['output'] = File.basename(c['input'], ".*") + ".out"
        c['input'] = File.basename(c['input'])
      end
      if c['team'] then
        check_prog_exec('cp', ENV['srcdir'] + c['team'], @tmpdir.to_s)
        c['team'] = File.basename(c['team'])
      end
      if c['formation'] then
        check_prog_exec('cp', ENV['srcdir'] + c['formation'], @tmpdir.to_s)
        c['formation'] = File.basename(c['formation'])
      end
      if c['diff'] then
        check_prog_exec('cp', ENV['srcdir'] + c['diff'], @tmpdir.to_s)
        c['diff'] = File.basename(c['diff'])
      end
    end
    if @nb_client == 0
      cmd_result false, "Can't test anything, no client!" 
    end
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
<?xml version="1.0" encoding="iso-8859-1" ?>
<!DOCTYPE config SYSTEM "file://#{PKGDATADIR}/config.dtd">

<config>

  <game>
    <nb_team>#{@nb_client}</nb_team>
    <max_turn>#{conf['max_turn']}</max_turn>
    <map>#{conf['map']}</map>
  </game>

  <client>
    <rules>#{@client_rule_lib}</rules>
    <connect val="network" host="localhost" port="25169"
             game_uid="42" connect_on_startup="true" />
  </client>

     EOF

     (1..@nb_client).each do |n|
       c = conf['client_' + n.to_s]
       redir = ''
       if c['input'] and c['output'] then
         redir = "<redirection stdin=\"#{c['input']}\" stdout=\"#{c['output']}\" />"
       end
       f.puts <<-EOF
  <client id="#{n}">
    <champion>#{c['champion']}</champion> 
    <team>#{c['team']}</team>
    <mode replay="false" spectator="false" />
    <limit memory="10000" time="500" time_reserve="2500" />
    <debug valgrind="false" verbose="3" printloc="false" />
    #{redir}
  </client>

      EOF
    end
 
    f.puts <<-EOF
  <server>
    <rules>#{@server_rule_lib}</rules>
    <options persistent="false" start_game_timeout="30" />
    <listen port="25169" />
    <log enabled="false" file="match.log" />
    <nb_spectator>0</nb_spectator>
    <debug verbose="4" printloc="false" />
    <server_debug verbose="1" printloc="false" />
  </server>
</config>
    EOF

    f.close
  end
  
  #
  # Do a diff
  #
  def do_diff(c)
    print_banner "Doing diff between '#{c['diff']}' and '#{c['output']}'"
    tmp_file = c['output'].to_s + '.tmp'

    `sed -n '/^BEGIN DIFF/ { :n; n; /^END DIFF/d; p; b n } ' < #{c['output']} > #{tmp_file}`
    `diff -u #{c['diff']} #{tmp_file} > #{c['input'] + '.diff'}`
    res = $?.exitstatus

    if res != 0 then
      File.open(c['input'] + '.diff') do |f|
        while s = f.gets do
          print s
        end
      end
      puts
    end
    cmd_result(res == 0)
    res
  end

  #
  # Do some post-processing stuff
  #
  def post_run(conf)
    res = 0
    (1..@nb_client).each do |n|
       c = conf['client_' + n.to_s]
      if c['output'] and c['diff'] then
        res += do_diff c
      end
    end
    return res
  end

  #
  # Do some pre-processing stuff, inside temporary directory
  #
  def pre_run(conf)
    
    (1..@nb_client).each do |n|
      c = conf['client_' + n.to_s]
      if c['team']
        check_prog_exec('sed', '-i', '-e', "s,@pkgdatadir@,#{PKGDATADIR},", c['team'])
      end
      if c['formation']
        check_prog_exec('sed', '-i', '-e', "s,@pkgdatadir@,#{PKGDATADIR},", c['formation'])
      end
    end
  end

  # clean our temporary directory at exit
  # (or keep it if DEBUG is defined in environnment)
  def clean
      if ENV['DEBUG']
        puts @test_name + ": temporary directory left at " + @tmpdir
        if @oldpwd
          File.open("debug_me.sh", "w") do |f|
            f.puts "#!/bin/sh"
            f.puts "export PATH=#{ENV['PATH']}"
            f.puts "export RUBYLIB=#{ENV['RUBYLIB']}"          
            f.puts "export LD_LIBRARY_PATH=#{ENV['LD_LIBRARY_PATH']}"
            f.puts "export xml_parser_path=#{ENV['BASHLIB']}/"
            f.puts "bash #{ENV['BASHLIB']}/run.sh conf.xml"
          end
        `chmod +x #{"debug_me.sh"}`
        end
      else
        Dir.chdir @oldpwd if @oldpwd
        puts "cleaning ..." + @tmpdir.to_s
        @tmpdir.rmtree if @tmpdir.directory?
      end
  end

  #
  # test the specified section of test_pool.yml.
  #
  def test_stub(conf)
    
    # get and compile all clients from source, if needed.
    prepare_client conf

    # generate xml
    generate_xml conf

    # go in the temporary directory
    @oldpwd = Dir.pwd
    Dir.chdir(@tmpdir.to_s)
    pre_run conf

    # run the test.
    print_banner 'Run the test'
    ENV['LD_LIBRARY_PATH'] = ENV['LD_LIBRARY_PATH'] + ":" + File.expand_path(@tmpdir)
    `bash #{ENV['BASHLIB']}/run.sh conf.xml`
    res = $?.exitstatus
    cmd_result(res == 0)

    # run things after run
    res = post_run conf

    exit(res)
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

  end


  def run
    pool = YAML::load(File.open(ARGV[0]))
    @test_name = File.basename(ARGV[2])

    # create temporary directory
    @tmpdir = Pathname.new(@test_name + "-rules-test")
    @tmpdir.rmtree if @tmpdir.directory?
    @tmpdir.mkdir

    at_exit { clean }
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
