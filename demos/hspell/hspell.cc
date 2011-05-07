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
const static char RCSID[]="$Id: hspell.cc,v 1.3 2003/05/14 05:50:16 john Exp $";

#include <stdexcept>
#include <iostream>
#include <fstream>
#include "word.h"
#include <set>
#include <ann/hopfield.h>
#include <ann/log.h>

using namespace std;

void
usage()
{
      cout << "Usage: hspell -d dict word\n";
      exit(1);
}

int
main(int argc, char **argv)
{
  try { 
    if ( argc < 4 ) usage();
    if ( string(argv[1]) != "-d" ) usage();


    int length=0;
    { 
    ifstream dictionary(argv[2]);

    if (!dictionary.is_open() ) { 
      cerr << "Cannot open " + string(argv[2]) << endl;
      return 1;
    }


    // Find the lenght of the longest word in the dictionary
    while(!dictionary.fail()) { 
      string word;
      dictionary >> word;
      
      length= (length>word.length())?length:word.length();
    }
    dictionary.close();
    }

    set<ann::ExtInput> dictionarySet;

    ifstream dictionary(argv[2]);
    if (!dictionary.is_open() ) { 
      cerr << "Cannot open " + string(argv[2]) << endl;
      return 1;
    }

    while(!dictionary.eof()) { 
      string word;
      dictionary >> word;

      if ( word != "" ) { 
	Word w(word,length*5);
	dictionarySet.insert(w);
      }
    }
    dictionary.close();


    ann::Hopfield network(dictionarySet);

    Word x(argv[3],length*5);


    ann::vector result(network.recall(x));

    Word r(result);

    cout << r.toString() << endl;

  }
  catch (const exception &e) { 
    cerr << "Unhandled exception: " << e.what() << endl;
    return 1;
  }
  catch (...) { 
    cerr << "Unexpected exception\n";
    return 1;
  }
  return 0;
}
