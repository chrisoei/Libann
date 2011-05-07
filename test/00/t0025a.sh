#!/bin/sh
# A script to test the ExtInput assignment from a ann::vector

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

cat <<EOF > $TEMPDIR/test.cc
#line $[$LINENO+2]  "$0"

#include "ann/ann.h"

using namespace std;

int
main()
{
	ann::vector v(3);

	v[0]=1;
	v[1]=2;
	v[2]=3;

        ann::ExtInput u(3);
        u=v;

	assert(u[0]==v[0]);
	assert(u[1]==v[1]);
	assert(u[2]==v[2]);

	return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi

$CXX  $TEMPDIR/test.cc   $include_path  -L $builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi

 
pass




