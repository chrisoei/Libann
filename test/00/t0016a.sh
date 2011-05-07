#!/bin/sh
# A script to test the matrix::isSquare and matrix::isSymetrical methods

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

include_path='-I '`echo $search_path | sed -e 's/:/ -I /g'`
if [ $? -ne 0 ] ; then fail ; fi


here=`pwd`
if [ $? -ne 0 ] ; then fail ; fi

TEMPDIR=/tmp/$$
mkdir -p $TEMPDIR
if [ $? -ne 0 ] ; then fail ; fi

cd $TEMPDIR
if [ $? -ne 0 ] ; then fail ; fi


cat << EOF > $TEMPDIR/test.cc
#line $[$LINENO +2]

#include "ann/matrix.h"
#include <assert.h>

using namespace ann;

int 
main()
{
    matrix m(3,3);

    const matrix n(3,2);

    assert(m.isSquare());
    assert(!n.isSquare());


    assert(m.isSymetrical()); 

    m.set(0,1,45.03);

    assert(!m.isSymetrical()); 


    return 0 ;
}
EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L $builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out 
if [ $? -ne 0 ] ; then fail ; fi




pass
