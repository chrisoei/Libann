#!/bin/sh
# A script to test the word class

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

#include "demos/hspell/word.h"
#include <assert.h>
using namespace std;

int 
main(int argc, char **argv)
{

    const string s("PEANUTS");

    Word w(s,s.length()*5+14);

    assert(s== w.toString());

    return 0;
}
EOF
if [ $? -ne 0 ] ; then fail ; fi

$CXX $TEMPDIR/test.cc  $include_path $builddir/demos/hspell/word.o -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi

pass
