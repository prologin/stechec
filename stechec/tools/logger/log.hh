/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2006 The TBT Team.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#ifndef LOG_HH_
# define LOG_HH_

/*! @file log.hh
**
** This module could be used to log debug and error message into a
** c++ iostream (usually @p std::cerr). This is still experimental, and
** could be changed in the futur. It is designed to be thread-safe, and could
** log each thread in a separate ostream.
**
** Currently, it could log only into @p std::cerr.
**
** To use it, define the following macro to your compilator:
** @code
** -DMODULE_NAME=\"server\"  // Name of your module (eg: server).
** -DMODULE_COLOR=C_GREEN    // Color to use.
** @endcode
**
**
** Then, somewhere in your code, keep a Log object per thread. If you
** don't do this, it will happily segfault. (hint: put it at the beginning
** of your main() and thread starting functions).
** @code
** Log log(3);         // Verbose level to 3.
** log.setPrintLoc();
** @endcode
**
** Now, you are ready to use it ! Only thinks that LOG1 or other macros
** will be replaced by 'std::cout <<'
** @code
** int foo() {
**   LOG1("hello " << 42 << " world!");
**   while (true) ;
**   ERR("Your computer stinks.");
** }
** @endcode
**
** The module name will be printed into bracket before each line. Beware, the
** the printed module name will reflect which compilation unit possess it, and
** may not reflect the source directory module. This is only true for LOG
** in headers.
*/

/*! @brief Application logger
**
** @author victor
** @date 21/01/2006
*/
class Log
{
public:
  Log(int verbose_level = 1, const char* suffix = NULL);
  ~Log();

  //! @brief Enable the specified level of verbosity.
  //! @param level Activate all levels from 0 up to level (0-31).
  void setVerboseLevel(int level);
  //! @brief Directly change the verbosity mask.
  //! @param mask The mask to apply (eg: 0x06 activate level 1
  //! and 2, but not level 0).
  //! @note This function is mostly useless, only using \c setVerboseLevel is fine.
  void setVerboseMask(int mask);
  //! @brief Set a string to display right after module name, into the braces.
  //! @param name Set it to NULL if you don't want a suffix.
  void setModuleSuffix(const char* name);
  //! @brief Print line number and file name to easily find the LOG macro.
  void setPrintLoc(bool enable = true);
  //! @brief Enable or disable color (by default use them).
  void setUseColor(bool enable = true);

  //! @brief Used internally.
  static Log* getInst();
  //! @brief Used internally.
  std::ostream& getStream();

  // Don't use them directly !
  static pthread_mutex_t lock_;
  int verbose_mask_;
  bool print_loc_;
  bool use_color_;
  char modsuffix_[64];

private:
  pthread_t pth_id_;
  char orig_modsuffix_[32];

  typedef std::map<pthread_t, Log*> ThreadList;
  static ThreadList thread_list_;
  static int ref_count_;
};

inline Log* Log::getInst()
{
  // If this assert breaks, you probably not have a Log object instancied.
  ThreadList::iterator it = thread_list_.find(pthread_self());
  assert(it != thread_list_.end());
  return it->second;
}

inline std::ostream& Log::getStream()
{
  return std::cerr;
}

// Set default MODULE_NAME and MODULE_COLOR, in case they are not provided.
# ifndef MODULE_NAME
#  define MODULE_NAME "unset"
# endif
# ifdef MODULE_COLOR
#  define C_NONE        "\e[0m"
#  define C_RED         "\e[1;31m"
#  define C_GREEN       "\e[1;32m"
#  define C_BROWN       "\e[1;33m"
#  define C_BLUE        "\e[1;34m"
#  define C_PURPLE      "\e[1;35m"
#  define C_CYAN        "\e[1;36m"
#  define C_GRAY        "\e[0;30m"
#  define C_BRED        "\e[0;31m"
#  define C_BGREEN      "\e[0;32m"
#  define C_YELLOW      "\e[0;33m"
#  define C_BBLUE       "\e[0;34m"
#  define C_BPURPLE     "\e[0;35m"
#  define C_BCYAN       "\e[0;36m"
# else
#  define MODULE_COLOR  ""
#  define C_NONE        ""
#  define C_RED         ""
#  define C_GREEN       ""
#  define C_BROWN       ""
#  define C_BLUE        ""
#  define C_PURPLE      ""
#  define C_CYAN        ""
#  define C_GRAY        ""
#  define C_BRED        ""
#  define C_BGREEN      ""
#  define C_YELLOW      ""
#  define C_BBLUE       ""
#  define C_BPURPLE     ""
#  define C_BCYAN       ""
# endif

// Do not use it. Prefer the ERR, WARN, and LOG? macros
# define LOG(Level, Msg)                                                        \
{                                                                               \
  Log* l__ = Log::getInst();                                                    \
  if (l__->verbose_mask_ & (1 << Level)) {                                      \
    pthread_mutex_lock(&Log::lock_);                                            \
    std::ostringstream os__;                                                    \
    if (Level == 0)                                                             \
      if (l__->use_color_)                                                      \
        os__ << C_BRED "Error: " C_NONE;                                        \
      else                                                                      \
        os__ << "Error: ";                                                      \
    else if (Level == 1)                                                        \
      if (l__->use_color_)                                                      \
        os__ << C_YELLOW "Warning: " C_NONE;                                    \
      else                                                                      \
        os__ << "Warning: ";                                                    \
    os__ << Msg;                                                                \
    if (l__->print_loc_)                                                        \
      if (l__->use_color_)                                                      \
        l__->getStream() << "[" MODULE_COLOR MODULE_NAME << l__->modsuffix_     \
                       << C_NONE "] " << __FILE__ << ":" << __LINE__ << ": "    \
                       << os__.str() << std::endl;                              \
      else                                                                      \
        l__->getStream() << "[" MODULE_NAME << l__->modsuffix_                  \
                       << "] " << __FILE__ << ":" << __LINE__ << ": "           \
                       << os__.str() << std::endl;                              \
    else                                                                        \
      if (l__->use_color_)                                                      \
        l__->getStream() << "[" MODULE_COLOR MODULE_NAME << l__->modsuffix_     \
                       << C_NONE "] " << os__.str() << std::endl;               \
      else                                                                      \
        l__->getStream() << "[" MODULE_NAME << l__->modsuffix_                  \
                       << "] " << os__.str() << std::endl;                      \
    pthread_mutex_unlock(&Log::lock_);                                          \
  }                                                                             \
}

# define ERR(Msg) LOG(0, Msg)
# define WARN(Msg) LOG(1, Msg)

# define LOG1(Msg) LOG(2, Msg)
# define LOG2(Msg) LOG(3, Msg)
# define LOG3(Msg) LOG(4, Msg)
# define LOG4(Msg) LOG(5, Msg)
# define LOG5(Msg) LOG(6, Msg)
# define LOG6(Msg) LOG(7, Msg)


#endif /* !LOG_H_ */
