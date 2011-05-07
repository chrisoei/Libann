
/*
    ann  An artificial neural network library
    Copyright (C) 2000, 2003  John Darrington

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

#include <assert.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>

#include <iomanip>

const static char RCSID[] = "$Id: matrix.cc,v 1.20 2003/05/14 05:50:16 john Exp $";

namespace ann {

/*
This module implements a matrix class and a vector class.
*/

// =============Matrix Method Implementation=============


// Print the matrix to os
//
std::ostream &
operator<<(std::ostream& os, const ann::matrix &mtx)
{
	typedef std::vector<ann::vector>::const_iterator CI;
	for(CI i = mtx.m_.begin() ; i != mtx.m_.end() ; ++i ) {
		for(CVI j = i->begin() ; j != i->end() ; ++j ) 
			os << *j << "  ";
		os << std::endl;
	}
	return os;
}


// Set the matrix to the zero matrix (all 0s)
//
void
matrix::setToZeroMatrix()
{
	typedef std::vector<ann::vector>::iterator I;
	for(I i = m_.begin() ; i != m_.end() ; ++i ) {
		for(VI j = i->begin() ; j != i->end() ; ++j ) 
			*j=0;
	}
}


// Set the matrix to the unit matrix (all 1s)
//
void
matrix::setToUnitMatrix()
{
	typedef std::vector<ann::vector>::iterator I;
	for(I i = m_.begin() ; i != m_.end() ; ++i ) {
		for(VI j = i->begin() ; j != i->end() ; ++j ) 
			*j=1;
	}
}

// Set the matrix to random values from -1 to +1
//
void
matrix::randomise()
{
	typedef std::vector<ann::vector>::iterator I;
	for(I i = m_.begin() ; i != m_.end() ; ++i ) {
		for(VI j = i->begin() ; j != i->end() ; ++j ) 
			*j=((float) rand() *2.0 / RAND_MAX - 1.0 )/ (float) columns();
	}

}


// Set matrix to a random symetrical value
//
void
matrix::randomiseSymetrical()
{

  assert(isSquare());


  int i,j;

  for(i = 0 ; i < rows() ; ++i ) { 
    for(j = 0 ; j <=i ; ++j ) { 
      float value =((float) rand() *2.0 / RAND_MAX - 1.0 )/ (float) columns();
      set(i,j,value);
      set(j,i,value);
  
    }
  }

  assert(isSymetrical());

}


// Postmultiply the matrix by a vector, vec
// return the result.
//
ann::vector 
ann::matrix::operator*(const ann::vector &vec) const
{
	assert (cols_== vec.size() );
	vector product(m_.size());
	int n=0;

	typedef std::vector<ann::vector>::const_iterator CI;
	for(CI i = m_.begin() ; i != m_.end() ; ++i ) {
		float sum=0;
		int x=0;
		for(CVI j = i->begin() ; j != i->end() ; ++j )  {
			sum += *j * vec[x++];
		}
		product[n++]=sum ;
	}
	return product;
};

// Add the vector vec, to all columns of the 
// matrix
//
void
ann::matrix::raiseColumns(const vector &vec) 
{
	assert (rows_ == vec.size() );
	typedef std::vector<ann::vector>::iterator I;
	for(int j=0; j< cols_ ; ++j) {
		int k=0 ;
		for(I i = m_.begin() ; i != m_.end() ; ++i ,++k) {
			(*i)[j]+=vec[k];
		}
	}
}


// Add the vector vec, to all rows of the 
// matrix
//
void
ann::matrix::raiseRows(const vector &vec) 
{
	assert (cols_ == vec.size() );
	typedef std::vector<ann::vector>::iterator I;

	for(I i = m_.begin() ; i != m_.end() ; ++i ) {
	  (*i) += vec;
	}

}

