#ifndef MESH_H
#define MESH_H
/*

    Copyright (C) 2002,2003  John Darrington 

    $Id: mesh.h,v 1.8 2003/05/14 05:50:16 john Exp $

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


#include "layer.h"



namespace ann { 

  class ExtInput;


  /*! \brief An arbitrary function, called by the mesh on each 
      update iteration.

      UpdateFunction is a pure virtual class, instances of which can 
      be passed to a Mesh.  The () operator for the UpdateFunction will
      be called on each iteration of the mesh.
  */

  class UpdateFunction { 
  public:
    /// The function to be called
    virtual float  operator()() const = 0;
  };


  /*! \brief A layer whose outputs are also its inputs

    A mesh is a `fully connected' network.  That is, every neuron has
    connections to every other neuron (except to itself).  Typically
    it's weight matrix is symetrical and with zero diagonal.
  */

  class Mesh : public Nlayer { 
  public:
    /// Construct a Mesh 
    /// \param dim The number of neurons in the mesh
    /// \param af  The activation function of the neurons
    /// \param synchronous True if the mesh uses synchronous updating
    /// \param updateFunc An arbitrary function called on each iteration of the mesh update
    Mesh(int dim ,const ActivationFunction &af, bool synchronous = true, const UpdateFunction *updateFunc  = 0);


    /// Return the output (which are also input) values of the mesh
    const ann::vector getSource() const ;


    /// Set the mesh values from a feature vector
    Mesh &
      operator=(const vector &ip);


    /// Access the value on an arbitrary neuron
    float & operator[](int e);


    /// Clamp an neuron to a particular value
    /// \param e The neuron to be clamped
    /// \param v The value to which it should be clamped
    void clamp(int e, float v);

    /// Release all clamps from the mesh
    void releaseClamps() ;
     

    /// Update the weight matrix with Hebbian rule
    /// \param increment The value by which each element will be updated 
    /// (may be negative).
    void hebbianUpdate(float increment) const;

    /// Destroy the Mesh
    ~Mesh();


  private:
    ExtInput *theUnits;
    std::vector<bool> *clampedNeuron;
    ann::vector *clamps;

    bool synchronous;
    const UpdateFunction *updateFunction;

    const ann::vector getSourceSynchronous() const ;
    const ann::vector getSourceAsynchronous() const ;


  private:
    Mesh &
      operator=(const Mesh &);

    
    Mesh(const Mesh &);

    

  };



};

#endif /* MESH_H */




