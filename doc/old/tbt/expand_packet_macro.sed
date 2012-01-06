#! /bin/sed -f

# Ugly... Used to expand ugly macros instead of doxygen preprocessor.
# Keep it synchronized with macro definitions in the following files:
#   stechec/base_rules/PacketHandler.cc
#   stechec/tools/tools.hh

/^DECLARE_EMPTY_PACKET/{
  s/^DECLARE_EMPTY_PACKET/DECLARE_PACKET/
  aEND_PACKET;
}
s/^DECLARE_PACKET( *\([^ ,]*\) *, *\([^ )]*\) *)$/\nstruct \2;\ntemplate <typename T>\nclass PacketHandler<\1, T> : public BasePacketHandler\n{\npublic:\n  typedef void (T::*fct_t)(const \2*);\n  PacketHandler<\1, T>(T* obj, fct_t f)\n    : obj_(obj), f_(f) {}\n  virtual void handle(const Packet* p)\n  {\n    LOG5("PacketHandler gets message `"\n         #\1 "' (client_id: %1)", p->client_id);\n    (obj_->*f_)(reinterpret_cast<const \2*>(p));\n  }\n  virtual int get\1Value() const { return \1; }\n  virtual void dispatchUIEvent(const EventProcess* evp,\n                               const Packet\& pkt)\n  {\n    evp->dispatch(reinterpret_cast<const \2\&>(pkt));\n  }\nprivate:\n  T* obj_;\n  fct_t f_;\n};\nstruct \2 : public Packet\n{\n  \2(int client_id = -1)\n   : Packet(\1, client_id) { data_size = sizeof(*this); }/
s/END_PACKET/};/
s/^BEGIN_NS( *\([^ )]*\) *)/namespace \1 {/
s/^END_NS( *\([^ )]*\) *)/}/