// Multiply all columns of the matrix
// by the vector vec, 
//
void
ann::matrix::multiplyColumns(const vector &vec) 
{
	assert (rows_ == vec.size() );
	typedef std::vector<ann::vector>::iterator I;
	for(int j=0; j< cols_ ; ++j) {
		int k=0 ;
		for(I i = m_.begin() ; i != m_.end() ; ++i ,++k) {
			(*i)[j]*=vec[k];
		}
	}
}


// Multiply all rows of the matrix
// by the vector vec, 
//
void
ann::matrix::multiplyRows(const vector &vec) 
{
	assert (cols_ == vec.size() );
	typedef std::vector<ann::vector>::iterator I;

	for(I i = m_.begin() ; i != m_.end() ; ++i ) {
	  (*i) = vec.product(*i);
	}

}

// Return a matrix comprising the columns n...m of this matrix
ann::matrix
ann::matrix::subc(int n,int m) const
{
  assert ( n <= m);

  assert ( n > 0);
  assert ( n <= columns() );

  assert ( m > 0);
  assert ( m <= columns() );

  matrix result(rows(), m-n+1);

	
  typedef std::vector<ann::vector>::const_iterator CI;
  typedef std::vector<ann::vector>::iterator I;
	
  I ri = result.m_.begin(); 
  for(CI i = m_.begin() ; i != m_.end() ; ++i ,++ri ) { // for each row . . .
    for ( int j = n-1 ; j <m ; ++j) {
      (*ri)[j-n+1] = (*i)[j];
    }
  }

  return result;
  
}


 
 
// ============= Vector method implementations ==========

// This method writes a vector to a file 
void
vector::save(std::ofstream &ofs) const
{
#if 0 
        ofs << setiosflags(std::ios::binary);
#endif
	
	int vecSize = size();

	// write out the size
	char *data= (char *) &vecSize;
	for(int i = 0 ; i < sizeof (vecSize) ; ++i) {
		ofs.put(data[i]);
	}

	// and write out each element of the matrix
	for(int j = 0 ; j < size() ; ++j ) {
	  float val = (*this)[j];
	  data= (char *) &(val);
	  for(int k = 0 ; k < sizeof (val) ; ++k) {
	    ofs.put(data[k]);
	  }
	}
	
}



// The load function reads a vector from a file
void
vector::load(std::ifstream &ifs) 
{
#if 0
        ifs >> setiosflags(std::ios::binary);
#endif

	int vecSize=0;

	// Read the size
	char *data= (char *) &vecSize;
	for(int i = 0 ; i < sizeof (vecSize) ; ++i) {
		ifs.get(data[i]);
	}


	// adjust the dimension of the vector accordingly
	resize(vecSize);

	// and read every element into the matrix
	for(int j = 0 ; j < vecSize ; ++j ) {
	  float val;
	  data= (char *) &(val);
	  for(int k = 0 ; k < sizeof (val) ; ++k) {
	    ifs.get(data[k]);
	  }
	  (*this)[j] = val;
	}
	
}






// Scale a vector by the scalar quantity s;
//
vector& 
operator*=(vector &vec, float f)
{
  for(VI i=vec.begin(); i!=vec.end() ; ++i)
    *i *= f;
  return vec;
}

// Return the difference between two vectors
//
vector
vector::operator-(const vector &v) const
{
	assert(v.size()==size());
	vector diff;
	CVI i,j;
	for(i=begin(),j=v.begin() ; i!=end() ; ++i,++j)
		diff.push_back(*i-*j);

	return diff;
	
}



// Return the modulus of a vector
//
float
vector::modulus() const 
{
  float sum=0;
  CVI i;
  for(i=begin() ; i!=end() ; ++i) {
    sum += (*i) * (*i) ;
  }
  
  return sqrt(sum);
}


// Return a sub vector
vector
vector::subvec(int start, int stop) const 
{
  ann::vector retval(stop - start +1);

  for(int i = start ; i <= stop ; ++i )
    retval[i-start] = (*this)[i];
  
  return retval;
}

