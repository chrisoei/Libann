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
const static char RCSID[]="$Id: hopfield-char.cc,v 1.2 2002/07/22 05:54:43 john Exp $";

// This program demonstrates simple use of a Hopfield network.
// It takes one argument, which is the name of a directory containing
// a set of glyphs.  The program trains a network to these patterns,
// and then presents the network with a noisy copy of each pattern.
// Each output is then saved to a file with the same name as the input
// pattern.

#include <ann/ann.h>
#include <ann/hopfield.h>

#include <ann/mlp.h>

#include <iostream>
#include <set>
#include <fstream>

using namespace std;

using namespace ann;

#include <direntry.h>
#include <glyph.h>


// A function to insert random noise into a Glyph
void 
mutate(Glyph &g) ;


int 
main(int argc, char **argv)
{
  try {

    if ( argc <= 1 ) { 
      cerr << "Usage: hopfield-char <directory-name>" << endl;
      exit (1);
    }


    set<ExtInput> inset;

    typedef set<string>::const_iterator CI;


    // Create a training set
    //
    set<string> filenames = getDirEntries(argv[1],"^[1-4]\\.char$");
    
    for ( CI ci = filenames.begin() ; ci != filenames.end() ; ++ci ) { 
      Glyph g(*ci, true);
      inset.insert(g);
    }
    


   // Instantiate a hopfield net trained with our patterns
   Hopfield h(inset);

   // Recall all the data after mutating them a bit
   {
     set<string> recallNames = getDirEntries(argv[1],"^[1-4]\\.char$");
    for ( CI ci = recallNames.begin() ; ci != recallNames.end() ; ++ci ) { 
     Glyph g(*ci,true);
    
     mutate(g);
     cout << "Recalling" << *ci << endl;
     const string filename(ci->substr(ci->rfind("/")+1,string::npos));

     ann::vector result = h.recall(g);
     Glyph *g1 = static_cast<Glyph *>(&result);

     ofstream ofs(filename.c_str());
     g1->print(ofs);
     ofs.close();
    }
   } 

   return 0;
  }
  catch (const exception &e) {
    cerr << "Caught an exception " << e.what() << endl;
  }
  catch (...) { 
    cerr << "Unknown exception\n";
  }

  return 1 ;
}






// A function to insert random noise into a Glyph
void 
mutate(Glyph &g) 
{
  for (int i = 0 ; i < g.size() ; ++i ) {
    if ( rand() < RAND_MAX*1.0/10.0 ) { 
      g[i] = -g[i];
    }
  }
}
