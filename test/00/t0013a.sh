# Test the vector and matrix addition methods

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
  /* First test the vectors */ 

  ann::vector v1(5);
  v1[0]=-12.09; v1[1]=4.09; v1[2]=-1.098; v1[3]=9.093;v1[4]=43.09;

  ann::vector v2(5);
  v2[0]=-121.09; v2[1]=41.09; v2[2]=-21.098; v2[3]=29.093;v2[4]=243.09;

  ann::vector v3 = v1 + v2;

  assert ( fcmp( v3[0], v1[0] + v2[0],0.0001 )); 
  assert ( fcmp( v3[1], v1[1] + v2[1],0.0001 )); 
  assert ( fcmp( v3[2], v1[2] + v2[2],0.0001 )); 
  assert ( fcmp( v3[3], v1[3] + v2[3],0.0001 )); 
  assert ( fcmp( v3[4], v1[4] + v2[4],0.0001 )); 


 /* now test the matrices */

  const float m100=-09.207;
  const float m110=-909.207;
  const float m200=97.22307;
  const float m210=89.7;


  ann::matrix m1(2,1);
  m1.set(0,0,m100);
  m1.set(1,0,m110);

  ann::matrix m2(2,1);
  m2.set(0,0,m200);
  m2.set(1,0,m210);


  ann::matrix m3(2,1);
  m3.set(0,0,m200+ m100);
  m3.set(1,0,m210 + m110);

  ofstream ofs1("$TEMPDIR/foo1");

  ofs1 << m3;

  m1+=m2;

  ofstream ofs2("$TEMPDIR/foo2");
  ofs2 << m1 ;

  return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path   -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out 
if [ $? -ne 0 ] ; then fail ; fi

diff $TEMPDIR/foo1 $TEMPDIR/foo2
if [ $? -ne 0 ] ; then fail ; fi

pass

