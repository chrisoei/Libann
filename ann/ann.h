#ifndef ANN_H
#define ANN_H
/*

    Copyright (C) 2002,2003  John Darrington 

    $Id: ann.h,v 1.14 2003/05/14 13:47:24 john Exp $

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


/*! \file ann.h

  \brief Common user space declarations for the Neural Net library.

  This file is the common interface to libann.  It contains all
  declarations for all structures (other than the networks
  themselves) necessary to use a Neural Net.

*/

#include <string>
#include <vector>
#include <fstream>



namespace ann {



  /// Return the version number of the library
  std::string
  version();



class matrix;

/// A vector (array) of floating point numbers
class vector : public std::vector<float> {
public:
	/// create a vector of size i
	vector(int i): std::vector<float>(i)
	{
		for(int x=0; x < i ; ++x)
			(*this)[x]=1; 
	};

	/// default constructor, a vector of size 0
	vector() {};


	/// Return the value of the absolute maximum element of a vector
	float  max() const;


	/// return the modulus of this vector;
	float modulus() const ;

	/// Make the modulus equal to unity
	/// Leave the direction unchanged
	void normalise() { 
	  float mod = modulus(); 
	  *this/= mod ;
	}



	/// Return a new vector: [ vec *this ]
	vector
	juxtapose(const vector &vec) const;

	/// Return a sub vector.
	/// \param start The starting element.
	/// \param stop  The last element.
	vector
	  subvec(int start, int stop) const ;


	/// save the vector
	/// \param ofs Stream to write
	void
	  vector::save(std::ofstream &ofs) const ;

	/// load from stream
	/// \param ifs stream to read
	void
	  vector::load(std::ifstream &ifs) ;



	vector operator-(const vector &vec) const;
	vector operator+(const vector &vec) const;
	vector operator*(const matrix &mat) const;
	vector product(const vector &vec) const;

        vector& operator+=(const vector &v);

	// Divide a vector by a scalar
        vector& operator/=(const float &f);


	
};

ann::vector operator*(const vector &, float f) ;
ann::vector operator*(float f, const vector &) ;

ann::vector& operator*=(vector &, float f) ;

/// Return the dot product (inner product) of two vectors
float dot(const ann::vector& v1, const ann::vector& v2);




typedef ann::vector::const_iterator CVI;
typedef ann::vector::iterator VI;

// print the transpose of the vector to os
std::ostream & operator<<(std::ostream& os, const ann::vector &v) ;



/*! \brief An abstract base class for inputs to networks or network
  layers.

  The \c InputSource class is a pure abstraction.  It represents
  anything that can be an input to a network layer.  Typically this is
  an \c ExtInput (ie. a feature vector) or another network layer.
*/
class InputSource {
public:
	// return the vector to be used as input
	virtual const ann::vector getSource() const = 0;

	virtual int getDimension() const { return dimension; };
	
protected:
	int dimension;
};


/*! \brief An external input to a network

  An ExtInput is something that can be used as an input to a neural
  net.  It's implementation is nothing more than a vector of \c floats
*/
class ExtInput : public virtual InputSource, public vector {
public:

  // construct a vector of dim external inputs 
  ExtInput(int dim): vector(dim){dimension=dim; }; 

  // In case of external inputs, the implementation is of getSource is trivial
  // It's the object itself
  virtual const ann::vector
  getSource() const 
  {
     return *this;
  }

  ExtInput &
  operator=(const ann::vector &v) ;


};


bool operator==(const ExtInput &e, const ann::vector &v);

inline bool operator!=(const ExtInput &e, const ann::vector &v)
{

  return !(e == v);
}



}

#endif /* ANN_H */
