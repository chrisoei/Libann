#ifndef FM_H
#define FM_H
/*

    Copyright (C) 2002,2003  John Darrington 

    $Id: fm.h,v 1.9 2003/05/14 05:50:16 john Exp $

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

#include <map>
#include <set>
#include <string>
#include <iostream>
#include "ann.h"


namespace ann {


  // A map of features, indexed by their respective class names
  class FeatureMap : public std::map<std::string,std::set<ann::ExtInput> > {

  public:
    // An iterator to a feature map
    // This iterates through each feature within the map
    class const_feature_iterator { 
      friend class FeatureMap;
    public:
      const_feature_iterator & operator++();
      const_feature_iterator operator++(int );

      std::pair<std::string,ann::ExtInput> operator*() const ; 
      std::pair<std::string,ann::ExtInput>* operator->() const ; 


      bool operator==(const const_feature_iterator &cfi) const ; 
      inline bool operator!=(const const_feature_iterator &cfi) const 
      {
	return ( ! operator==(cfi));
      }
    private:
      const_feature_iterator(

      const std::map<std::string,std::set<ann::ExtInput> >::const_iterator &mIterator, 
      const std::set<ann::ExtInput>::const_iterator &sIterator,
      const std::map<std::string,std::set<ann::ExtInput> >::const_iterator &mEnd 
);      

      std::map<std::string,std::set<ExtInput> >::const_iterator mapIterator ;
      std::set<ExtInput>::const_iterator setIterator ; 

      std::map<std::string,std::set<ExtInput> >::const_iterator mapEnd ;

      int setOffset;

    }; // end const_feature_iterator
    


    friend
      std::ostream & operator<<(std::ostream &os, const FeatureMap &fm);
    
  public:

    FeatureMap():vecSize(-1){};

    // add a new feature to the map
    // The size of the feature must be identical to all features previously 
    // added 
    void 
      addFeature(const std::string &className, const ann::ExtInput &feature) ;
    
    // return the size of the feature vectors in the map
    // (all vectors have the same size)
    int
      featureSize() const 
      { return vecSize ; }


    // return true if the map is balanced
    // (ie contains the same number of features per class)
    bool isBalanced() const ;


    const_feature_iterator fbegin() const ; 
    const_feature_iterator fend() const ; 

  private:
    int vecSize;
  } ;


};


#endif /* FM_H */
