// This header file is only for Doxygen documentation !

/*! \mainpage Tow Bowl Tactics - Doxygen Code
**
** \section intro_sec Introduction
**
** First, please take a look at the document \c propal.pdf, it might
** give you a good oversight of the program, while this documentation
** will go more inside.
**
** If you have some time and a average english level (better than
** mine, it's not difficult), please feel free to fix some basic
** mistakes.
**
** \section UI Interface
**
** For those who wants to implement an user interface (which could be
** a 2d gui, 3d gui, ai, ...), you should look closely to the
** GameClient class. It will provide you all game informations you may
** need, and enough methods to do any actions the game require (move
** player, end a turn, ...).
**
** \section toolbox_sec Toolbox
**
** There are some tools already implemented, that you must
** be aware of.
**
** \subsection log_sec Logger
**
** This is a facility to log debug message, with customizable
** verbosity level.
** See file log.hh for further details.
**
** \subsection xml_sec Xml parser
**
** Our XML parser is built on top of xerces, and abstract most of the
** boring work. You should have a look at the class Xml.
**
** \subsection net_sec Network tools
**
** There is already all that is needed for networking.
** See class Cx, TcpCx, DirectCx, and Packet.
**
** \subsection misc_sec Miscellaneous
**
** There is Position, a tiny point class, that should be useful
** everywhere...
**
** Happy coding!
*/
