#!/bin/sh
# Copyright 2003 John Darrington
# A script to test the FeatureMap::const_feature_iterator

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

#include <ann/fm.h>

#include <iostream>

using namespace std;
using namespace ann;

int
main()
{

  FeatureMap fm;

  ExtInput f1(3);
  f1[0]=1; f1[1]=1; f1[2]=1;

  ExtInput f2(3);
  f2[0]=2; f2[1]=2; f2[2]=2;

  ExtInput f3(3);
  f3[0]=3; f3[1]=3; f3[2]=3;

  ExtInput f4(3);
  f4[0]=4; f4[1]=4; f4[2]=4;



  fm.addFeature("first",f1);
  fm.addFeature("first",f2);
  fm.addFeature("second",f3);
  fm.addFeature("third",f4);

  typedef FeatureMap::const_feature_iterator CI;

  for ( CI i = fm.fbegin(); i != fm.fend() ; ++i )  { 
    cout << (*i).first  << " " ;
    cout << (*i).second << endl;
  }

  return 0;
}

EOF
if [ $? -ne 0 ] ; then no_result ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out > $TEMPDIR/file
if [ $? -ne 0 ] ; then no_result ; fi

diff $TEMPDIR/file - << EOF
first ( 1 1 1 )T
first ( 2 2 2 )T
second ( 3 3 3 )T
third ( 4 4 4 )T
EOF
if [ $? -ne 0 ] ; then fail ; fi


pass
