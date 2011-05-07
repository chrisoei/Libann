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
const static char RCSID[]="$Id: ann.cc,v 1.7 2003/05/14 05:50:16 john Exp $";

#include <assert.h>

#include "ann.h"
#include "vers.h"


namespace ann {

std::string
version()
{
  return theVersion;
}



ExtInput &
ExtInput::operator=(const ann::vector &v) 
{
  assert ( v.size() == size() );

  for (int i = 0 ; i < size() ; ++i ) { 
    (*this)[i]=v[i];
  }

  return *this;
}




bool 
operator==(const ExtInput &e, const ann::vector &v)
{

  assert(e.size() == v.size());

  for (int i = 0 ; i < v.size() ; ++i ) { 
    if ( e[i] != v[i]) 
      return false;
  }


  return true;
  
}



};


