# Test the ann::log functions

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
#line $[$LINENO+2]  "$0"

#include <ann/log.h>
#include <iostream>
#include <string.h>

int
main(int argc, char **argv)
{

  if ( strcmp(argv[1],"cout") == 0 ) {
    ann::setLogStream(std::cout);
  }
  else  if ( strcmp(argv[1],"file") == 0 ) {
    ann::setLogFile("$TEMPDIR/foobar");
  }


  ann::log() << "Hello"  << std::endl;
  return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out  xxx > $TEMPDIR/file
if [ $? -ne 0 ] ; then fail ; fi

test -s $TEMPDIR/file
if [ $? -eq 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out  cout | grep 'Hello' > /dev/null
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out  file > $TEMPDIR/file
if [ $? -ne 0 ] ; then fail ; fi

test -s $TEMPDIR/file
if [ $? -eq 0 ] ; then fail ; fi


grep 'Hello'  $TEMPDIR/foobar > /dev/null
if [ $? -ne 0 ] ; then fail ; fi


pass

