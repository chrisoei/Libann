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
const static char RCSID[]="$Id: cmdline.cc,v 1.4 2003/05/14 05:50:16 john Exp $";


#include <string>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <iostream>
#include <assert.h>

#include <ann/log.h>

#include "cmdline.h"


using namespace std;

  
bool reading = false;
bool writing = false;

  
string comment="A multilayer perceptron";
string netfile="mlp.net";
float alpha = 0.2;
float tolerance = 0.5;
float k = 1.0;
int hiddenLayerSize = 0; // zero is sentinel for ipsize /2 ;
  
float momentum = 0.0;
int epochs=0;


void
usage()
{
  cerr << "Usage: " << "mlp "  ;
  cerr << "\
[-n] [-h] [-v] \
[-e epochs] \
[-k kvalue] \
[-a learning rate] \
[-m momentum] \
[-t tolerance] \
[-i hidden units] \
[-c comment] \
[-d file] \
[-{r|w} file]";
 cerr << endl;
}


void
parseCmdLine(int &argc, char **argv)
{
  

  char c;
  while ( -1 != (c = getopt(argc, argv, "k:a:t:nhvi:r::m:w::d::c:e:"))) {
    static bool readingP;
    static bool writingP;

    switch (c) {
    case 'e':
      writing = true;
      epochs = atoi(optarg);
      break;
    case 'k':
      writing = true;
      k = atof(optarg);
      break;
    case 'a':
      writing = true;
      alpha = atof(optarg);
      break;
    case 'd':
      {
      string logfile("mlp.log");
      if ( optarg) logfile=optarg;
      ann::setLogFile(logfile);
      break;
      }
    case 't':
      writing = true;
      tolerance = atof(optarg);
      break;
    case 'i':
      writing = true;
      hiddenLayerSize = atoi(optarg);
      if ( hiddenLayerSize < 1 ) { 
	cerr << "Invalid hidden layer size: " << optarg << endl;
	exit (1);
      }
      break;
    case 'n':
      writing = true;
      srand(0);  // useful for debugging
      break;
    case 'h':
      usage();
      exit(0);
      break;
    case 'w':
      writing = true;
      if ( optarg) netfile = optarg;
      break;
    case 'r':
      reading = true;
      if ( optarg) netfile = optarg;
      break;
    case 'm':
      writing = true;
      if ( optarg) momentum = atoi(optarg);
      break;
    case 'c':
      writing = true;
      comment = optarg;
      break;
    case 'v':
      break;
    default:
      usage();
      exit(1);
      break;
    }
    if ( reading && writing ){ 
      assert(readingP ^writingP);
      cerr << "Option " << c << " not valid in ";
      if ( writingP) 
	cerr << "training";
      else
	cerr << "recall";
      cerr << " mode\n";
      exit(1);
    }
    readingP = reading;
    writingP = writing;
  }

  if ( !writing && !reading ) 
    reading = true;

}

