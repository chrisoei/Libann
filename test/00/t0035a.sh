#!/bin/sh
# A script to test the hspell demo program

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


cat <<EOF > $TEMPDIR/dict
apricot
gravel
theory
tender
EOF
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir $builddir/demos/hspell/hspell -d $TEMPDIR/dict tendre > $TEMPDIR/out
if [ $? -ne 0 ] ; then fail ; fi

if [ "TENDER" != `cat $TEMPDIR/out` ] ; then fail ; fi

pass
