/*

    Copyright (C) 2002,2003  John Darrington 

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
const static char RCSID[]="$Id: mesh.cc,v 1.9 2003/05/14 05:50:16 john Exp $";

#include "mesh.h"
#include "matrix.h"
#include <set>
#include "log.h"
#include "ucount.h"

#include <stdexcept>

using namespace ann ;

Mesh::Mesh(int dim ,
	   const ActivationFunction &af, 
	   bool sync,
	   const UpdateFunction *updateFunc ): 
  Nlayer(dim,af,false), synchronous(sync), updateFunction(updateFunc)
{
  trace;

  weightMatrix  = new matrix(dim,dim);

  theUnits = new ExtInput(dim);

  matrix &weights = getWeights();

  typedef std::set<ExtInput>::const_iterator CI;

  for (int i = 0 ; i < dim ; ++i ) { 
    weights.set(i,i,0.0);
    for (int j = i+1 ; j < dim ; ++j ) { 
      float value = weights.get(i,j);
      weights.set(j,i,value);
    }
  }

  assert(weights.isSymetrical());

  clampedNeuron = new std::vector<bool>(dim);

  clamps = new ann::vector(dim);

  releaseClamps();
  

};



const ann::vector 
Mesh::getSource() const
{
  if ( synchronous ) 
    return getSourceSynchronous();
  else
    return getSourceAsynchronous();

}


const ann::vector 
Mesh::getSourceAsynchronous() const
{
  trace;

  const matrix &weights=getWeights() ;


  //  const_cast<matrix&>(weights).randomise();

  log() << "Weights:\n" << weights << std::endl;


  Ucount<ExtInput> uc;

  uc.push(*theUnits);
  
  do  { 


    log() << "Old value " << *theUnits << std::endl;

    // Pick an element to update
    const int element=static_cast<int>( (rand() / (float) RAND_MAX) 
					* dimension);

    // If the neuron is clamped, then set it to the clamped value and continue
    if ( (*clampedNeuron)[element] ) { 
      log() << "Unit " << element << " is clamped to " << (*clamps)[element] << std::endl;
      (*theUnits)[element] = (*clamps)[element];
      continue;
    }

    log() << "Updating node " << element << std::endl;

    const ann::vector &row = weights.row(element);

    float activation = dot(row, *theUnits ) ;

    (*theUnits)[element] = activationFunction(activation) ;

    log() << "New value " << *theUnits << std::endl;

    uc.push(*theUnits);


    if ( updateFunction) (*updateFunction)();


  } while ( uc.count() < dimension * 2);


  return *theUnits;

}

    
void 
Mesh::clamp(int e, float v)
{
  trace;

  (*clampedNeuron)[e] = true;
  (*clamps)[e]=v;

}

     
void 
Mesh::releaseClamps() 
{
  trace;


  for (int i = 0 ; i < dimension ; ++i )
    (*clampedNeuron)[i]=false;

}

    
const ann::vector 
Mesh::getSourceSynchronous() const
{
  trace;


  vector layerOutput(dimension);

  // first set the output to be the weighted sum
  // of the input values

  ann::vector thebias(0);
  if ( bias()) { 
    thebias.resize(1);
    thebias[0] = 1.0;
  }


  Ucount<ExtInput> uc;

  uc.push(*theUnits);
  
  do  { 


    log() << "Input : " << *theUnits << std::endl;

    layerOutput = (*weightMatrix) * (theUnits->getSource()).juxtapose(thebias);

    // then apply the activation function for each output
    for(VI i=layerOutput.begin(); i!=layerOutput.end() ; ++i)
      *i = activationFunction(*i);

    log() << "Output: " << layerOutput << std::endl;

    *theUnits = layerOutput;
    uc.push(*theUnits);


    if ( updateFunction) (*updateFunction)();


  } while ( uc.count() < 2 );


  return layerOutput;


}


Mesh &
Mesh::operator=(const vector &ip)
{

  if (  ip.size() != theUnits->size()) {
    throw std::range_error("Wrong size for vector assignment");
  }


  for (int i = 0 ; i < ip.size() ; ++i ) { 
    (*theUnits)[i]=ip[i];
  }

  return *this;
}


Mesh::~Mesh()
{
  delete theUnits;
  delete clamps;
  delete clampedNeuron;
}


float &
Mesh::operator[](int i)
{
  return (*theUnits)[i];
}


void 
Mesh::hebbianUpdate(float increment) const
{

  matrix &weights = getWeights();

  for ( int i = 0 ; i < getDimension() ; ++i ) { 
    for ( int j = 0 ; j < getDimension() ; ++j ) {
      if ( i == j ) continue ; 
      if ( ( *theUnits)[i] == +1 && (*theUnits)[j]==+1 )
	weights.set(i,j,weights.get(i,j)+increment);
    }
  }
}
