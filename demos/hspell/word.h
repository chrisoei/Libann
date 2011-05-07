#ifndef WORD_H
#define WORD_H
/*

    Copyright (C) 2002  John Darrington 

    $Id: word.h,v 1.2 2003/05/14 05:50:16 john Exp $

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

#include "ann/ann.h"
#include <string>

class Word : public ann::ExtInput { 
 public:
  Word(const std::string &s, int length);
  Word(const ann::vector &);

  std::string toString() const;
  
 private:
  const static int bitsPerChar;
};


#endif /* WORD_H */
