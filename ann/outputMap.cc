/*

    Copyright (C) 2002, 2003  John Darrington 

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
const static char RCSID[]="$Id: outputMap.cc,v 1.5 2003/05/14 05:50:16 john Exp $";

#include "ann.h"
#include "outputMap.h"

#include "fm.h"

#include <math.h>

using namespace ann;
  
OutputMap::OutputMap(const FeatureMap &fm, bool bipolar, int outputMapSize)
{
  // Populate the output map with unique output patterns
  
  typedef FeatureMap::const_iterator CMI;


  if ( -1 == outputMapSize) {
    const int classes = fm.size();
    outputMapSize =  static_cast<int>(ceil( log(classes) / log(2.0) )) ;
  }
  

  int number=0;
  for ( CMI ci = fm.begin() ; ci != fm.end() ; ++ci ) { 
    ann::vector pattern(outputMapSize) ;
       

    for ( int bit = 0 ; bit < outputMapSize ; ++bit ) {
      pattern[bit]= (number>>bit)%2 ;
      if (bipolar && ( pattern[bit] == 0 ) ) 
	pattern[bit]=-1;
    }

    (*this)[pattern]=ci->first;
    reverseMap[ci->first]=pattern;

    ++number;
  }


  assert(size() == reverseMap.size());
}



// Look up in the `wrong' direction
// --- Get a vector, given a string
const ann::vector & 
OutputMap::reverseLookup(const std::string &s) const 
{

  return (reverseMap.find(s)->second ) ;
  
}

