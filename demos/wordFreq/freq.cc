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
const static char RCSID[]="$Id: freq.cc,v 1.4 2002/07/11 01:27:57 john Exp $";

#include <iostream>

#include "wordVec.h"

#include "direntry.h"

#include "freqCount.h"

#include <map>

#include <ann/ann.h>
#include <ann/kohonen.h>

using namespace std;


int 
main(int argc, char **argv)
{
  if ( argc <= 1 ) { 
    cerr << "Usage: freq directory-name\n";
    return 1;
  }

  
  try {

    const int vectorSize=3;

    // Create a word vector of the  most common words from the files
    // in the directory 
    cout << "Creating word vector" ;
    cout.flush();
    WordVec wv(argv[1],vectorSize);
    cout << endl;

    cout << "The most common words are: " ;
    for (int i = 0 ; i < wv.size() ; ++i  ) { 
      cout << wv[i] ;
      if ( i < wv.size() -1 ) 
	cout << ", ";
    }
    cout << endl;

    set<string> files=getDirEntries(argv[1]);

    // Create Frequency counts for all the files
    //
    set<ann::ExtInput> trainingData ; 
    map<string,FreqCount> recallMap;

    typedef set<string>::const_iterator CI;
    for (CI ci = files.begin(); ci != files.end() ; ci++) {

      // Skip files not ending in .txt
      if ( ".txt" != ci->substr(ci->length() -4, string::npos) )
	continue;

      FreqCount fc(*ci,wv);

      cout << "Word vec for " << *ci << " is " << fc << endl;
      
      trainingData.insert(fc);

      recallMap.insert(make_pair(*ci,fc));
    }
    

    ann::Kohonen net(vectorSize,7);
    
    cout << "Training the network";
    cout.flush();
    net.train(trainingData,0.3,0.8,0.1,0.40);
    cout << endl;

    typedef map<string,FreqCount>::const_iterator CMI;
    for (CMI ci = recallMap.begin(); ci != recallMap.end() ; ci++) {

      ann::vector output(net.recall(ci->second));
      cout << "Recall of " << ci->first << " is " << output << endl;
      string className ;
      for ( int i = 0 ; i < output.size() ; ++i  ) {
	if ( output[i]==0.0 ) 
	  className+="0";
	else
	  className+="1";
      }

      system(("mkdir -p " + className).c_str());
      system(("cp " + ci->first + " " + className).c_str());

    }


  }
  catch (const exception &e) {
    cerr << "Unhandled exception: " << e.what() << endl;
    return 1;
  }
  catch (...) {
    cerr << "Unexpected exception" << endl;
    return 1;
  }



  return 0;
}




