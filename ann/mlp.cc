
/*
    ann  An artificual neural network library
    Copyright (C) 2000,2003  John Darrington

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

#include "layer.h"

#include "fm.h"
#include "mlp.h"
#include "outputMap.h"
#include <algorithm>

#include <math.h>
#include <string>
#include <iomanip>
#include <list>
#include <stdexcept>
#include <iostream>

#include "persist.h"

const static char RCSID[]="$Id: mlp.cc,v 1.13 2003/05/14 05:50:16 john Exp $";
#include "log.h"


// This module implements a Multi-Layer perceptron network class
// This particular implementation has three layers (including input layer)



using namespace std;

namespace ann {

  
class Sigmoid : public ActivationFunction { 
public:
  Sigmoid (float slope): K(slope){};

  float operator()(float x) const 
  { 
    // something funny seems to be happening with exp in my math library ...
    assert(exp(0)==1.0);

    return 1.0/(1.0 + exp(-K*x) );

  }

  void save(std::ostream &os) const 
  {
    write(os,K);
  }

  void load(std::istream &is) {
    read(is,K);
  }


  float slope() const 
  { return K; };

private:
  float K;
};

// Create a network with specified input and 
// output dimensions
//
Mlp::Mlp(
	int inputDim,   // size of input layer
	int outputDim,  // size of output layer
	float alpha,    // the learning rate
	float momentum, // the learning momentum
	float t,        // the acceptable error level
	float k,        // slope of sigmoid function
	int hSize	// size of hidden layer
	):OP_SIZE(outputDim),IP_SIZE(inputDim),LEARNING_RATE(alpha),TOLERANCE(t),MOMENTUM(momentum)
{
	activationFunction = new ann::Sigmoid(k);

	assert ( hSize >=0);
	H_SIZE = (hSize==0)?IP_SIZE/2:hSize;

	// First  the network allocates an abitrary input of the appropriate size.
	// At this stage the values of the inputs are undefined.
	// The real data is copied to it in the training stage.
	extInput = new ann::ExtInput(inputDim);

	// Next the intermediate (hidden) layer is created.
	// All neurons use the sigmoid function as their activation	
	// function.
	hidden = new ann::Layer(H_SIZE,*activationFunction,true);
	// And the input to the hidden layer is the network's external input
	// created previously.
	hidden->setInput(*extInput);

	// The output layer is created in the same way as the hidden layer.
	output = new ann::Layer(OP_SIZE,*activationFunction,true);
	// It's input is the hidden layer.
	output->setInput(*hidden);

	// The network needs access to the weight matrices of both the
	// hidden and output layers in the training process.
	hiddenLayerWeights = &(hidden->getWeights() ) ; 
	outputLayerWeights = &(output->getWeights() ) ; 

	oldHiddenLayerWeights = new matrix(*hiddenLayerWeights);
	oldOutputLayerWeights = new matrix(*outputLayerWeights);

	outputmap = 0;

};

// So that we can use a previously trained network for recalling,
// we need to be able to create a network from  a file in non
// volatile storage
// ( If we had a persistable base class this would be much easier,
// perhaps I should have used Java . . . . )
Mlp::Mlp(std::ifstream &ifs)
{
	assert(ifs.is_open());



	// Perhaps we should read a magic number from the file . . ?


	// Allocate memory for the activation function
	// The K value will get overwritten in the call to load()
	activationFunction = new Sigmoid(0);

	// First comes the definition of the network, including a
	// string for identification purposes
	// and the dimensions of the various layers
	string desc;
	cout << "Loading network: ";

	load(ifs,desc);
	cout << desc << endl;

	// So create the layers and add assign the inputs appropriately

	extInput = new ann::ExtInput(IP_SIZE);

	hidden = new ann::Layer(H_SIZE,*activationFunction,true);
	hidden->setInput(*extInput);

	output = new ann::Layer(OP_SIZE,*activationFunction,true);
	output->setInput(*hidden);


	// and we need a pointer to the weight matrices
	hiddenLayerWeights = &(hidden->getWeights() ) ; 
	outputLayerWeights = &(output->getWeights() ) ; 

	// so that we can initialise them with the previously 
	// trained weights

	hiddenLayerWeights->load(ifs); 
	outputLayerWeights->load(ifs);

	oldHiddenLayerWeights = 0;
	oldOutputLayerWeights = 0;


	// finally load the output pattern map

	int mapSize=0;
	char *data = (char *) &mapSize;
	for(int i = 0 ; i < sizeof (mapSize) ; ++i) {
		ifs.get(data[i]);
	}


	outputmap = new OutputMap;

	for  ( int i = 0 ; i < mapSize ; ++i ) {
	  // First comes the length of the name
	  int nameSize;
	  read(ifs, nameSize);

	  // Next comes the name itself
	  string name;
	  for(int i = 0 ; i < nameSize ; ++i ) {
	    char c;
	    read(ifs,c);
	    name+=c;
	  }
	  ann::vector v;
	  v.load(ifs);
	  (*outputmap)[v]=name;
	}



};


// The Mlp constructor is non trivial, so the network must be
// destroyed carefully
Mlp::~Mlp()
{	


  // delete all the layers, in reverse order to their 
  // creation.

  delete oldHiddenLayerWeights ; 
  delete oldOutputLayerWeights ;


  delete output;
  delete hidden;
  delete extInput;


  delete activationFunction;

  delete outputmap;

}


// Each lesson is taught to the network
float
Mlp::teach(const Lesson &lesson) 
{


  const float K=activationFunction->slope();

  const ann::ExtInput &pattern=lesson.first;
  const ann::vector &target=lesson.second;


  assert(target.size()== OP_SIZE);

  if (pattern.size()!=IP_SIZE) {
    string ost("Pattern size does not match network input size");

    throw length_error(ost);
  }
	
  ann::log() << "Pattern " << pattern ;

  const int hdim = hidden->getDimension() ;
	
  const ann::vector hUnity(hdim);
	

  const ann::vector oUnity(output->getDimension());

  // copy the pattern into the input layer
  *extInput=pattern;
  *extInput*=prescale;
	
  // The following learning rule relies upon the simple nature of
  // the sigmoid function, in terms of itself
  // It is specific to the sigmoid and won't work for other
  // activation functions
  // See Beale and Jackson section 4.4.1 for derivation
	
  const ann::vector o(output->getSource());
  const ann::vector diff(delta(target,o));
  const ann::vector h(hidden->getSource());
  int c=1;
  if ( output->bias() ) 
    c = 2;
	
  const ann::vector error(
			  K*h.product(hUnity - h).product(diff * 
							  (*outputLayerWeights).subc(c,outputLayerWeights->columns()) )
			  );
  ann::matrix outputMomentum(*outputLayerWeights - 
			     *oldOutputLayerWeights) ;

  outputMomentum *= MOMENTUM;
	
  const ann::vector deltaOutput( LEARNING_RATE * o.product(diff)) ;

  ann::matrix incrementO(outputLayerWeights->rows(),
			 outputLayerWeights->columns()
			 );

	
  incrementO.multiplyColumns(diff);

  const ann::vector unitvec(1);
	
  if ( output->bias())
    incrementO.multiplyRows(h.juxtapose(unitvec));
  else
    incrementO.multiplyRows(h);

  incrementO *= LEARNING_RATE;

  *outputLayerWeights += incrementO ;

  *outputLayerWeights += outputMomentum ;


	
  ann::matrix hiddenMomentum(*hiddenLayerWeights - 
			     *oldHiddenLayerWeights) ;
	
  hiddenMomentum *= MOMENTUM;
		
  ann::matrix incrementH(hiddenLayerWeights->rows(),
			 hiddenLayerWeights->columns()
			 );

  incrementH.multiplyColumns(error);

  if ( hidden->bias())
    incrementH.multiplyRows((*extInput).juxtapose(unitvec));
  else
    incrementH.multiplyRows(*extInput);
  incrementH *= LEARNING_RATE;

  *hiddenLayerWeights += incrementH ;
  *hiddenLayerWeights += hiddenMomentum ;
	

  ann::log() << "Target is " << target ;
  ann::log() << "Output is " << output->getSource() ;
	
  const vector difference(target - output->getSource()); 
	

  float err = difference.max();

  ann::log() << " error " << err << endl;

  return  err;

}

// This function is used a few times during the training process
//
ann::vector
Mlp::delta(const vector &target, const vector &o) const
{
  const float &K = activationFunction->slope();

  const ann::vector oUnity(OP_SIZE);
  const ann::vector diff(
			 K*o.product(oUnity - o).product(target - o)
			 );
  return diff;
}


// For any network to be useful, there needs to be a method
// to recall patterns presented to it.
std::string
Mlp::recall(const ann::ExtInput &pattern) const
{
  if(pattern.size()!=IP_SIZE)
    throw length_error("Pattern size must match the network input size");

  // Recalling a pattern is easy.

  // Copy the pattern into the input layer
  *extInput=pattern ;
  *extInput*=prescale;

  // and see what we get on the output layer
  ann::vector result(output->getSource());


  // Threshold the result
  for( int i = 0 ; i < result.size() ; ++i ) {
    if ( result[i] > 0.5 ) result[i]=1.0;
    else result[i]=0.0;
  }


  // and look it up in the output map
  
  OutputMap::const_iterator it = outputmap->find(result);

  assert(it != outputmap->end());

  return it->second;

}


// Part of the network file contains the dimensions 
// of the layers and other information
// The Mlp::load method sets these parameters from
// the file
void
Mlp::load(ifstream &ifs, string &comment)
{
#if 0 
  // some operating systems require this
  ifs >> setiosflags(ios::binary);
#endif

  // First comes the length of the comment
  int comment_size;
  read(ifs, comment_size);

  // Next comes the comment itself
  comment="";
  for(int i = 0 ; i < comment_size ; ++i ) {
    char c;
    read(ifs,c);
    comment+=c;
  }

  // the tolerance
  read(ifs,TOLERANCE);

  // And the other parameters
  //  .
  //  .
  read(ifs,OP_SIZE);
  read(ifs,H_SIZE);
  read(ifs,IP_SIZE);
  activationFunction->load(ifs);
  read(ifs,prescale);

}

// Similarly , the Mlp::save method writes a network to 
// a file
void
Mlp::save(ofstream &ofs, const std::string &comment) const
{
#if 0
  ofs << setiosflags(std::ios::binary);
#endif

  // First write out the size of the comment string.
  const int comment_size=comment.length();
  write(ofs,comment_size);
	

  // Then the comment itself
  for(int i = 0 ; i < comment_size ; ++i ) {
    char c = comment[i];
    write(ofs,c);
  }

  // Then the tolerance
  write(ofs,TOLERANCE);

  // And the other parameters
  //  .
  //  .
  write(ofs,OP_SIZE);
  write(ofs,H_SIZE);
  write(ofs,IP_SIZE);
  activationFunction->save(ofs);
  write(ofs,prescale);

  // And the values of all the weights
  hiddenLayerWeights->save(ofs); 
  outputLayerWeights->save(ofs);

  /* why am I saveing these here, but not loading them in Mlp::load ?? */
	
  // finally save the output pattern map
  typedef OutputMap::const_iterator CI;
 
  int mapSize ;

  if ( !outputmap ) 
    mapSize = 0;
  else
    mapSize = outputmap->size();
	
  char *data = (char *) &mapSize;
  for(int i = 0 ; i < sizeof (mapSize) ; ++i) {
    ofs.put(data[i]);
  }

  if ( outputmap ) {
  for ( CI ci = outputmap->begin() ; ci != outputmap->end() ; ++ci ) {
    const string name = ci->second;
    const int namelen = name.length();
    write(ofs,namelen);

    // Then the name itself
    for(int i = 0 ; i < namelen ; ++i ) {
      char c = name[i];
      write(ofs,c);
    }

    (ci->first).save(ofs);
  }

  }

}



