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
const static char RCSID[]="$Id: glyph.cc,v 1.4 2003/05/14 05:50:16 john Exp $";

#include <ann/ann.h>

#include <string>
#include "glyph.h"

#include <fstream>
#include <stdexcept>
#include <math.h>

Glyph::Glyph(const std::string &filename, bool bipolar):ExtInput(0)
{
  std::ifstream ifs(filename.c_str());
  if ( !ifs ) 
    throw std::runtime_error("Cannot open file " + filename);


  while (!ifs.fail()) { 
    char c;
    ifs.get(c);
    
    // ignore whitespace
    if ( isspace(c)) continue;


    if ( '#' == c ) 
      push_back(1.0);
    else { 
      if ( bipolar ) 
	push_back(-1.0);
      else
        push_back(0.0);
    }
  }

  ifs.close();
};


void
Glyph::print(std::ostream &os) const
{
  const int dim = sqrt(size());

  for (int j = 0 ; j < dim ; ++j ) { 
  for (int i = 0 ; i < dim ; ++i ) { 
    if ( (*this)[j*dim + i] == 1.0 )  
      os << "#" ;
    else
      os << "." ;

  }
  os << std::endl;
  }
}
