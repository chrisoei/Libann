#!/bin/sh
# Copyright 2003 John Darrington
# A script to test the matrix random Symetrical method

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

#include <iostream>

using namespace std;

int
main()
{
	ann::matrix m(4,4);

	// m.randomise();
	m.randomiseSymetrical();


	for ( int i = 0 ; i < m.rows() ; ++i ) {
	    for ( int j = 0 ; j < i ; ++j ) 
		assert(m.get(i,j) == m.get(j,i));
	}

	return 0;

}
EOF
if [ $? -ne 0 ] ; then no_result ; fi


$CXX $TEMPDIR/test.cc $include_path -L $builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi


pass
