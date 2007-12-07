/* Defines String::compose(fmt, arg...) for easy, i18n-friendly
 * composition of strings.
 *
 * Version 1.0.
 *
 * Copyright (c) 2002 Ole Laursen <olau@hardworking.dk>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 */

//
// Basic usage is like
//
//   std::cout << String::compose("This is a %1x%2 matrix.", rows, cols);
//
// See http://www.cs.aau.dk/~olau/compose/ or the included README.compose for
// more details.
//

//
// The original implementation was too slow for our need, and
// as I supected some segv in this code, I rewrote most of the code.
// Basically, it does not have the same feature as before, but it is a lot
// faster...
// Victor, 2007.
//

#ifndef STRING_COMPOSE_H
# define STRING_COMPOSE_H

# include <iostream>
# include <sstream>
# include <string>
# include <deque>

namespace StringPrivate
{
  // Hide the mess in a obscur namespace... Yes, really.

  class Composition
  {
  public:
    // initialize and prepare format string on the form "text %1 text %2 etc."
    explicit Composition(std::string fmt);
    ~Composition();

    // supply an replacement argument starting from %1
    template <typename T>
    Composition &arg(const T &obj);

  private:
    std::ostringstream os;
    typedef std::deque<std::pair<int, std::string> > SpecList;
    SpecList spec;
  };

  inline Composition::Composition(std::string fmt)
  {
    std::string::size_type fmt_size = fmt.length();
    std::string::size_type last = 0;
    std::string::size_type i;
    bool next_m = false;
    std::string current;
    int spec_no;
    int spec_size;
  
    for (i = 0; i < fmt_size; i++)
      {
	if (fmt[i] == '%')
	  {
	    if (next_m)
	      {
		current += fmt.substr(last, i - last - 1);
		last = i;
		next_m = false;
	      }
	    else
	      next_m = true;
	    continue;
	  }

	if (next_m && fmt[i] >= '0' && fmt[i] <= '9')
	  {
	    spec_no = fmt[i] - '0';
	    spec_size = 1;
	    if (i + 1 < fmt_size && fmt[i + 1] >= '0' && fmt[i + 1] <= '9') {
	      spec_no = spec_no * 10 + (fmt[i + 1] - '0');
	      spec_size = 2;
	      i += 1;
	    }

	    spec.push_back(std::make_pair(spec_no, current + fmt.substr(last, i - last - spec_size)));
	    last = i + 1;
	    next_m = false;
	    current = "";
	  }
      }
    spec.push_back(std::make_pair(spec_no, current + fmt.substr(last, i - last)));
  }

  // Print remainings data in format string
  inline Composition::~Composition()
  {
    for (SpecList::iterator it = spec.begin(); it != spec.end(); ++it)
      {
	std::cerr << it->second;
      }
    std::cerr << std::endl;
  }

  // implementation of class Composition
  template <typename T>
  inline Composition &Composition::arg(const T& obj)
  {
    std::cerr << spec.front().second;
    spec.pop_front();
    std::cerr << obj;
    os.str("");
    
    return *this;
  }
}





inline void tlog(const std::string &fmt)
{
  std::cerr << fmt << std::endl;
}

// a series of functions which accept a format string on the form "text %1
// more %2 less %3" and a number of templated parameters and spits out the
// composited string
template <typename T1>
inline void tlog(const std::string &fmt, const T1 &o1)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1);
}

template <typename T1, typename T2>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2);
}

template <typename T1, typename T2, typename T3>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7, const T8 &o8)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7).arg(o8);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7, const T8 &o8, const T9 &o9)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7).arg(o8).arg(o9);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7, const T8 &o8, const T9 &o9,
			const T10 &o10)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7).arg(o8).arg(o9)
    .arg(o10);
}
  
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10,
	  typename T11>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7, const T8 &o8, const T9 &o9,
			const T10 &o10, const T11 &o11)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7).arg(o8).arg(o9)
    .arg(o10).arg(o11);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10,
	  typename T11, typename T12>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7, const T8 &o8, const T9 &o9,
			const T10 &o10, const T11 &o11, const T12 &o12)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7).arg(o8).arg(o9)
    .arg(o10).arg(o11).arg(o12);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10,
	  typename T11, typename T12, typename T13>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7, const T8 &o8, const T9 &o9,
			const T10 &o10, const T11 &o11, const T12 &o12,
			const T13 &o13)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7).arg(o8).arg(o9)
    .arg(o10).arg(o11).arg(o12).arg(o13);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10,
	  typename T11, typename T12, typename T13, typename T14>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7, const T8 &o8, const T9 &o9,
			const T10 &o10, const T11 &o11, const T12 &o12,
			const T13 &o13, const T14 &o14)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7).arg(o8).arg(o9)
    .arg(o10).arg(o11).arg(o12).arg(o13).arg(o14);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10,
	  typename T11, typename T12, typename T13, typename T14,
	  typename T15>
inline void tlog(const std::string &fmt,
			const T1 &o1, const T2 &o2, const T3 &o3,
			const T4 &o4, const T5 &o5, const T6 &o6,
			const T7 &o7, const T8 &o8, const T9 &o9,
			const T10 &o10, const T11 &o11, const T12 &o12,
			const T13 &o13, const T14 &o14, const T15 &o15)
{
  StringPrivate::Composition c(fmt);
  c.arg(o1).arg(o2).arg(o3).arg(o4).arg(o5).arg(o6).arg(o7).arg(o8).arg(o9)
    .arg(o10).arg(o11).arg(o12).arg(o13).arg(o14).arg(o15);
}


#endif // STRING_COMPOSE_H
