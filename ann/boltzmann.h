#ifndef BOLTZMANN_H
#define BOLTZMANN_H
/*

    Copyright (C) 2003  John Darrington 

    $Id: boltzmann.h,v 1.5 2003/05/14 05:50:16 john Exp $

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

#include <ann/hopfield.h>
#include <ann/layer.h>
#include <ann/mesh.h>
#include <ann/fm.h>

namespace ann {

  class OutputMap;

  class CoolingFunction;


  // This activation function works as follows:
  // +1 if x > 0 AND p < 1/(1 + e^(-x/temperature))
  // -1 if x < 0 AND p < 1/(1 + e^(-x/temperature))
  // otherwise same as previous value.
  //
  class ThermalBiPolarStep : public ActivationFunction { 
  public:
    ThermalBiPolarStep(float &temp):temperature(temp){};

    float operator()(float x) const ;

    void save(std::ostream &os) const {};
    void load(std::istream &os)  {};
  private:
    mutable float previousValue;
    float &temperature;
  };


/*! A function to reduce the `temperature'

  This function reduces temperature in accordance with Newton's 
  law of cooling
 */
class CoolingFunction : public UpdateFunction { 
public:
  CoolingFunction(float & temp, float r);

  float operator()() const ;
  
private:
  float &temperature;
  float rate;

};





  // A Boltzmann Machine class

  class Boltzmann { 

  public:
    Boltzmann(const ann::FeatureMap &fm,  int hiddenUnits, 
	      float temperature, float coolingRate);
 
    ~Boltzmann();
    std::string recall(const ann::ExtInput &ip) const ;


  private:
    Boltzmann(const Boltzmann &);
    Boltzmann &operator=(const Boltzmann &);

  private:
    Mesh *mesh;
    ActivationFunction *activationFunction;
    CoolingFunction *coolingFunction;
    OutputMap *outputMap;
    int outputUnits;
    int inputUnits;
    int hiddenUnits;


    float startTemperature;
    mutable float temperature;
  };





}

#endif /* BOLTZMANN_H */


