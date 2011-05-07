#!/bin/sh
# Copyright 2003 John Darrington
# A script to test the mesh::hebbianUpdate method

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


cat << EOF > $TEMPDIR/test.cc
#line $[$LINENO+2]  "$0"

#include <ann/mesh.h>
#include <ann/boltzmann.h>

#include <iostream>

using namespace std;
using namespace ann;



int
main()
{
    
  float t ;
  ThermalBiPolarStep bps(t);

  Mesh m(7,bps);

  for ( int i = 0 ; i < 4 ; ++i ) 
    m[i]=-1;


  cout <<  m.getWeights() << endl;

  cout << "Update\n";
  m.hebbianUpdate(1.0);

  cout <<  m.getWeights() << endl;


  return 0;
}
EOF
if [ $? -ne 0 ] ; then no_result ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out > $TEMPDIR/file
if [ $? -ne 0 ] ; then no_result ; fi

cat > $TEMPDIR/expected <<EOF
0  1  1  1  1  1  1  
1  0  1  1  1  1  1  
1  1  0  1  1  1  1  
1  1  1  0  1  1  1  
1  1  1  1  0  1  1  
1  1  1  1  1  0  1  
1  1  1  1  1  1  0  

Update
0  1  1  1  1  1  1  
1  0  1  1  1  1  1  
1  1  0  1  1  1  1  
1  1  1  0  1  1  1  
1  1  1  1  0  2  2  
1  1  1  1  2  0  2  
1  1  1  1  2  2  0   

EOF
if [ $? -ne 0 ] ; then no_result ; fi


diff -w $TEMPDIR/file $TEMPDIR/expected
if [ $? -ne 0 ] ; then fail ; fi


pass
