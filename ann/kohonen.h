#ifndef KOHONEN_H
#define KOHONEN_H
/*

    Copyright (C) 2002,2003  John Darrington 

    $Id: kohonen.h,v 1.10 2003/05/14 13:47:24 john Exp $

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
#include <iostream>
#include <fstream>

namespace ann {

class ExtInput;
class Layer;
class vector;

class ActivationFunction;


class Kohonen {
public:

  // Construct a network with i inputs, and grid*grid outputs
  // The total number of outputs will always be square.
  Kohonen(int i, int grid);

  // Destroy the network
  ~Kohonen();
 
  // Train the network
  void
  train(const std::set<ann::ExtInput> &trainingData,
	       float tolerance=0.015,
	       float adaptionRate=0.75,
	       float shrinkRate=0.90,
	       float initialRadius=1.0) const;


  // Recall a  pattern from the network
  ann::vector
  recall(const ExtInput &p) const ;

	
  // Save a network to ofs
  void save(std::ofstream &ofs) const;

	
  // read a network definition from ifs, and construct
  // an object accordingly
  Kohonen(std::ifstream &ifs);


  // Print the network to cout
  // primarily usefull for debugging
  void print() const;
  


 private:
  int
  getNodeFromCoords(int x, int y) const ;

  void 
  getCoordsFromNode(int node, int &x, int &y) const ;

  // Return the topological distance between node1 and node2
  int 
  neighbourDistance(int node1, int node2) const;

  // Load a network from ifs
  void load(std::ifstream &ifs);


  // Private since the class contains pointer members
  Kohonen(const Kohonen &);
  Kohonen &operator=(const Kohonen &);


private:

  ExtInput *inputLayer;
  Layer    *outputLayer;
  int outputLayerSize;
  int inputLayerSize;
  ActivationFunction *activationFunction;

};


};

#endif /* KOHONEN_H */