// Return the absolute maximum element of a vector
//
float
vector::max() const
{
  float result=0;

  CVI i;
  for(i=begin() ; i!=end() ; ++i)
    result = std::max(result,(float) fabs(*i));

  return result;
}

// Add a vector to this one
//
vector&
vector::operator+=(const vector &v)
{
	assert(v.size()==size());

	VI i;
	CVI j;
	for(i=begin(),j=v.begin() ; i!=end() ; ++i,++j)
	  *i += *j;
	
	return *this;
	
}



// Divide a vector by a scalar
//
vector&
vector::operator/=(const float &f)
{

	VI i;
	for(i=begin() ; i!=end() ; ++i)
	  *i /= f;
	
	return *this;
}


// Return the sum of  two vectors
//
vector
vector::operator+(const vector &v) const
{
	vector sum(*this);
	sum+=v;
	return sum;
	
}


	
// Return a new vector: [ vec *this ]
ann::vector
ann::vector::juxtapose(const vector &vec) const
{
	vector result;
	CVI i;
	for(i=vec.begin(); i!=vec.end() ; ++i)
		result.push_back(*i);

	for(i=begin() ; i!=end() ; ++i)
		result.push_back(*i);
	return result;
}




// Scale a vector by the scalar quantity s;
//
ann::vector 
operator*(float f, const vector &vec) 
{
	vector scaled(vec) ;
	for(VI i=scaled.begin(); i!=scaled.end() ; ++i)
		*i *= f;
	return scaled;
	
}

// Return a vector which is this vector postmultiplied by
// a matrix mat
vector
vector::operator*(const matrix &mat) const
{
	assert (mat.rows_ == size() );
	vector product(mat.cols_);

	typedef std::vector<ann::vector>::const_iterator CI;

	for(int i=0 ; i < mat.cols_; ++i) {
		float sum=0;
		int n=0;
		for(CVI j = begin() ; j != end() ; ++j ,++n) {
			sum+= mat.m_[n][i]* *j;
		}
		product[i]=sum;
	}
	return product;
}

// Return a vector whose terms are the products
// of the terms of the input vectors
//
vector
vector::product(const vector &v) const
{
	assert(v.size()==size());
	vector prod;
	CVI i,j;
	for(i=begin(),j=v.begin() ; i!=end() ; ++i,++j)
		prod.push_back((*i)*(*j));

	return prod;
}


// print the transpose of the vector to os
//
std::ostream &
operator<<(std::ostream& os, const ann::vector &v)
{
	os << "( ";
	for(CVI i=v.begin() ; i!=v.end() ; ++i)
		os << *i << " ";
	os << ")T";

	return os;
}


// The load function reads a matrix from a file
void
matrix::load(std::ifstream &ifs) 
{
#if 0
        ifs >> setiosflags(std::ios::binary);
#endif

	// Read the number of rows_
	char *data= (char *) &rows_;
	for(int i = 0 ; i < sizeof (rows_) ; ++i) {
		ifs.get(data[i]);
	}

	// and the number of columns
	data = (char *) &cols_;
	for(int i = 0 ; i < sizeof (cols_) ; ++i) {
		ifs.get(data[i]);
	}

	// adjust the dimensions of the matrix accordingly
	m_.resize(rows_);

	typedef std::vector<ann::vector>::iterator I;
	// and read every element into the matrix
	for(I i = m_.begin() ; i != m_.end() ; ++i ) {
		i->resize(cols_);
		for(CVI j = i->begin() ; j != i->end() ; ++j )  {
			data= (char *) &(*j);
			for(int k = 0 ; k < sizeof (*j) ; ++k) {
				ifs.get(data[k]);
			}
		}
	}
	
}

