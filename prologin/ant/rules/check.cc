/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

class ClientCx {
 public:
  bool process(bool = true);
};
bool ClientCx::process(bool) { return true; }
int main() {
  ClientCx cx;
  cx.process();
  return 0;
}
