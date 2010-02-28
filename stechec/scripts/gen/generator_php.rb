#
# Stechec project is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `NOTICE' file in the root directory.
#
# Copyright (C) 2010 Prologin
#

# C++ generator, for PHP interface
class PhpCxxFileGenerator < CxxProto
  def initialize
    super
    @lang = "C++ (for PHP interface)"
  end

  def build
    # TODO
  end
end

class PhpFileGenerator < FileGenerator
  def initialize
    super
    @lang = "php"
  end

  def build
    # TODO
  end
end
