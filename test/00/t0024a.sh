#!/bin/sh
# A script to test the MLP demonstration program works

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


LD_LIBRARY_PATH=$builddir $builddir/demos/mlp-char/mlp-char $here/demos/data/glyphs | grep 'has class' > $TEMPDIR/output
if [ $? -ne 0 ] ; then fail ; fi

# Make sure that most of the samples are correctly classified
for class in a b c d e ; do 
    samples=`grep "${class}[0-9].char" $TEMPDIR/output | wc -l`
    correct=`grep "${class}[0-9].char" $TEMPDIR/output | grep "class ${class}" | wc -l`
    if [ $correct -lt $[$samples * 8 / 10 ] ] ; then fail ; fi
done



pass
