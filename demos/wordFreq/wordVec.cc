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
const static char RCSID[]="$Id: wordVec.cc,v 1.6 2003/05/14 05:50:16 john Exp $";

#include <string>
#include <iostream>
#include <stdexcept>
#include <errno.h>
#include <set>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

#include "wordVec.h"

#include "direntry.h"

typedef std::pair<std::string,int> WordCount;


using namespace std;

class CompareCount { 
public:
  int operator()(const WordCount &wc1, const WordCount &wc2) const 
  {
    return ( wc1.second < wc2.second);
  }
};



WordVec::WordVec(const string &dirname, int words):vector<string>(words)
{

  map<string,int> wordmap;
  set<string> files=getDirEntries(dirname);

  typedef set<string>::const_iterator CI;
  for ( CI ci = files.begin(); ci != files.end() ; ++ci ) {
    
    // Skip files not ending in .txt
    if ( ".txt" != ci->substr(ci->length() -4, string::npos) ) 
      continue;

    cout << "Using file " << *ci << endl;

    
    ifstream ifs(ci->c_str());
    if (!ifs) { 
      throw runtime_error("Cannot open " + *ci);
    }

    while (!ifs.fail()) {
      string word;
      ifs >> word;
      if ( word.length() == 0 ) 
	continue;


      // Filter out certain nasties
      filter(word);

      if ( word.length() == 0 ) 
	continue;

      // Increment this count
      wordmap[word]++;


    }
    ifs.close();


    typedef map<string,int>::const_iterator CMI;



    vector<WordCount> wcv(wordmap.size());

    int i = 0; 
    for(CMI ci = wordmap.begin() ; ci != wordmap.end() ; ++ci ) {
      wcv[i++]=*ci;
    }


    // Sort them into ascending order
    sort(wcv.begin(),wcv.end(),CompareCount());

    typedef vector<WordCount>::reverse_iterator CVI;

    // Copy the first words into this vector
    i=0;
    for(CVI ci = wcv.rbegin() ; ci != wcv.rend() && i < words ; ++ci, ++i ) {
      (*this)[i]=ci->first;
    }
      
  }
}




void
filter(string &word)
{

  if ( word.length() == 0 ) 
    return;


  // remove punctiation at start and end of words
       		     
  // deal with word ends
  size_t x=word.length()-1;

  char lastchar = word[x];

  while ( ispunct(lastchar) ) {

    --x;
    if ( x == 0 ) 
      break;

    lastchar = word[x];

  }



  word.erase(x+1,string::npos);


  if ( word.length() == 0 ) 
    return;


  // deal with word starts
  x=0;
  char firstchar = word[0];
  while ( ispunct(firstchar) ){ 
    ++x;
    if ( x >= word.length()) 
      break;

    firstchar = word[x];

  }

  word.erase(0,x);


  if ( word.length() == 0 ) 
    return;


  // Fold all strings to lower case
  for (int i = 0 ; i < word.length() ; ++i ) 
    word[i]=tolower(word[i]);


}









