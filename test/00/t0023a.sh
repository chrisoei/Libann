#!/bin/sh
# A script to test the Word Frequency Demonstration Program

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

sp=(${search_path//:/ })

rbd=${builddir##${sp[0]}}

binary=demos/wordFreq/freq

for i in ${sp[@]} ; do
 if [ -e $i/${rbd}/$binary ]  ; then 
   prog="$i/${rbd}/$binary"
   break
 fi
done


LD_LIBRARY_PATH=$builddir $prog $here/demos/data/texts
if [ $? -ne 0 ] ; then fail ; fi

classes=`ls -d $TEMPDIR/[10]* | wc -l`
if [ $? -ne 0 ] ; then fail ; fi

samples=`ls $here/demos/data/texts | wc -l`
if [ $? -ne 0 ] ; then fail ; fi

echo No of samples is $samples

# Should be more than one class
if [ $classes -le 1 ] ; then fail ; fi

# but not more than half of the no of samples
if [ $classes -gt $[$samples /2 ] ] ; then fail ; fi

pass
