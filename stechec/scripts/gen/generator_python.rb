#
# Stechec project is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `NOTICE' file in the root directory.
#
# Copyright (C) 2006 Prologin
#

# C++ generator, for python-interface
class PythonCxxFileGenerator < CxxProto
  def initialize
    super
    @lang = "C++ (for python interface)"
  end

  def generate_header()
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_python.rb"

    @f.puts <<-EOF
#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <Python.h>

class PythonInterface
{
public:
  PythonInterface();
  ~PythonInterface();
  void callPythonFunction(const char* name);

private:
  PyObject *pModule;
};

    EOF

    build_constants
    @f.puts "", 'extern "C" {', ""
    for_each_fun { |x, y, z| print_proto(x, y, z, "extern"); @f.puts ";" }
    for_each_user_fun { |x, y, z| print_proto(x, y, z); @f.puts ";" }
    @f.puts "}", "", "#endif // !INTERFACE_HH_"
    @f.close
  end

  def generate_source()
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_python.rb"
    
    @f.puts <<-EOF
#include "interface.hh"

PythonInterface gl_python;

/*
** Api functions called from Python to stechec.
*/
extern "C" {
    EOF
      
    for_each_fun(false) do |name, ret, args|
      var_args = []
      str_fmt = ""
      args.each_index do |i|
        n = (i + 1).to_s
        str_fmt = str_fmt + "i"
        var_args << "arg" + n
      end if args
      @f.print "static PyObject* p_", name, "(PyObject*, PyObject* args)\n"
      @f.puts "{"
      @f.print "  int ", var_args.join(", "), ";\n" if args
      @f.print "  if (!PyArg_ParseTuple(args, \"#{str_fmt}\""
      @f.print ", &" if args
      @f.print var_args.join(", &"), "))\n"
      @f.puts "{"
      @f.puts "    PyErr_SetString(PyExc_TypeError, \"Invalid parameters.\");"
      @f.puts "    return NULL;"
      @f.puts "}"
      @f.print "  int ret = ", name, "(", var_args.join(", "), ");\n"
      @f.puts "  return PyInt_FromLong(ret);", "}"
    end

    @f.puts <<-EOF
} // !api functions

/*
** Api functions to register.
*/
static PyMethodDef api_callback[] = {
    EOF

    for_each_fun(false) do |name, ret, arg|
      @f.print '  {"', name, '", p_', name, ', METH_VARARGS, "', name, '"},'
    end

    @f.puts <<-EOF
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initapi()
{
  Py_InitModule("api", api_callback);
}

/*
** Inititialize python, register API functions,
** and load .py file
*/
PythonInterface::PythonInterface()
{
  PyObject* pName;
  char* pChampionPath;
      
  pChampionPath = getenv("CHAMPION_PATH");
  if (pChampionPath == NULL)
    pChampionPath = ".";

  setenv("PYTHONPATH", pChampionPath, 1);

  Py_SetProgramName("stechec");
  Py_Initialize();
  initapi();

  pName = PyString_FromString("prologin");
  pModule = PyImport_Import(pName);
  Py_DECREF(pName);
  if (pModule == NULL)
    if (PyErr_Occurred())
    {
      PyErr_Print();
      abort();
    }
}

PythonInterface::~PythonInterface()
{
  Py_Finalize();
}

/*
** Run a python function.
*/
void PythonInterface::callPythonFunction(const char* name)
{
  PyObject *arglist, *pFunc;
  PyObject *result = NULL;
  
  pFunc = PyObject_GetAttrString(pModule, (char*)name);
  if (pFunc && PyCallable_Check(pFunc))
    {
      arglist = Py_BuildValue("()");
      result = PyEval_CallObject(pFunc, arglist);
      Py_XDECREF(arglist);
    }
  if (result == NULL && PyErr_Occurred())
    PyErr_Print();
  Py_XDECREF(result);
}

/*
** Functions called from stechec to C.
*/
    EOF

    for_each_user_fun(false) do |name, type_ret, args|
      print_proto(name, type_ret, args, 'extern "C"')
      print_body "  gl_python.callPythonFunction(\"" + name + "\");"
     end

    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "python"
    @header_file = 'interface.hh'
    @source_file = 'interface.cc'

    generate_header
    generate_source
  end

end

class PythonFileGenerator < FileGenerator
  def initialize
    @lang = "python"
  end

  def print_comment(str)
    @f.puts '# ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    str.each {|s| @f.print '# ', s }
    @f.puts ""
  end

  def generate_makefile
    @f = File.open(@path + "Makefile", 'w')
    @f.print <<-EOF
# -*- Makefile -*-
#

SRC       = #{@source_file} # Ajoutez ici vos fichiers .py
NAME      = #{$conf['conf']['player_lib']}.so

MY_CXXFLAGS = -ggdb3

include ../includes/makepython
    EOF
    @f.close
  end


  def build()
    @path = Pathname.new($install_path) + "python"
    @source_file = $conf['conf']['player_filename'] + '.py'

    ######################################
    ##  interface.hh file generating    ##
    ######################################
    PythonCxxFileGenerator.new.build

    ######################################
    ##  prologin.py file generating     ##
    ######################################
    @f = File.new(@path + @source_file, 'w')
    print_banner "generator_python.rb"
    @f.puts "import api", ""
    for_each_user_fun do |name, ret, args|
      @f.puts "def " + name + "():"
      @f.puts "\tpass # Place ton code ici"
    end
    @f.close

    generate_makefile
  end

end
