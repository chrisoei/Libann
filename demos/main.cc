/*
    Copyright (C) 2000,2003  John Darrington

    This program is free software; you can redistribute it and/or modify
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
#include <ann/ann.h>
#include <math.h>
#include <ann/fm.h>
#include <ann/mlp.h>

#include <ann/log.h>
#include "cmdline.h"

#include <signal.h>
#include <iostream>
#include <fstream>

int yyparse();

using namespace std;
using namespace ann;

FeatureMap featureMap;

typedef FeatureMap::const_iterator CI;

double 
log2(double x)
{
  return log10(x)/log10(2.0);
}

ann::Mlp *theNet=0;

void 
showStatus(int s) /*, siginfo_t *sinf, void *ucon) */
{
  cout << "Epochs so far: " << theNet->epochs() << endl;
  exit(0);
}


int
main(int argc, char **argv)
{

  srand(time(0));

  parseCmdLine(argc,argv);

  if ( 0 != yyparse() ) 
    return 1;



  int classes = featureMap.size();

  // Find out the size of the first feature 
  // (hopefully they're all the same size! ) 
  CI ci=featureMap.begin();
  const int inputSize = (ci->second).begin()->size();



  /* Compute number of output units needed */
  double x = log2(classes);
  /* round up if non integer */
  int outputSize = ( (x - (int) x ) > 0 ) ? (int) x + 1 : (int) x ;

  if ( writing ) {

  /* now create the neural network */
  ann::Mlp network(inputSize,outputSize,
		   alpha,momentum,tolerance,
		   k,hiddenLayerSize);


  /* Train the network */
  cout << "Training the network\n";

  cout << "FeatureMap size is " << featureMap.size() << endl;

  struct sigaction  sigact;
  sigact.sa_sigaction=0;
  sigact.sa_handler=showStatus;
  sigact.sa_flags=0;
  sigemptyset(&sigact.sa_mask);

  theNet = &network;

  if ( -1 == sigaction(SIGINT, &sigact, 0) ) {
    cerr << "Cannot set signal handler\n";
    exit(1);
  }

  if ( network.train(featureMap, epochs) ) { 
    cout << "Done" << endl;
  }
  else    {
    return 1;
  }
      
  ofstream ofs(netfile.c_str());
  if ( ! ofs.is_open() ) {
    cerr << "Cannot open trained.net\n";
    exit (1);
  }
  network.save(ofs,"Trained Neural net");
  ofs.close();
  }
  else if ( reading )  { 
    cout << "Reading  from " << netfile << " . .  . " << endl;
    ifstream ifs(netfile.c_str());
    if ( ! ifs.is_open() ) {
      cerr << "Cannot open " << netfile << endl;
      exit(1);
    }
    ann::Mlp network(ifs);
    ifs.close();

    cout << network << endl;

    typedef std::map<ann::vector,std::string>::const_iterator CI2;

    // Iterate through the feature map and recall every feature there
    // 
    for (CI ci=featureMap.begin() ; 
       ci != featureMap.end() ; 
       ++ci) {

       const set<ExtInput> featureSet(ci->second);

       if ( featureSet.size() != 1 ) { 
	 cerr << "Feature Set has size  " << featureSet.size() << endl;
	 cerr << "Features for recall must be single features\n";
	 exit(1);
       }

       const ExtInput feature(*featureSet.begin());

       if ( feature.size() != network.inputs() ) { 
	 cerr << "Vector size does not match network input size\n";
	 exit(1);
       }

       cout << "Recalling " << ci->first << endl;

       cout << "This belongs to class \"" << network.recall(feature) << "\"\n";

    }


  }

  return 0;
}


