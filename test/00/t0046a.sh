#!/bin/sh
# Copyright 2003 John Darrington
# A script to test for the unbalanced feature map bug

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

#include <ann/ann.h>
#include <ann/fm.h>
#include <ann/mlp.h>

#include <assert.h>
#include <string.h>
#include <stdexcept>

using namespace ann;
using namespace std;


int
main(int argc, char **argv)
{
    

  try { 

  FeatureMap fm;

  const int patternSize=6;

  ExtInput f1(patternSize);
  f1[0]=1; f1[1]=1; f1[2]=1;  f1[3]=1; f1[4]=1; f1[5]=1;

  ExtInput f2(patternSize);
  f2[0]=2; f2[1]=2; f2[2]=2;  f2[3]=1; f2[4]=1; f2[5]=2;

  ExtInput f3(patternSize);
  f3[0]=3; f3[1]=3; f3[2]=3;  f3[3]=1; f3[4]=1; f3[5]=3;

  ExtInput f4(patternSize);
  f4[0]=4; f4[1]=4; f4[2]=4;  f4[3]=1; f4[4]=1; f4[5]=4;

  ExtInput f5(patternSize);
  f5[0]=4; f5[1]=4; f5[2]=5;  f5[3]=1; f5[4]=1; f5[5]=4;

  ExtInput f6(patternSize);
  f6[0]=4; f6[1]=4; f6[2]=6;  f6[3]=1; f6[4]=1; f6[5]=4;

  ExtInput f7(patternSize);
  f7[0]=4; f7[1]=4; f7[2]=7;  f7[3]=1; f7[4]=1; f7[5]=4;


  fm.addFeature("second",f7);

  fm.addFeature("first",f1);
  fm.addFeature("first",f2);
  fm.addFeature("first",f3);
  fm.addFeature("first",f4);
  fm.addFeature("first",f5);
  fm.addFeature("first",f6);



  ann::Mlp mlp(patternSize,1,1,1);

  mlp.train(fm);


  }
  catch (const std::runtime_error &r )  {
    assert ( 0 == strcmp(r.what(),"Feature map is unbalanced"));
    return 0;
  }   
  catch (const std::exception &e )  {
    std::cout << "Caught execption: " << e.what() << endl;
    return 1;
  }
  catch (...) { 
    std::cout << "Caught unhandled execption\n" ;
    return 1;
  }
    

  return 1;
}

EOF
if [ $? -ne 0 ] ; then no_result ; fi



$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then no_result ; fi

LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi


pass
