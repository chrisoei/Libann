#!/bin/sh
# Copyright (C) 2003  John Darrington 
# A script to test the style classifier program

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

export LD_LIBRARY_PATH=$builddir

# Train the classifier with some data
 
$builddir/demos/dict-style/dict-style -w $TEMPDIR/style-classifier $here/demos/data/texts/styles/[a-z]*[1,3-9].txt
if [ $? -ne 0 ] ; then fail ; fi

# Recall the classifier with some other data

$builddir/demos/dict-style/dict-style -r $TEMPDIR/style-classifier $here/demos/data/texts/styles/legal2.txt | tail -1 > $TEMPDIR/result
if [ $? -ne 0 ] ; then no_result ; fi

diff $TEMPDIR/result - <<EOF
legal
EOF
if [ $? -ne 0 ] ; then fail ; fi


$builddir/demos/dict-style/dict-style -r $TEMPDIR/style-classifier $here/demos/data/texts/styles/novel2.txt | tail -1 > $TEMPDIR/result
if [ $? -ne 0 ] ; then no_result ; fi

diff $TEMPDIR/result - <<EOF
novel
EOF
if [ $? -ne 0 ] ; then fail ; fi


$builddir/demos/dict-style/dict-style -r $TEMPDIR/style-classifier $here/demos/data/texts/styles/manual2.txt | tail -1 > $TEMPDIR/result
if [ $? -ne 0 ] ; then no_result ; fi

diff $TEMPDIR/result - <<EOF
manual
EOF
if [ $? -ne 0 ] ; then fail ; fi


pass
