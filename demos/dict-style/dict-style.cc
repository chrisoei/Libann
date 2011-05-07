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
const static char RCSID[]="$Id: dict-style.cc,v 1.3 2003/05/14 13:47:24 john Exp $";

#include <stdio.h>
#include <iostream>
#include <set>
#include <ann/ann.h>
#include <ann/fm.h>
#include <ann/mlp.h>

#include <fstream>
#include <sstream>

using namespace std;


class DictStyle : public ann::ExtInput {
public:
  DictStyle::DictStyle(string buf) ;

};


int
network_write(int argc, char **argv);

int
network_read(int argc, char **argv);







int
main(int argc, char **argv)
{

  try {


   // We expect directoryname to hold a bunch of text files 
   if ( argc < 4 ) {
     cerr << "Usage: author -{r|w} <netfile> <textfile1> <textfile2> ... <textfile3> " << endl;
     exit(1);
   }

   srand(time(0));


   typedef int (* network_action)(int, char **) ;

   network_action na = ( 0 == strcmp(argv[1],"-w") ) ? network_write: network_read;


   return na(argc, argv);

  }

  catch (const std::exception &e){
   cerr << "Unexpected exception: " << e.what() << endl;
  }
  catch (...) {
   cerr << "Unhandled exception\n";
  }

  return 1;



}





DictStyle::DictStyle(string buf) : ExtInput(0)
{

  char b[256];

  std::ostringstream ostr ; 
  ostr << "style " <<  buf ; 
  ostr << "| head -10 | tail -9 | awk -F':' '{print $2}' | awk -F= '{printf $1 \" \"}'" ;


  FILE *pipe = popen(ostr.str().c_str(), "r");

  assert(pipe);

  fgets(b,256,pipe);

  string buffer(b);

  // Delete whitespace at RHS
  buffer.replace(buffer.rfind(" "),1,"");


  istringstream str(buffer);


  for(int i = 0 ; i < 9 ; ++i ) {
    float feature;
    str >> feature ; 
    if ( str.bad() ) break ;

    resize(i+1);
    (*this)[i]=feature;
  }

}



int
network_write(int argc, char **argv)
{

  const string netfile(argv[2]);

  // Populate the feature map from the files
   ann::FeatureMap fm;

 

   for ( int i = 3; i < argc ; ++i ) { 

     const string pathname(argv[i]);


     // FIXME: This is not a reliable way of getting the filename from
     // a pathname.  It'll fail if the filename happens to have a / in
     // it. 
     const string filename(pathname.substr(pathname.rfind("/")+1,
					   string::npos));

     // The classname is the first letter of the filename
     const string className(filename.substr(0,filename.find_first_of("0123456789")));

     DictStyle ds(pathname);

     fm.addFeature(className,ds);

   }


   ann::Mlp classifier(9,2,0.4,6,0.45,1,3);


   cout << "Training the  classifier\n";

   classifier.train(fm);

   cout  << "Writing to " << netfile << endl;



   ofstream ofs(netfile.c_str());
   assert(ofs.is_open());
   classifier.save(ofs,"styles");
   ofs.close();

   return 0;
}



int
network_read(int argc, char **argv)
{

  const string netfile(argv[2]);

  // load classifier from file

  ifstream ifs(netfile.c_str());
  assert(ifs.is_open());

  ann::Mlp classifier(ifs);
  ifs.close();
		      


  for ( int i = 3; i < argc ; ++i ) { 

   const string pathname(argv[i]);

   DictStyle ds(pathname);

   cout << classifier.recall(ds) << endl;
  }
   

  return 0;


}
