
/*
    ann  An artificual neural network library
    Copyright (C) 2000  John Darrington

    This program is free software; you can redistribute it and/or modify
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

#include "matrix.h"
#include "layer.h"


const static char RCSID[]="$Id: layer.cc,v 1.8 2003/05/14 05:50:16 john Exp $";

namespace ann {


// Construct a network layer with dim units
//
Nlayer::Nlayer(int dim, const ActivationFunction &af, bool bias):activationFunction(af)
{ 
	dimension=dim;
	weightMatrix = 0;
	input = 0;
	hasBias = bias;
};

// Set the input source for this layer
//
void
Layer::setInput(const InputSource &i)
{ 	
	input = &i; 

	// We have to wait until now, before defining the weight matrix for the
	// layer, because until now we didn't know what size it was.
	int inputDim = i.getDimension();
	if (bias()) ++inputDim;
	weightMatrix  = new matrix(dimension,inputDim);

	// Set weights to random values
	weightMatrix->randomise();
};


// Return the output value of this layer
//
const ann::vector 
Nlayer::getSource() const 
{
  assert(weightMatrix);
  assert(input);
  vector layerOutput(dimension);

  // first set the output to be the weighted sum
  // of the input values

  ann::vector thebias(0);
  if ( bias()) { 
    thebias.resize(1);
    thebias[0] = 1.0;
  }
  layerOutput = (*weightMatrix) * (input->getSource()).juxtapose(thebias);

  // then apply the activation function for each output
  for(VI i=layerOutput.begin(); i!=layerOutput.end() ; ++i)
    *i = activationFunction(*i);

  return layerOutput;
}

// destroy a network layer
Nlayer::~Nlayer()
{
       delete weightMatrix;
       weightMatrix = 0;
}

} // end namespace ann

