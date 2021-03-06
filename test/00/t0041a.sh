#!/bin/sh
# Copyright 2003 John Darrington
# A script to test the ann::Matrix operator== and operator!= methods

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

#include <ann/matrix.h>

int
main()
{
  ann::matrix m1(3,3);


  m1.set(0,0,0) ;  m1.set(0,1,1) ;   m1.set(0,2,2) ; 
  m1.set(1,0,4) ;  m1.set(1,1,5) ;   m1.set(1,2,6) ; 
  m1.set(2,0,7) ;  m1.set(2,1,8) ;   m1.set(2,2,9) ; 

    
  ann::matrix m2(m1);

  assert(m1 == m2);

  assert(! ( m1 != m2) );

  m2.set(0,0,1);


  assert(m1 != m2);

  assert(! ( m1 == m2) );
  

  return 0;
}

EOF
if [ $? -ne 0 ] ; then no_result ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out > $TEMPDIR/file
if [ $? -ne 0 ] ; then fail ; fi


pass
