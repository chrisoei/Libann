#ifndef PERSIST_H
#define PERSIST_H
/*

    Copyright (C) 2002  John Darrington 

    $Id: persist.h,v 1.7 2003/05/14 05:50:16 john Exp $

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

namespace ann {

void _write(std::ostream &os, unsigned char *data, int size);

void _read(std::istream &is, unsigned char *data, int size);

};

//@{

/*! 
  The read and write macros should be used \em only for primitive types.
  That is, only
  \li \c int
  \li \c char
  \li \c float
  \li etc.
  
  Classes and structs will not work.
*/

/// Read a datum from a std::istream
#define read(is,datum) _read(is,(unsigned char*) (&datum),sizeof(datum))

/// Write a datum to a std::ostream
#define write(os,datum) _write(os,(unsigned char*) (&datum),sizeof(datum))

//@}



#endif /* PERSIST_H */
