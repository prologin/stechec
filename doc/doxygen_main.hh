// This header file is only for Doxygen documentation !

/*!
** @mainpage Stechec/Tow Bowl Tactics - Source Documentation
**
** @section intro_sec Introduction
**
** First, please take a look at the document
** <a href="https://projects.nekeme.net/svn/tbt/trunk/doc/propal.pdf">Proposition
** d'architecture</a> (still in french), it might give you a good oversight
** of the program, while this documentation will go more inside.
**
** If you have some time and an average english level (better than
** mine, it's not difficult), please feel free to fix some basic
** mistakes.
**
** @section layout_sec Source Layout
**
** Tow Bowl Tactics is based on %Stechec which provides many handy tools and
** modules, such as logger, %XML parser, data transfert, SDL wrapper, etc.
** %Stechec is in fact pre-existent and not specific to TBT. It is now
** maintained and developed here in parallel to TBT. That's why the source
** code is splitted in @c stechec and @c bb5 directories.
** 
** Tow Bowl Tactics has been designed to be as modular as possible,
** therefore you will probably find easier to browse this documentation
** by <a href="modules.html">Modules</a> (mostly based on directories layout).
**
** @section notes_sec Notes
**
** @subsection client_sec Implementing an user interface
**
** For those who wants to implement an user interface to TBT (which could
** be a text-based UI, 2D GUI, 3D GUI, AI, ...), you should look closely
** to the Api class. It will provide you all game informations you may
** need, and enough methods to do any actions the game require (move
** player, end a turn, ...).
**
** @subsection toolbox_sec Stechec Toolbox
**
** There are some tools already implemented, that you must
** be aware of, see @ref tools module.
**
** Happy coding!
*/
