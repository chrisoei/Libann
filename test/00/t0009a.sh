# Test the matrix / vector multiply by scalar methods

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
    
  ann::vector v(5);
    
  v[0] = 0.9;
  v[4] = -2.3;
  v[1] = 3.45;
  v[3] = 0.92;
  v[2] = 4.908;
	
  const float scalar(34.5);

  v*=scalar;
  

  assert( fcmp(v[0], 0.9*scalar,0.0001 )) ;
  assert( fcmp(v[1], 3.45*scalar,0.0001 )) ;
  assert( fcmp(v[2], 4.908*scalar,0.0001 )) ;
  assert( fcmp(v[3], 0.92*scalar,0.0001 )) ;
  assert( fcmp(v[4], -2.3*scalar,0.0001 )) ;

  return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out 
if [ $? -ne 0 ] ; then fail ; fi


pass

