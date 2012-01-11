class JavaFileGenerator < JavaProto
  def initialize
    super
    @lang = "java"
    @java_types = {
      'void' => 'void',
      'int' => 'int',
      'bool' => 'boolean'
    }
    @java_obj_types = {
      'int' => 'Integer',
      'bool' => 'Boolean'
    }
  end

  def name_to_type(str)
    $conf['enum'].each do |x|
       if str == x['enum_name']
         return x
       end
    end
    return nil #error
  end

  def build
    @path = Pathname.new($install_path) + "java"
    @java_interface = 'Interface'
    @java_file = $conf['conf']['player_filename'].capitalize

    # TODO: JavaCxxFileGenerator.new.build

    ######################################
    ##  Interface.java file generating  ##
    ######################################
    @f = File.new(@path + (@java_interface + '.java'), 'w')
    print_banner "generator_java.rb"

    for_each_enum do |x|
      name = conv_java_type(x['enum_name'])
      @f.puts "enum #{name} {"
      x['enum_field'].each do |f|
        @f.puts "  #{f[0].upcase}, // <- #{f[1]}"
      end
      @f.puts "}"
    end

    for_each_struct do |x|
      name = conv_java_type(x['str_name'])
      @f.puts "class #{name} {"
      x["str_field"].each do |f|
        @f.print "  public #{conv_java_type(f[1])} #{f[0]}"
        if @types[f[1]].is_struct? then
          # Initialize structures so that the candidate can use the object
          # more easily
          @f.print " = new #{conv_java_type(f[1])}()";
        end
        @f.puts "; // #{f[2]}"
      end
      @f.puts "}"
    end

    @f.puts "public class #{@java_interface}", "{"
    build_constants '  '
    for_each_fun(true, 'function', '  ') do |f|
      print_proto("  public static native", f)
      @f.puts ";"
    end
    @f.puts "}"
    @f.close

    #####################################
    ##  Prologin.java file generating  ##
    #####################################
    @f = File.new(@path + (@java_file + '.java'), 'w')
    print_banner "generator_java.rb"
    # generate functions bodies
    @f.puts "public class #{@java_file} extends #{@java_interface}", "{"
    for_each_user_fun(true, '  ') do |f|
      @f.print "  "
      print_proto("public", f)
      @f.puts "",
      "  {",
      "    // Place ton code ici",
      "  }",
      ""
    end
    @f.puts "}"
    @f.close

    #
    # Makefile generation
    #
    target = $conf['conf']['player_lib']
    @f = File.new(@path + "Makefile", 'w')
    @f.print <<-EOF
# -*- Makefile -*-

lib_TARGETS = #{target}

#{target}-srcs = Interface.java Prologin.java
#{target}-cxxflags = -I. -ggdb3

# Evite de toucher a ce qui suit
EOF
    @f.print "#{target}-jclassopt ="
    $conf['struct'].each do |x|
        @f.print " ", x['str_name'].capitalize, ".class"
    end
    $conf['enum'].each do |x|
       @f.print " ", x['enum_name'].capitalize, ".class"
    end
    @f.puts
    @f.print <<-EOF
#{target}-dists = interface.hh
#{target}-srcs += interface.cc ../includes/main.cc
include ../includes/rules.mk
    EOF
    @f.close
  end

end