// Train the network with all the lessons in lessonSet
// until it knows them all.
// Returns 1 if training was sucessfull 0 otherwise.
int
Mlp::train(const FeatureMap &fm, int maxEpochs)
{
  if ( static_cast<int>(pow(2.0, OP_SIZE) ) < fm.size())
    throw length_error("Feature map is too large for network output");

  if ( ! fm.isBalanced() ) 
    throw runtime_error("Feature map is unbalanced");


  delete outputmap;
  outputmap = new OutputMap(fm);


  typedef std::vector<Lesson>::const_iterator CI;
	
  int risingCount=0;
  float error = 0;


  typedef std::vector<Lesson> LessonSet;


  float biggest=0.0;
  std::list<LessonSet> theLessonSets;

  // Here, we copy all the patterns from the feature map into 
  // the set of the lesson sets
  typedef FeatureMap::const_iterator CMI;
  for( CMI ci = fm.begin() ; ci != fm.end() ; ++ci ) {
    const set<ExtInput> & classSet = ci->second;
    const string className = ci->first;

    OutputMap::const_iterator it ;

    for ( it = outputmap->begin() ; it != outputmap->end() ; ++it ) {
      if ( it->second == className ) {
	break;
      }
    }
    if ( it == outputmap->end() ) 
      throw runtime_error(className + " exists in training data, but not in output map");


    ann::vector target(it->first);

    LessonSet ls;
    typedef set<ExtInput>::const_iterator CSI;
    for (CSI csi = classSet.begin(); csi != classSet.end(); ++csi ) { 
      ls.push_back(make_pair(*csi,target));
      biggest = max(biggest,csi->max());      
    }

    theLessonSets.push_front(ls);

  }

  prescale=1.0/biggest;

  epochs_=0;

  do {
    std::list<LessonSet> localLessonSets = theLessonSets;

    error = 0 ;

    while ( ! (localLessonSets.begin())->empty() ) { 
	
      typedef std::list<LessonSet>::iterator LSI;

      for(LSI lsi = localLessonSets.begin() ;
	  lsi != localLessonSets.end() ; 
	  ++lsi) { 
	LessonSet &ls = *lsi;

	// Lessons must be learnt in random order.

	// It's easiest to do this by shuffling the set
	random_shuffle(ls.begin(),ls.end());

	// and use the last element
	Lesson &thisLesson=ls.back();



	error  = max(error,teach(thisLesson));
	ls.pop_back();

	*oldHiddenLayerWeights = *hiddenLayerWeights;
	*oldOutputLayerWeights = *outputLayerWeights;

      }

    }

    cout << "Error is " << error << "\r";
    cout.flush();
    ann::log() << "Max error is " << error << "\n";

    ++epochs_;

  } while (error > TOLERANCE);

  cout << endl;
  cout << "Number of epochs: " << epochs_ << endl;

  return 1;

}

