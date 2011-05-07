#!/bin/sh
# A script to test the hopfield-char program

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

LD_LIBRARY_PATH=$builddir $builddir/demos/hopfield-char/hopfield-char $here/demos/data/glyphs/numerals
if [ $? -ne 0 ] ; then fail ; fi

# Check the results are the same as their patterns
for f in `ls` ; do
    diff $f $here/demos/data/glyphs/numerals/$f
    if [ $? -ne 0 ] ; then fail ; fi
done


pass




