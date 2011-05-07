#!/bin/sh
# Copyright 2003 John Darrington
# Test that Kohonen networks can be created and that they actually work



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


pass ()
{
    cleanup
    exit 0
}

no_result ()
{
    cleanup
    exit 2
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


cat > data1 <<x1
 0.96  1.55  
 0.98  1.45  
 0.91  1.42 
 0.88  1.51
x1
if [ $? -ne 0 ] ; then no_result ; fi

sort data1 > tmp 
if [ $? -ne 0 ] ; then no_result ; fi
mv tmp data1
if [ $? -ne 0 ] ; then no_result ; fi

cat > data2 <<x2
 1.43  0.91 
 1.39  0.96  
 1.33  0.99  
 1.41  1.02
x2
if [ $? -ne 0 ] ; then no_result ; fi

sort data2 > tmp
if [ $? -ne 0 ] ; then no_result ; fi
mv tmp data2
if [ $? -ne 0 ] ; then no_result ; fi

 
cat > data3 <<x3
 -0.49 0.21  
 -0.38 0.14 
 -0.57 0.25  
 -0.59 0.28  
x3
if [ $? -ne 0 ] ; then no_result ; fi

sort data3 > tmp
if [ $? -ne 0 ] ; then no_result ; fi
mv tmp data3
if [ $? -ne 0 ] ; then no_result ; fi



cat > test-kohonen.cc <<EOF
#line $[$LINENO+2]  "$0"

#include <ann/ann.h>
#include <ann/kohonen.h>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <fstream>

using namespace ann;
using namespace std;


int 
main(int argc, char **argv)
{

    // Kohonen net with 2 inputs and 4 outputs
    Kohonen net(2,5);


    if ( argc < 1 ) { 
     exit(1);
    }



    ifstream inputFile(argv[1]);

    set<ann::ExtInput> trainingData ;
    typedef set<ann::ExtInput>::const_iterator CI;

    while( !inputFile.eof()) {
     ExtInput ei(2);
     for(int i = 0 ; i < 2 ; ++i ) {
      float x;
      inputFile >> x;
      if ( inputFile.eof() ) 
       goto out;
      ei[i]=x;
     }
     trainingData.insert(ei);
    }
    out:

    // tolerance, adaption rate, shrink rate
    net.train(trainingData, 0.05, 0.30, 0.30);




    for( CI ci = trainingData.begin() ; ci != trainingData.end() ; ++ci ) {
     ann::vector output(net.recall(*ci));
     string filename;
     for ( int i = 0  ; i < output.size() ; ++i ) {
       if ( output[i] == 1.0) 
        filename+="1";
       else
        filename+="0";
     }


     ofstream of(filename.c_str(),ios::app);
     for(int i = 0 ; i < ci->size() ; ++i) {
       of << (*ci)[i] << " " ;
     }
     of << endl;
     of.close();

    }



    return 0;
}

EOF
if [ $? -ne 0 ] ; then no_result ; fi

$CXX  $include_path -L $builddir -lann test-kohonen.cc
if [ $? -ne 0 ] ; then no_result ; fi

cat data1 data2 data3 > data
if [ $? -ne 0 ] ; then no_result ; fi

LD_LIBRARY_PATH=$builddir ./a.out $TEMPDIR/data
if [ $? -ne 0 ] ; then no_result ; fi

# Make sure there is a file which matches the input clusters
for  i in [01]* ; do 
    sort $i > tmp; 
    mv tmp $i;

    match=no
    for j in data1 data2 data3 ; do 
     diff  -q -w $i $j
     if [ $? -eq 0 ] ; then match=yes ; fi
    done
    if [ "$match" != "yes" ] ; then fail ; fi
done





pass

