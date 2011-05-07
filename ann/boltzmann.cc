/*

    Copyright (C) 2003  John Darrington 

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
const static char RCSID[]="$Id: boltzmann.cc,v 1.6 2003/05/14 05:50:16 john Exp $";

#include "outputMap.h"
#include "mesh.h"
#include "boltzmann.h"
#include "layer.h"
#include "log.h"

#include <cmath>
#include <stdexcept>

using namespace ann;


float
ThermalBiPolarStep::operator()(float x) const 
{
  float retval;

  const float p = static_cast<float>(rand()) / RAND_MAX;
  

  const float p_k = (temperature == 0.0) ? 1.0 : 1.0/(1.0 + exp(-fabs(x)/temperature));

  if ( x == 0.0 ) 
    retval=previousValue;
  else {
    if ( x > 0 ) 
      retval =  1.0;
    else
      retval = -1.0;


    // Change to a higher energy instead of a lower one if
    // the temperature is high
    if ( ! (p <  p_k )) { 
      retval = -retval ; 
    }

  }

  previousValue = retval;
  return retval;
}



CoolingFunction::CoolingFunction(float & temp, float r): temperature(temp),rate(r)  
{
  if ( rate > 1 || rate < 0 )
    throw std::range_error("Cooling rate should be positive and less than 1.0");
};
 
float 
CoolingFunction::operator()() const 
{ 
  temperature *= rate;
};
  



Boltzmann::Boltzmann(const ann::FeatureMap &fm,int hidden,
		     float temp, float coolingRate)
{
  trace;

  temperature = temp;

  inputUnits = fm.featureSize();
  outputUnits = static_cast<int>(ceil(::log(fm.size())/::log(2.0)));
  hiddenUnits = hidden;



  const int totalUnits = inputUnits + outputUnits +hiddenUnits;

  outputMap = new OutputMap(fm,true);

  activationFunction = new ThermalBiPolarStep(temperature);

  coolingFunction = new CoolingFunction(temperature,coolingRate);

  mesh = new Mesh(totalUnits,*activationFunction, false, coolingFunction);

  // Populate the weights with our patterns

  matrix &weights = mesh->getWeights();

  typedef FeatureMap::const_feature_iterator CFI;

  weights.randomiseSymetrical() ; 

  ann::log() << "Initialising weights\n" ;

  // Initialise the pattern bits
  for (int i = 0 ; i < inputUnits+outputUnits ; ++i ) { 
    for (int j = 0 ; j <=i ; ++j ) { 
      float value=0;
      for(CFI ci = fm.fbegin() ; ci != fm.fend() ; ++ci ) {
	const std::string klass((*ci).first);
	const ann::vector trainingPattern(outputMap->reverseLookup(klass));
	const ann::vector v((trainingPattern).juxtapose((*ci).second));
	value+=v[i]*v[j];
      }
      weights.set(i,j,value);
      weights.set(j,i,value);
    }
  }


  for (int i = 0 ; i < totalUnits ; ++i ) 
    weights.set(i,i,0);

  assert ( weights.isSymetrical());


 startTemperature = temperature;
 
  matrix matrix1; 

  do { 


    // Phase 1

    mesh->releaseClamps();

    for ( int i = 0 ; i < inputUnits+outputUnits ; ++i ) 
      mesh->clamp(i,(*mesh)[i]);

 
    matrix1 = mesh->getWeights();

    mesh->getSource() ;

    mesh->hebbianUpdate(0.1);

    // Phase 2 

    temperature = startTemperature;

    mesh->releaseClamps();

    for ( int i = 0 ; i < inputUnits ; ++i ) 
      mesh->clamp(i,(*mesh)[i]);


    mesh->getSource();

    mesh->hebbianUpdate(-0.1);
  

    temperature = startTemperature;

  }
  while ( matrix1 != mesh->getWeights() ); 


}



Boltzmann::~Boltzmann()
{
  delete mesh;
  delete activationFunction;
  delete outputMap;
}



std::string
Boltzmann::recall(const ann::ExtInput &ip) const
{
  trace;

  assert(mesh);

  temperature = startTemperature;

  mesh->releaseClamps();

  ann::vector dummy(hiddenUnits+outputUnits);
  for ( int i = 0 ; i < dummy.size() ; ++i ) 
    dummy[i]=0.0;
  
  
  const ann::vector e(dummy.juxtapose(ip) ); 

  *mesh = e;

  for ( int i = 0 ; i < inputUnits ; ++i ) 
    mesh->clamp(i,(*mesh)[i]);

  const ann::vector o(mesh->getSource());

  const ann::vector r(o.subvec(inputUnits,inputUnits+outputUnits -1 ));
  

  return (*outputMap)[r];

 
}

