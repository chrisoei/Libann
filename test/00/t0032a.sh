#!/bin/sh

# A script to test that the leak detector works

cleanup ()
{
    rm -rf $TEMPDIR
    rm -f a.out
    true
}

fail ()
{
    cleanup
    echo $activity 
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


activity="wite cc 41"

cat << EOF > $TEMPDIR/test.cc
int
main()
{

  short *p = new short[100];
  int *x = new int;

  *x = 300;

  delete []p;

  delete x;


  return 0 ; 
}
EOF
if [ $? -ne 0 ] ; then fail ; fi


activity="compile 64"
$CXX -g $TEMPDIR/test.cc # $here/ann/leakDetect.cc
if [ $? -ne 0 ] ; then fail ; fi


activity="a.out 69"
./a.out
if [ $? -ne 0 ] ; then fail ; fi

activity="wite cc 73"

cat << EOF > $TEMPDIR/test.cc

int
main()
{

  short *p = new short[100];
  int *x = new int;

  *x = 300;

  delete []p;

 // delete x;


  return 0 ; 
}
EOF
if [ $? -ne 0 ] ; then fail ; fi


activity="compile 96"
$CXX -g $TEMPDIR/test.cc $here/ann/leakDetect.cc
if [ $? -ne 0 ] ; then fail ; fi


activity="a.out 101"
./a.out
test $? -ne 0 
if [ $? -ne 0 ] ; then fail ; fi

activity="write 104"
cat << EOF > $TEMPDIR/test.cc
int
main()
{

  short *p = new short[100];
  int *x = new int;

  *x = 300;

//  delete []p;

  delete x;



  return 0 ; 
}
EOF
if [ $? -ne 0 ] ; then fail ; fi


activity="compile 120"
$CXX -g $TEMPDIR/test.cc $here/ann/leakDetect.cc
if [ $? -ne 0 ] ; then fail ; fi


activity="a.out  131"

./a.out
test $? -ne 0 
if [ $? -ne 0 ] ; then fail ; fi


pass
