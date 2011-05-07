/*

    Copyright (C) 2002  John Darrington 

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
const static char RCSID[]="$Id: regexpr.cc,v 1.4 2003/05/14 05:50:16 john Exp $";

#include "regexpr.h"
#include <stdexcept>


Regexpr::Regexpr(const std::string &regex,int cflags) {
  int ret = regcomp(&preg, regex.c_str(),0);
  if (ret != 0 ) { 
    throw std::runtime_error("Cannot create regular expression");
  }
};


bool
Regexpr::exec(const std::string &x) const 
{
  int ret = regexec(&preg,x.c_str(),0,0,0 );

  return ( ret == 0 );
}

Regexpr::~Regexpr() { 
  regfree(&preg);
}

