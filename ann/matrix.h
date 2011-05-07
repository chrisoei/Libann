
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

#ifndef MATRIX_H
#define MATRIX_H

#include "ann.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>

// $Id: matrix.h,v 1.18 2003/05/14 05:50:16 john Exp $



namespace ann {

// A class to represent matrices
// A rather crude implementation of  a matrix class
// requires a lot of tidying (bounds checking etc);
// perhaps a vector of vectors is the wrong way to 
// implement it anyway . . . . 
//
class matrix {

friend std::ostream &
operator<<(std::ostream& os, const ann::matrix &mtx);

friend ann::vector 
ann::vector::operator*(const matrix &mat) const;

friend ann::matrix
  operator-(const matrix &m1, const matrix &m2) ;

friend ann::matrix
  operator+(const matrix &m1, const matrix &m2) ;

friend ann::matrix& 
operator*=(matrix &, float f) ;

// Comparison operators
friend bool
  operator==(const matrix &m1, const matrix &m2) ;

friend inline bool
operator!=(const matrix &m1, const matrix &m2) 
{
  return ( ! (m1 == m2));
}





public:
	matrix():m_(0),rows_(0),cols_(0){}
	
	// Create a unity matrix of r rows and c columns
	//
	matrix(int r, int c):m_(r),rows_(r),cols_(c){
		int x=0;
		int y=0;
		typedef std::vector<ann::vector>::iterator I;

		for(I i = m_.begin() ; i != m_.end() ; ++i ) {
			x=0;
			i->resize(cols_);
			for(VI j = i->begin() ; j != i->end() ; ++j ) 
				*j=1;
			++y;
		}
	};


	// Make all rows have a modulus of one
	void
	normaliseRows();

	// Set element x,y to val
	void
	set(int x, int y, float val)
	{
		m_[x][y]=val;
	}


	// return  element x,y
	float
	get(int x, int y) const
	{
	  return m_[x][y];
	}


	int
	rows() const
        { 
	  return rows_; 
	}


	int
	columns() const
        { 
	  return cols_; 
	}

	// Return the i th row 
	ann::vector 
	row(int i) const
	{    
	  return m_[i] ; 
	}

 	// Post multiply the matrix by a vector vec
 	//
 	ann::vector 
 	operator*(const ann::vector &vec) const;

        matrix&
	  ann::matrix::operator+=(const matrix &m2) ;
 
 
	// Return a matrix comprising the columns n...m of this matrix
	ann::matrix
	  subc(int n,int m) const;



	// set the matrix to the unit matrix
	//
	void setToUnitMatrix();

	// set the matrix to the zero matrix
	//
	void setToZeroMatrix();



	// set the matrix elements to have random values between -1 and +1
	//
	void randomise();


	// Like randomize, but the matrix is gauranteed to be symetrical
	// Applicable only to square matrices
	//
	void randomiseSymetrical();


	// Add the vector vec, to all columns of the matrix
	//
	void raiseColumns(const vector &vec) ;
	

	// Add the vector vec, to all rows of the matrix
	//
	void raiseRows(const vector &vec) ;

	// Multiply all columns of the matrix
	// by the vector vec, 
	//
	void  ann::matrix::multiplyColumns(const vector &vec) ;


	// Multiply all rows of the matrix
	// by the vector vec, 
	//
	void  ann::matrix::multiplyRows(const vector &vec) ;


	// Return the absolute maximum element
	// of the matrix
	float max() const ;

	
	// Methods to test properties of matrices

	bool isSquare() const
	  { return (rows_ == cols_ ) ; } ;

	bool isSymetrical() const ; 



public:
	// A couple of methods to try and make this class
	// persistable. Not very neat but they seem to 
	// work.
	void save(std::ofstream &ofs) const;
	void load(std::ifstream &ifs) ;

protected:
	std::vector<ann::vector> m_;
	int rows_;
	int cols_;
};


matrix operator-(const matrix &m1, const matrix &m2) ;
matrix operator+(const matrix &m1, const matrix &m2) ;




 ann::matrix& operator*=(matrix &, float f) ;


 class squareMatrix : public matrix { 
 public:
   explicit squareMatrix(int d):matrix(d,d) {};

   // set the matrix to the identity matrix
   //
   void setToIdentityMatrix()
     { 
       setToZeroMatrix();
       for ( int i = 0 ; i < rows_ ; ++i ) { 
	 set(i,i,1.0);
       }
     }
 };

} // end namespace ann


#endif




