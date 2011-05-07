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
const static char RCSID[]="$Id: fm.cc,v 1.6 2003/05/14 05:50:16 john Exp $";

#include "ann/fm.h"

using namespace ann;

FeatureMap::const_feature_iterator::const_feature_iterator(
  const std::map<std::string,std::set<ann::ExtInput> >::const_iterator &mIterator, 
  const std::set<ann::ExtInput>::const_iterator &sIterator,
  const std::map<std::string,std::set<ann::ExtInput> >::const_iterator &mEnd):
mapIterator(mIterator),setIterator(sIterator),mapEnd(mEnd)
{
  setOffset = 0;
}



FeatureMap::const_feature_iterator 
FeatureMap::fbegin() const 
{
  typedef std::map<std::string,std::set<ann::ExtInput> >::const_iterator CI;


  CI i = begin();
  
  const std::set<ann::ExtInput> &firstSet =  i->second ; 
  
  return const_feature_iterator(i,firstSet.begin(),this->end());
 
}

FeatureMap::const_feature_iterator 
FeatureMap::fend() const 
{
  typedef std::map<std::string,std::set<ann::ExtInput> >::const_iterator CI;


  CI i = end();
  
  const std::set<ann::ExtInput> &lastSet =  i->second ; 
  
  return const_feature_iterator(i,lastSet.end(), this->end());

}


/* Returning the address of a static variable is too dangerous for use I think 
   Let's wait untill there's a better implementation

std::pair<std::string,ann::ExtInput>* 
FeatureMap::const_feature_iterator::operator->() const 
{

  // FIXME:  Gets overwritten by subsequent calls OR NOT (constructor called on already constructed object ??
  static std::pair<std::string,ExtInput> retval(mapIterator->first,*setIterator);
  
  return &retval;
  
}

*/      

std::pair<std::string,ExtInput>
FeatureMap::const_feature_iterator::operator*() const
{

  std::pair<std::string,ExtInput> retval(mapIterator->first,*setIterator);
  
  return retval;
}



// PreIncrement Operator
FeatureMap::const_feature_iterator &
FeatureMap::const_feature_iterator::operator++()  
{
  ++setIterator ;
  ++setOffset ; 
  if ( setOffset == (mapIterator->second).size()  ) {  
    ++mapIterator ; 
    if ( mapIterator != mapEnd ) { 
      const std::set<ann::ExtInput> &nextSet =  mapIterator->second ; 
      setIterator = nextSet.begin();
    }
    setOffset = 0;
  }
  return *this ; 
}



// Post Increment operator
FeatureMap::const_feature_iterator
FeatureMap::const_feature_iterator::operator++(int )
{
  FeatureMap::const_feature_iterator oldOne(*this);
  ++(*this);

  return oldOne;
}


bool
FeatureMap::const_feature_iterator::operator==(
const FeatureMap::const_feature_iterator &cfi
) const 
{

  if ( setOffset != cfi.setOffset )     return false;
  if ( mapIterator != cfi.mapIterator ) return false;
  if ( mapEnd != cfi.mapEnd ) return false;

  return true;
}

    
bool 
FeatureMap::isBalanced() const
{
  int lastSize=-1;

  typedef FeatureMap::const_iterator CI;

  for ( CI ci = begin() ; ci != end() ; ++ci ) { 

    if ( lastSize != -1 && lastSize != (ci->second).size()) 
      return false;

    lastSize = ci->second.size();
  }


  return true;
}





