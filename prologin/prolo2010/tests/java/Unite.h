
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __Unite__
#define __Unite__

#pragma interface

#include <java/lang/Object.h>
extern "Java"
{
    class Position;
    class Type_unite;
    class Unite;
}

class Unite : public ::java::lang::Object
{

public: // actually package-private
  Unite();
public:
  ::Position * __attribute__((aligned(__alignof__( ::java::lang::Object)))) pos;
  jboolean ennemi;
  ::Type_unite * type_unite_actuel;
  ::Type_unite * vrai_type_unite;
  jint ko;
  jint pa;
  jint attaques;
  jint attaques_gratuites;
  jint id;
  static ::java::lang::Class class$;
};

#endif // __Unite__
