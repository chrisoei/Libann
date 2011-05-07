#!/bin/sh
# A script to test the . . . 

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

using namespace std;

int
main()
{
  ann::vector v(4);

  v[0]=0 ; v[1]=1; v[2]=2; v[3]=3;

  ann::vector v2(3);

  v2[0]=1; v2[1]=2; v2[2]=3;
		

  assert(v.subvec(1,3) == v2);


  return 0;
}
EOF
if [ $? -ne 0 ] ; then no_result ; fi


$CXX $TEMPDIR/test.cc $include_path  -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi


pass
