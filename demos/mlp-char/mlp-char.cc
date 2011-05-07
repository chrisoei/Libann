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
const static char RCSID[]="$Id: mlp-char.cc,v 1.6 2003/05/14 05:50:16 john Exp $";


// This program demonstrates the use of the ann::Mlp class
// It uses the Glyph class (a specialisation of the ann::ExtInput
// class) to train a multi-layer perceptron network.  It then uses the
// trained network to recall some glyphs.

#include <ann/ann.h>
#include <ann/fm.h>
#include <ann/mlp.h>

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
     cerr << "Usage: mlp-char <directoryname>" << endl;
     exit(1);
   }

   srand(time(0));
   
   // Get the names of all the files in the directory
   set<string> de = getDirEntries(argv[1]);

   typedef set<string>::const_iterator CI;


   // Populate the feature map from the files
   ann::FeatureMap fm;

   for (CI ci = de.begin() ; ci != de.end() ; ++ci ) {
     const string pathname(*ci);


cout << "Pathname is " << pathname << endl;

     // FIXME: This is not a reliable way of getting the filename from
     // a pathname.  It'll fail if the filename happens to have a / in
     // it. 
     const string filename(pathname.substr(pathname.rfind("/")+1,
					   string::npos));
cout << "Filename is " << filename << endl;

     // Ignore any files not ending in `.char'
     int len = filename.length();
     if ( filename.length() < 5 || ".char" != filename.substr(len -5, len) ) 
       continue;


     // Reserve files with a 6 in them for recall
     // Don't train with them
     if ( "6.char" == filename.substr(len -6, len) ) 
       continue;

     // The classname is the first letter of the filename
     const string className(filename.substr(0,1));

cout << "Classname is " << className << endl;

     // Create the glyph and add it to the map
     const Glyph g(pathname);
     fm.addFeature(className,g);

   }



   // Set up the network and train it.

   const int outputSize=3;
   const int inputSize=fm.featureSize();

   ann::Mlp net(inputSize,outputSize,1,0);

   net.train(fm);



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

