#ifndef GLYPH_H
#define GLYPH_H
/*

    Copyright (C) 2002  John Darrington 

    $Id: glyph.h,v 1.2 2002/07/22 05:54:43 john Exp $

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

#include <ann/ann.h>

#include <string>

class Glyph : public ann::ExtInput {
public:
  // Construct a glyph from the file called `filename'
  // if bipolar is true, then it will be a bipolar vector
  // otherwise a binary vector
  Glyph(const std::string &filename,bool bipolar=false);


  // print an ascii art representation of the glyph to a ostream
  void
    print(std::ostream &os) const ;


 private:
  bool bip;
};


#endif /* GLYPH_H */
