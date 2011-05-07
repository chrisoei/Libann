/*

    Copyright (C) 2002,2003  John Darrington 

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
const static char RCSID[]="$Id: boltzmann-char.cc,v 1.3 2003/05/14 05:50:16 john Exp $";


// This program demonstrates the use of the ann::Boltzmann class
// It uses the Glyph class (a specialisation of the ann::ExtInput
// class) to train a Boltzmann machine.  It then uses the
// trained network to recall some glyphs.

#include <ann/ann.h>
#include <ann/fm.h>
#include <ann/boltzmann.h>

#include <string>
#include "glyph.h"

#include "direntry.h"

#include <stdexcept>
#include <iostream>


using namespace std;


int
main(int argc, char **argv)
{

  try {

   // We expect directoryname to hold a bunch of glyph files which
   // should all end in .char
   if ( argc < 2 ) {
     cerr << "Usage: boltzmann-char <directoryname>" << endl;
     exit(1);
   }

   srand(time(0));
   
   // Get the names of all the files in the directory
   set<string> de = getDirEntries(argv[1]);

   typedef set<string>::const_reverse_iterator CRI;
   typedef set<string>::const_iterator CI;


   // Populate the feature map from the files
   ann::FeatureMap fm;

   for (CRI ci = de.rbegin() ; ci != de.rend() ; ++ci ) {
     const string pathname(*ci);


     // FIXME: This is not a reliable way of getting the filename from
     // a pathname.  It'll fail if the filename happens to have a / in
     // it. 
     const string filename(pathname.substr(pathname.rfind("/")+1,
					   string::npos));

     // Ignore any files not ending in `.char'
     int len = filename.length();
     if ( ".char" != filename.substr(len -5, len) ) 
       continue;

     // Save these ones for recall purposes
     if ( filename.find_first_of("23") != std::string::npos)
       continue;

     // The classname is the first letter of the filename
     const string className(filename.substr(0,1));

     // Create the glyph and add it to the map
     const Glyph g(pathname,true);

     fm.addFeature(className,g);

   }


   // Set up the network and train it.

   const int outputSize=3;
   const int inputSize=fm.featureSize();

   ann::Boltzmann net(fm,10,10,0.9);


   // Recall  all the glyphs
   for (CI ci = de.begin() ; ci != de.end() ; ++ci ) {
     const string pathname(*ci);

     // Ignore any files not ending in `.char'
     int len = pathname.length();
     if ( ".char" != pathname.substr(len -5, len) ) 
       continue;

     const Glyph g(pathname);

     cout << pathname << " has class " << net.recall(g) << endl;

   }

   return 0;

  }
  catch (const std::exception &e){
   cerr << "Unexpected exception: " << e.what() << endl;
  }
  catch (...) {
   cerr << "Unhandled exception\n";
  }

  return 1;
}