// This method writes a matrix to a file 
void
matrix::save(std::ofstream &ofs) const
{
#if 0 
        ofs << setiosflags(ios::binary);
#endif
	
	// write out the number of rows
	char *data= (char *) &rows_;
	for(int i = 0 ; i < sizeof (rows_) ; ++i) {
		ofs.put(data[i]);
	}

	// write out the number of columns
	data = (char *) &cols_;
	for(int i = 0 ; i < sizeof (cols_) ; ++i) {
		ofs.put(data[i]);
	}

	typedef std::vector<ann::vector>::const_iterator CI;
	// and write out each element of the matrix
	for(CI i = m_.begin() ; i != m_.end() ; ++i ) {
		for(CVI j = i->begin() ; j != i->end() ; ++j )  {
			data= (char *) &(*j);
			for(int k = 0 ; k < sizeof (*j) ; ++k) {
				ofs.put(data[k]);
			}
		}
	}
	
}




bool
operator==(const matrix &m1, const matrix &m2) 
{
  assert(m1.columns() == m2.columns());
  assert(m1.rows() == m2.rows());


  for ( int i = 0 ; i < m1.rows() ; ++i ) { 
    for ( int j = 0 ; j < m1.columns() ; ++j ) { 
      if ( m1.get(i,j) != m2.get(i,j)) 
	return false;
    }
  }

  return true;

}



matrix
operator-(const matrix &m1, const matrix &m2) 
{
  assert(m1.columns() == m2.columns());
  assert(m1.rows() == m2.rows());

  matrix result(m1); // create a matrix of appropriate size

  typedef std::vector<ann::vector>::const_iterator CI;
  typedef std::vector<ann::vector>::iterator I;
  CI i1,i2;
  I ri;
  for(i1 = m1.m_.begin() , i2 = m2.m_.begin(), ri = result.m_.begin(); 
      i1 != m1.m_.end() ; ++i1, ++i2, ++ri ) {
    *ri = *i1 - *i2 ;
  }
  
  

  return result;
}


matrix
operator+(const matrix &m1, const matrix &m2) 
{
  matrix result(m1); 
  result += m2;
  return result;
}


matrix&
ann::matrix::operator+=(const matrix &m2) 
{
  assert(columns() == m2.columns());
  assert(rows() == m2.rows());

  typedef std::vector<ann::vector>::const_iterator CI;
  typedef std::vector<ann::vector>::iterator I;
  CI i2;
  I i1;
  for(i1 = m_.begin() , i2 = m2.m_.begin() ; 
      i1 != m_.end() ; ++i1, ++i2 ) {
     *i1 += *i2 ;
  }

  return *this;
}

matrix& 
operator*=(matrix &m, float f) 
{

  typedef std::vector<ann::vector>::iterator I;
  for(I i = m.m_.begin() ; i != m.m_.end() ; ++i) {
    *i *= f;
  }
  
  return m;
}


// Return the absolute maximum element of a matrix
//
float
matrix::max() const
{

  float maxval=0;
  typedef std::vector<ann::vector>::const_iterator CI;
  for(CI ci = m_.begin() ; ci != m_.end() ; ++ci) {
    maxval=std::max(maxval,ci->max());
  }
    
  return maxval;
}

// Make all rows have a modulus of unity
void
matrix::normaliseRows()
{
  typedef std::vector<ann::vector>::iterator I;
  for(I i = m_.begin() ; i != m_.end() ; ++i) {
    i->normalise();
  }
}


// Return true if the matrix is symetrical 
// 
bool
matrix::isSymetrical() const 
{
  assert ( isSquare());


  for ( int i = 0 ; i < cols_ ;  ++i ) { 
    for ( int j = i+1 ; j < cols_ ;  ++j ) { 

      if (  m_[i][j] != m_[j][i])
	return false;
      
    }
  }

  return true;

}


float 
dot(const ann::vector& v1, const ann::vector& v2)
{
  assert( v1.size() == v2.size() ) ;

  float sum=0.0;

  for ( int i = 0 ; i < v1.size() ; ++i ) 
    sum += v1[i] * v2[i] ;

  return sum;

}


} // end namespace ann

