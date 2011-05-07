#!/bin/sh
# Copyright 2003 John Darrington
# A script to test the Boltzmann Machine

cleanup ()
{
    rm -rf $TEMPDIR
    rm -f a.out
    true
}

fail ()
{
    cleanup
    exit 1
}

no_result ()
{
    cleanup
    exit 2
}

pass ()
{
    cleanup
    exit 0
}

include_path='-I '`echo $search_path | sed -e 's/:/ -I /g'`
if [ $? -ne 0 ] ; then no_result ; fi


here=`pwd`
if [ $? -ne 0 ] ; then no_result ; fi

TEMPDIR=/tmp/$$
mkdir -p $TEMPDIR
if [ $? -ne 0 ] ; then no_result ; fi

cd $TEMPDIR
if [ $? -ne 0 ] ; then no_result ; fi


cat > $TEMPDIR/test.cc <<EOF
#line $[$LINENO+2]  "$0"

#include <iostream>
#include <ann/boltzmann.h>
#include <ann/fm.h>

using namespace std;
using namespace ann;

int
main()
{

      try { 

      // Pattern Dimension
      const int pDim=10;

      ann::ExtInput vA1(pDim);
      ann::ExtInput vA2(pDim);
      ann::ExtInput vA3(pDim);

      // Class A --- contains primarily -1's --- just a few +1s here and there

      vA1[0]=-1;vA1[1]=-1;vA1[2]=-1;vA1[3]=-1;vA1[4]=-1;
      vA1[5]=-1;vA1[6]=-1;vA1[7]=-1;vA1[8]=-1;vA1[9]=-1;

      vA2[0]=-1;vA2[1]=-1;vA2[2]=-1;vA2[3]=-1;vA2[4]=-1;
      vA2[5]=-1;vA2[6]=-1;vA2[7]=+1;vA2[8]=-1;vA2[9]=-1;

      vA3[0]=-1;vA3[1]=+1;vA3[2]=-1;vA3[3]=-1;vA3[4]=-1;
      vA3[5]=-1;vA3[6]=-1;vA3[7]=-1;vA3[8]=+1;vA3[9]=-1;


      // Class B --- +1's in the middle

      ann::ExtInput vB1(pDim);
      ann::ExtInput vB2(pDim);
      ann::ExtInput vB3(pDim);

      vB1[0]=-1;vB1[1]=-1;vB1[2]=-1;vB1[3]=-1;vB1[4]=+1;
      vB1[5]=+1;vB1[6]=-1;vB1[7]=-1;vB1[8]=-1;vB1[9]=-1;

      vB2[0]=-1;vB2[1]=-1;vB2[2]=-1;vB2[3]=-1;vB2[4]=+1;
      vB2[5]=-1;vB2[6]=-1;vB2[7]=-1;vB2[8]=-1;vB2[9]=-1;

      vB3[0]=-1;vB3[1]=-1;vB3[2]=-1;vB3[3]=-1;vB3[4]=+1;
      vB3[5]=+1;vB3[6]=+1;vB3[7]=-1;vB3[8]=-1;vB3[9]=-1;

      // Class C --- +1's around the end

      ann::ExtInput vC1(pDim);
      ann::ExtInput vC2(pDim);
      ann::ExtInput vC3(pDim);

      vC1[0]=-1;vC1[1]=-1;vC1[2]=-1;vC1[3]=-1;vC1[4]=-1;
      vC1[5]=-1;vC1[6]=-1;vC1[7]=-1;vC1[8]=-1;vC1[9]=+1;

      vC2[0]=-1;vC2[1]=-1;vC2[2]=-1;vC2[3]=-1;vC2[4]=-1;
      vC2[5]=-1;vC2[6]=-1;vC2[7]=+1;vC2[8]=-1;vC2[9]=+1;

      vC3[0]=-1;vC3[1]=-1;vC3[2]=-1;vC3[3]=-1;vC3[4]=-1;
      vC3[5]=-1;vC3[6]=-1;vC3[7]=+1;vC3[8]=+1;vC3[9]=+1;


      FeatureMap fm;

      set<ExtInput> examples ; 


      fm.addFeature("a",vA1);
      fm.addFeature("a",vA2);
      fm.addFeature("a",vA3);

      fm.addFeature("b",vB1);
      fm.addFeature("b",vB2);
      fm.addFeature("b",vB3);

      fm.addFeature("c",vC1);
      fm.addFeature("c",vC2);
      fm.addFeature("c",vC3);


      int hiddenUnits = 1;
      float temperature = 10;
      float coolingRate = 0.7;

      Boltzmann b(fm,hiddenUnits,temperature, coolingRate);


      ann::ExtInput vr(pDim); 

      // Test class C

      vr[0]=-1;vr[1]=-1;vr[2]=-1;vr[3]=-1;vr[4]=-1;
      vr[5]=-1;vr[6]=-1;vr[7]=+1;vr[8]=+1;vr[9]=+1;


      {
	string klass = b.recall(vr);

	assert( klass == "c");
      }




      // Test Class A

      vr[0]=-1;vr[1]=-1;vr[2]=-1;vr[3]=-1;vr[4]=-1;
      vr[5]=-1;vr[6]=-1;vr[7]=-1;vr[8]=-1;vr[9]=-1;

      vr[10]=0; vr[11]=0; // Class elements

      {
	string klass = b.recall(vr);

	assert( klass == "a");

      }


      // Test Class B

      vr[0]=-1;vr[1]=-1;vr[2]=-1;vr[3]=-1;vr[4]=+1;
      vr[5]=+1;vr[6]=-1;vr[7]=-1;vr[8]=-1;vr[9]=-1;

      vr[10]=0; vr[11]=0; // Class elements

      {
	string klass = b.recall(vr);

	assert( klass == "b");

      }

      return 0;
      }
      catch ( std::exception &e) { 
	cerr << "Caught exception: " << e.what() << endl;
	return 1;
      }
      catch ( ... ) { 
	cerr << "Caught unhandled exception: " << endl;
	return 1;
      }


}

EOF
if [ $? -ne 0 ] ; then no_result ; fi


$CXX  $TEMPDIR/test.cc $include_path  -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out 
if [ $? -ne 0 ] ; then fail ; fi


pass
