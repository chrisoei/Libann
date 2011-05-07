
/*
    ann  An artificial neural network library
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

#ifndef NETWORK_H
#define NETWORK_H


#include "ann.h"
#include <string>
#include <map>
#include <set>

#include <iostream>

// $Id: mlp.h,v 1.14 2003/05/14 05:50:16 john Exp $

namespace ann {

  class Sigmoid ;

  class matrix;
  class Layer;

  class OutputMap;

  /// A function returning a sum of squares of the elements of
  /// a vector is required to test how close well the network
  /// is trained.
  inline float
  sumOfSquares(const ann::vector &vec)
  {
    typedef ann::vector::const_iterator CI;
    float sum=0;
    for(CI i=vec.begin() ; i != vec.end() ; ++i)
      sum += (*i) * (*i) ;
    return sum;
  }


  typedef std::pair<ExtInput , ann::vector> Lesson;

  class FeatureMap;


  class Mlp {

  friend std::ostream & 
    operator<<(std::ostream &os, const Mlp &mlp) ;


  public:
    Mlp(int inputs,  // size of input layer
	int outputs, // size of output layer
	float alpha, // learning rate
	float momentum, // learning momentum
	float t=0.5,     // tolerance of training
	float k=1, // slope of sigmoid function
	int hSize=0  // size of hidden layer (0 --> inputs/2)
	);

    // read a network definition from ifs, and construct
    // an object accordingly
    Mlp(std::ifstream &ifs);

    // Destroy the network
    ~Mlp();

    int epochs() const 
    { return epochs_ ;};

    // train the network with all the lessons in lessonSet,
    // until it knows them all, or until the epoch count 
    // is exceeded
    int train(const FeatureMap &fm, int maxEpochs=0);

    // Recall a pattern from a (preferably trained) network
    // The return value it the network output
    std::string recall(const ann::ExtInput &pattern) const;

    // Save a network to ofs
    void save(std::ofstream &ofs, const std::string &s) const;

    // Load a network from ifs
    void load(std::ifstream &ifs, std::string &s);

    // return the size of the network's input layer
    int inputs() const
    { return IP_SIZE ; } ;


    void setOutputMap(const OutputMap &om);

    const OutputMap &
      getOutputMap() const ;


  private:
    // teach the network a lesson
    float teach(const Lesson &) ;

    ann::vector delta (const vector &, const vector&) const;

    Sigmoid *activationFunction;

    // Private since class contains pointer members
    Mlp(const Mlp &);
    Mlp &operator=(const Mlp&);

  private:
    float LEARNING_RATE;
    int OP_SIZE;
    int IP_SIZE;
    int H_SIZE;
    float TOLERANCE;
    float MOMENTUM;
    ann::matrix *hiddenLayerWeights ;
    ann::matrix *outputLayerWeights ;
    ann::matrix *oldHiddenLayerWeights;
    ann::matrix *oldOutputLayerWeights;
    ann::Layer *output;
    ann::Layer *hidden;
    ann::ExtInput *extInput;
    OutputMap *outputmap;
    int epochs_;
    float prescale;
  };





} // end namespace ann
#endif
