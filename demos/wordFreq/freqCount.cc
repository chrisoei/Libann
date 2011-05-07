/*

    Copyright (C) 2002  John Darrington 

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
const static char RCSID[]="$Id: freqCount.cc,v 1.4 2003/05/14 05:50:16 john Exp $";

#include "freqCount.h"
#include "wordVec.h"

#include <fstream>
#include <stdexcept>

using namespace ann;
using namespace std;

// Construct a frequency count from a file
FreqCount::FreqCount(const string &filename, const WordVec &w):
  ExtInput(w.size()) {
  
  ifstream ifs(filename.c_str());
  if (!ifs) 
    throw runtime_error("Cannot open " + filename );


  int wordCount = 0;
  while( ! ifs.fail() ) { 
    string word;
    ifs >> word;

    ++wordCount;

    // Filter words here
    filter(word);

    if ( word.length() == 0 ) 
      continue;


    // Iterate through the WordVec.
    // When a match is found increment it's count
    // If WordVec is very large, then it might be worthwhile creating a 
    // map rather than iterating . . . 
    int i ;
    for ( i = 0; i < w.size() ; ++i ) {
      if ( w[i] == word) {
	(*this)[i]++;
	break;
      }
    }



  }

  // Normalise to the size of the text
  (*this)/=(float)wordCount;
  
  ifs.close();
}
