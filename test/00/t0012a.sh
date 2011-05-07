# Test the identity matrix

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
#include <math.h>


using namespace std;

bool
fcmp(float f1, float f2, float tolerance)
{
  float diff = f1 -f2;
  if ( fabs(diff) > tolerance) return false;
  return true;
}


int
main(int argc, char **argv)
{

 /* now test the matrices */


  ann::vector v1(5);
  v1[0]=23.4; v1[1]=4.06; v1[2] = 09.823; v1[3]=-3.09; v1[4]=0.206;


  ann::squareMatrix eye(5);
  eye.setToIdentityMatrix();



  assert ( (eye *v1 ) == v1 ) ;

  return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path  -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out 
if [ $? -ne 0 ] ; then fail ; fi

pass

