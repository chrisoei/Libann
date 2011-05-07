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
const static char RCSID[]="$Id: persist.cc,v 1.7 2003/05/14 05:50:16 john Exp $";

#include <iostream>
#define PERSIST_CC
#include "persist.h"


namespace ann {

  /*! \brief Write bytes to a \c std::ostream

    \param os The stream to be written.
    \param data A pointer to the bytes to be written.
    \param size The number of bytes to be written.
  */
  void _mywrite(std::ostream &os, unsigned char *data, int size)
  {
    for (int i=0; i < size ; ++i )
      os.put(data[i]);
  }

  /*! \brief Read bytes from a \c std::istream

    \param is The stream to be read.
    \param data A pointer to a location to store the bytes.
    \param size The number of bytes to be read.
  */
  void _myread(std::istream &is, unsigned char *data, int size)
  {
    is.read((char*)(data), size);
  }

	
};
