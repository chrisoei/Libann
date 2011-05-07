# Test the ann::matrix operator - method

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

#include <ann/matrix.h>

#include <iostream>

using namespace std;

int
main()
{
  ann::matrix m(3,5);

  m.randomise();

  cout << m-m << endl;

  return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out > $TEMPDIR/file
if [ $? -ne 0 ] ; then fail ; fi

grep '[1-9]' $TEMPDIR/file > /dev/null
if [ $? -eq 0 ] ; then fail ; fi


grep '0' $TEMPDIR/file > /dev/null
if [ $? -ne 0 ] ; then fail ; fi


pass
