#!/bin/sh
# A script to test the RexExp class

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

here=`pwd`
if [ $? -ne 0 ] ; then fail ; fi

TEMPDIR=/tmp/$$
mkdir -p $TEMPDIR
if [ $? -ne 0 ] ; then fail ; fi

cd $TEMPDIR
if [ $? -ne 0 ] ; then fail ; fi


cat <<EOF > $TEMPDIR/test.cc
#line $[$LINENO + 2]

#include "regexpr.h"
#include <assert.h>
int
main() {


    Regexpr re(".xyz$");

    assert ( true == re.exec("hello.xyz") ); 
    assert ( false == re.exec("hello.abc") ); 
    assert ( false == re.exec("hello.xyz.xys")) ; 


    return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc  $builddir/demos/common/regexpr.o $include_path -I $here/demos/common
if [ $? -ne 0 ] ; then fail ; fi


LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi


pass
