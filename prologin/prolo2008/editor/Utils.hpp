// Utils.hpp -- The Prologin 2008 Map Editor

#include <string>
#include <stdarg.h>

std::string	Error(const char *s, ...)
{
  va_list	ap;
  std::string	err(s);

  va_start(ap, s);
  while ((s = va_arg(ap, const char *)))
    err += s;
  va_end(ap);
  return (err);
}