// For confidence, we have a method which streams the network's,
// parameters 
std::ostream & 
operator<<(ostream &os, const Mlp &mlp)
{
  os << "Input Layer Size " <<  mlp.IP_SIZE << endl;
  os << "Hidden Layer Size " << mlp.H_SIZE << endl;
  os << "Output Layer Size " << mlp.OP_SIZE << endl;
  os << "Tolerance : " <<       mlp.TOLERANCE << endl;
  os << "K factor : " <<        mlp.activationFunction->slope() << endl;
  os << "Prescale factor is " << mlp.prescale << endl;
  return os;
}



ostream & 
operator<<(ostream &os, const FeatureMap &fm) {
  typedef FeatureMap::const_iterator CI;

  for (CI ci = fm.begin() ; ci != fm.end() ; ++ci ) {
    os << ci->first << ":";
    const set<ExtInput> theSet(ci->second);
    typedef set<ExtInput>::const_iterator CSI;
    for (CSI csi = theSet.begin(); csi != theSet.end() ; ++csi) { 
      os << *csi << endl;
    }
    os << endl;
  }
  return os;
}


void 
FeatureMap::addFeature(const string &className, const ann::ExtInput &feature) {
  if ( -1 == vecSize ) 
    vecSize=feature.size();

  if ( feature.size() != vecSize )
    throw length_error("All feature vectors must be the same size");

  (*this)[className].insert(feature);
}


void 
Mlp::setOutputMap(const OutputMap &om)
{
  *outputmap = om;
};

const OutputMap &
Mlp::getOutputMap() const
{
  return *outputmap;
}



} // end namespace ann



