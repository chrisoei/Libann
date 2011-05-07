# Test the matrix multiply by scalar method

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
  ann::matrix m(3,5);

  m.randomise();

  const float scalar=9.095;

  std::vector<ann::vector> orig(7);
  for ( int i = 0 ; i < 3 ; ++i) {
    orig[i]=(m.row(i));
  }

  m*=scalar;

  for ( int i = 0 ; i < 3 ; ++i) {
    const ann::vector r(m.row(i));
    const ann::vector oldvec(orig[i]);

  assert( fcmp(r[0], oldvec[0]*scalar,0.0001 )) ;
  assert( fcmp(r[1], oldvec[1]*scalar,0.0001 )) ;
  assert( fcmp(r[2], oldvec[2]*scalar,0.0001 )) ;
  assert( fcmp(r[3], oldvec[3]*scalar,0.0001 )) ;
  assert( fcmp(r[4], oldvec[4]*scalar,0.0001 )) ;


  }


  return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out 
if [ $? -ne 0 ] ; then fail ; fi


pass

