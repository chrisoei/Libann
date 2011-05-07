#!/bin/sh
# A script to test the trace class

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


cat <<EOF > $TEMPDIR/test.cc
#line $[$LINENO +2]
#include <ann/log.h>

using namespace ann;
using namespace std;


void
func()
{
  trace;
  cout << "Some Text\n";
}

int
main()
{

  setLogStream(std::cout);
  trace;


  cout << "Hello" << endl;

  func();
  
  log() << "End of program\n";

  return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi

$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out  | sed -e 's%\(.*/\)*\(.*\)%\2%' > $TEMPDIR/output
if [ $? -ne 0 ] ; then fail ; fi


cat << EOF > $TEMPDIR/expected
test.cc:59 main
Hello
test.cc:50 func
Some Text
test.cc:func
End of program
test.cc:main
EOF
if [ $? -ne 0 ] ; then fail ; fi


diff $TEMPDIR/expected $TEMPDIR/output
if [ $? -ne 0 ] ; then fail ; fi



pass
