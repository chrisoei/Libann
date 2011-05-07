#ifndef HOPFIELD_H
#define HOPFIELD_H
/*

    Copyright (C) 2002  John Darrington 

    $Id: hopfield.h,v 1.6 2003/05/14 05:50:16 john Exp $

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

#include <set>

namespace ann {

  class Layer;
  class ExtInput;
  class ActivationFunction;
  class vector;
  class Mesh;

  // A Hopfield network class
  class Hopfield { 

  public:
    Hopfield(const std::set<ExtInput> &inSet) ; 
  
    ann::vector
      recall(const ann::ExtInput &ip) const ;

    ~Hopfield();

  private:
    Hopfield(const Hopfield &);
    Hopfield &operator=(const Hopfield &);

  private:
    Mesh *mesh;
    ActivationFunction *activationFunction;
    int units;

  };
};

#endif /* HOPFIELD_H */
