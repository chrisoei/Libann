#!/bin/sh
# A script to test the Boltzmann machine demonstration program works
# Copyright 2003 John Darrington

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


LD_LIBRARY_PATH=$builddir $builddir/demos/boltzmann/boltzmann-char $here/demos/data/glyphs/xo > $TEMPDIR/output
if [ $? -ne 0 ] ; then fail ; fi
testpid=$!


# Make sure that most of the samples are correctly classified
for class in x o p ; do 
    samples=`grep -e "/${class}[1-9]*\.char" $TEMPDIR/output | wc -l `

    correct=`grep -e "/${class}[1-9]*\.char" $TEMPDIR/output | grep "class ${class}" | wc -l` 
	echo score $correct out of $samples
    if [ $samples -ne $correct ] ; then fail ; fi
done




pass
