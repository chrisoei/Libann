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
const static char RCSID[]="$Id: word.cc,v 1.3 2003/05/14 05:50:16 john Exp $";

#include "ann/ann.h"
#include <string>
#include <math.h>
#include "word.h"

#include <stdexcept>
#include <ctype.h>

using namespace ann;

const int Word::bitsPerChar=5;

Word::Word(const std::string &s,int length):ExtInput(length)
{
  if ( length < bitsPerChar *s.length()) 
    throw std::length_error("word too small for string");


  // Note that the ann::vector constructor fills the vector with 1s
  for ( int i = 0 ; i < s.length() ; ++i ) {

    int num = toupper(s[i]) - 'A' + 1;

    for ( int j = 0 ; j < bitsPerChar ; ++j) { 
      float bit = float (num & 0x00000001 ) ;
      if ( bit == 0 ) bit = -1.0;
      num=num >>1;
      (*this)[i*bitsPerChar+j]=bit;
    }
  }
}


Word::Word(const ann::vector &v):ExtInput(v.size())
{
    for ( int j = 0 ; j < size() ; ++j) { 
      (*this)[j]=v[j];
    }
}

std::string
Word::toString() const
{

  // Note that the ann::vector constructor fills the vector with 1s

  std::string result;

  for (int i=0; i < dimension ; i+=bitsPerChar) { 
    int num=0;
    for ( int j = 0  ; j < bitsPerChar ; ++j ) { 
      if ((*this)[i+j]==1.0 )
	num+=int(pow(2,j));
    }
    if (num == 0x1F ) 
      break;

    char c = char( num + 'A' -1 );

    if (c >='A' && c <= 'Z' )
      result+=c;
    else 
      result+=".";
  }
  return result;
}

