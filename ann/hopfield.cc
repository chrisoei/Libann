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
const static char RCSID[]="$Id: hopfield.cc,v 1.6 2003/05/14 05:50:16 john Exp $";

#include "hopfield.h"
#include "layer.h"
#include "mesh.h"
#include "log.h"

using namespace ann;


// This activation function works as follows:
// +1 if x > 0 
// -1 if x < 0
// same as previous value if x==0
//
class BiPolarStep : public ActivationFunction { 
public:
  float operator()(float x) const {
    float retval;
    if ( x == 0.0 ) 
      retval=previousValue;
    else if ( x > 0 ) 
      retval =  1.0;
    else
      retval = -1.0;

    previousValue = retval;
    return retval;
  }

  void save(std::ostream &os) const {};
  void load(std::istream &os)  {};
private:
  mutable float previousValue;
};





Hopfield::Hopfield(const std::set<ExtInput> &inSet) 
{
  trace;

  units = inSet.begin()->size();

  activationFunction = new BiPolarStep;

  mesh = new Mesh(units,*activationFunction);

  // Populate the weights with our patterns

  matrix &weights = mesh->getWeights();

  typedef std::set<ExtInput>::const_iterator CI;

  for (int i = 0 ; i < units ; ++i ) { 
    for (int j = 0 ; j < units ; ++j ) { 
      if ( i == j ) 
	weights.set(i,j,0.0);
      else {
	float value=0;
	for(CI ci = inSet.begin() ; ci != inSet.end() ; ++ci ) {
	  value+=(*ci)[i]*(*ci)[j];
	}
	weights.set(i,j,value);
      }
    }
  }

  assert ( weights.isSymetrical());


}


Hopfield::~Hopfield() {
  trace;
  delete activationFunction;
  delete mesh;
}


ann::vector
Hopfield::recall(const ann::ExtInput &ip) const
{
  trace;

  assert(mesh);


  *mesh = ip ; 

  return mesh->getSource();

}

