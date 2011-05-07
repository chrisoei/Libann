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
const static char RCSID[]="$Id: kohonen.cc,v 1.8 2003/05/14 05:50:16 john Exp $";

#include "kohonen.h"
#include "persist.h"

#include <stdexcept>
#include "layer.h"
#include <set>
#include <vector>
#include <limits.h>
#include <float.h>
#include <math.h>

#include <fstream>
#include <iomanip>

using namespace ann;
using namespace std;

#define abs(x) ( ((x)>=0) ? (x) : (-x) )


class Heaviside : public ActivationFunction { 
public:
  float operator()(float x ) const { 
    if ( x > 0 ) 
      return 1.0;
    else
      return 0.0;
  }
  void save(std::ostream &os) const 
  {
    // Nothing needs doing here
  }

  void load(std::istream &os)
  {
    // Nothing needs doing here
  }



};

int
round(float f)
{
  int intpart = static_cast<int>(f);
  float fracpart = f - intpart;

  if (fracpart >0.5 ) 
    return intpart+1;
  else 
    return intpart;

}

Kohonen::Kohonen(int inputs, int gridsize)
{
  if (inputs < 1 ) 
    throw out_of_range("input size zero or negative");

  if (gridsize < 1 ) 
    throw out_of_range("grid size zero or negative");


  outputLayerSize=gridsize*gridsize;
  inputLayerSize=inputs;

  inputLayer = new ExtInput(inputs);

  activationFunction = new Heaviside ;

  outputLayer = new Layer(outputLayerSize,*activationFunction);

  outputLayer->setInput(*inputLayer);

}


int
Kohonen::getNodeFromCoords(int x, int y) const
{
  int node = node*x + y;  
}

void 
Kohonen::getCoordsFromNode(int node, int &x, int &y) const 
{
  int grid = static_cast<int>(sqrt(outputLayerSize));

  x = node/grid;
  y = node%grid;
}



int 
Kohonen::neighbourDistance(int node1, int node2) const
{

  // This implementation uses the maximum absolute distance
  int x1,y1;
  int x2,y2;
  getCoordsFromNode(node1,x1,y1);
  getCoordsFromNode(node2,x2,y2);

  return max(abs(x1-x2),abs(y1-y2));
}

void
Kohonen::train(const set<ExtInput> &trainingData,
	       float tolerance,   
	       float adaptionRate,
	       float shrinkRate ,
	       float initialRadius) const
{


  float gain=1.0;
  float radius= static_cast<float>(sqrt(outputLayerSize)/2) * initialRadius;

  float diff = FLT_MAX;

  set<int> neighbourhood;

  typedef set<int>::const_iterator CNI;

  matrix &weights = outputLayer->getWeights() ;
  weights.normaliseRows();

  matrix prevWeights(weights.rows(),weights.columns());
  prevWeights.setToZeroMatrix();

  typedef set<ExtInput>::const_iterator CI;
  typedef set<ExtInput>::iterator I;


  set<ExtInput> normalisedData;
  // Normalise the data and check it for consistency
  //
  for(I i=trainingData.begin(); i!=trainingData.end() ; ++i) {
      if ( i->size() != inputLayer->size() ) 
	throw invalid_argument("training data has invalid size");

      ann::ExtInput e = *i;
      e.normalise();
      normalisedData.insert(e);
  }

  // The normalised set *could* be smaller than the training set
  // (if any of the training data have the same angle)

  while (diff > tolerance ) {

    for(CI ci=normalisedData.begin(); ci!=normalisedData.end() ; ++ci) {

      int jstar=-1;

      float minDistance=FLT_MAX;

      for ( int j = 0 ; j < outputLayerSize ; ++j) {
	float distance = 0;

	ann::vector currentRow=outputLayer->getWeights().row(j);

	for( int i = 0 ; i < (*ci).size() ; ++i ) { 
	  float diff = (*ci)[i] - currentRow[i] ;
	  distance+= diff*diff;
	}
      
	if ( distance < minDistance) {
	  minDistance = distance;
	  jstar = j;
	}
      }

      assert(jstar != -1);

      {
	// Redefine neighbourhood

	neighbourhood.clear();


	for (int i = 0 ; i < outputLayerSize ; ++i ) {
	  if ( neighbourDistance(i, jstar) <= round(radius) ) 
	    neighbourhood.insert(i);	
	}
    
      }

      matrix weightAdjustment(weights.rows(),weights.columns());
      weightAdjustment.setToZeroMatrix();

      for( CNI cni = neighbourhood.begin() ;
	   cni != neighbourhood.end() ; 
	   ++cni)     {
	int node = *cni;
	const vector &nodeWeights = weights.row(node);
	for (int i = 0 ; i < nodeWeights.size() ; ++i) {
	  float a = gain*((*ci)[i] - nodeWeights[i]);
	  weightAdjustment.set(node,i,a);
	}
      }
      weights+=weightAdjustment;

      weights.normaliseRows();

    }

    diff = (weights - prevWeights).max() ;

    prevWeights=weights;

    gain*=adaptionRate;
    radius*=shrinkRate;

  }
  
}


ann::vector
Kohonen::recall(const ExtInput &pattern) const
{

  if ( pattern.size() != inputLayer->size() ) 
      throw invalid_argument("recall data has invalid size");

        
  // Copy the pattern into the input layer
  *inputLayer=pattern;

  // Normalise the input layer
  inputLayer->normalise();
        
  // and see what we get on the output layer
  return outputLayer->getSource();

}

Kohonen::~Kohonen()
{
  delete outputLayer;
  delete inputLayer;
  delete activationFunction;
}


 
// Save a network to ofs
void 
Kohonen::save(ofstream &ofs) const
{
#if 0
  ofs << setiosflags(ios::binary);
#endif

  write(ofs,outputLayerSize);
  write(ofs,inputLayerSize);


  // And the values of the weights
  matrix &weights = outputLayer->getWeights() ;

  weights.save(ofs);

}


void
Kohonen::load(ifstream &ifs)
{
#if 0
  // some operating systems require this
  ifs >> setiosflags(ios::binary);
#endif


  // read the dimensions from the stream
  read(ifs,outputLayerSize);
  read(ifs,inputLayerSize);


  // Weight values are read in the constructor


}

// Construct a Kohonen network from a previously saved file
Kohonen::Kohonen(ifstream &ifs)
{
  if (!ifs) 
    throw runtime_error("stream is not open");

  if (ifs.fail())
    throw runtime_error("cannot read from stream");



  // The load method sets the inputLayerSize and the outputLayerSize
  load(ifs);

  activationFunction = new Heaviside ;

  inputLayer = new ExtInput(inputLayerSize);

  outputLayer = new Layer(outputLayerSize,*activationFunction);

  outputLayer->setInput(*inputLayer);


  // and we need a pointer to the weight matrix
  ann::matrix *wm  = &(outputLayer->getWeights() ) ;

  // so that we can initialise it with the previously 
  // trained weights

  wm->load(ifs); 

}


void
Kohonen::print() const
{
  cout << "Kohonen Network\n";

  cout << "input size: " << inputLayerSize << endl;
  cout << "output size: " << outputLayerSize << endl;

  cout << "Weights:\n ";
  cout << outputLayer->getWeights() <<endl;

}
