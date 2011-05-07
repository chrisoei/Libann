#ifndef LOG_H
#define LOG_H
/*

    Copyright (C) 2002  John Darrington 

    $Id: log.h,v 1.7 2003/05/14 05:50:16 john Exp $

    This program is free software; you can redistibute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <iostream>
#include <string>

/// Call this macro as the VERY FIRST statement in every function
#define trace ann::Trace t(__FILE__ , __LINE__ , __FUNCTION__)


namespace ann { 

  void setLogStream(std::ostream &str);

  void setLogFile(const std::string  &filename);

  std::ostream & log();


  /*! A class to assist tracking of the function call stack.
      It should not be used directly.  Instead use the trace macro.
  */
  class Trace { 
  public:
    Trace(const std::string &text, int line, const std::string &func="");
    ~Trace();
  private:
    std::string text;
    static int stackDepth;
  };

}

#endif /* LOG_H */
