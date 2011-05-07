# Test the raiseColumns and raiseRows methods

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
  v1[0]=2; v1[1]=3; v1[2]=4; v1[3]=5;v1[4]=6;

  ann::vector v2(3);
  v2[0]=-9; v2[1]=-8; v2[2]=-7; 

  ann::matrix m(3,5);

  m.raiseRows(v1);

  ofstream ofs1("$TEMPDIR/foo1");

  ofs1 << m;

  m.setToUnitMatrix();

  m.raiseColumns(v2);

  ofstream ofs2("$TEMPDIR/foo2");
  ofs2 << m ;

  return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out 
if [ $? -ne 0 ] ; then fail ; fi

cat > $TEMPDIR/bar1 <<EOF
3  4  5  6  7  
3  4  5  6  7  
3  4  5  6  7  
EOF
if [ $? -ne 0 ] ; then fail ; fi

diff $TEMPDIR/foo1   $TEMPDIR/bar1
if [ $? -ne 0 ] ; then fail ; fi

cat > $TEMPDIR/bar2 <<EOF
-8  -8  -8  -8  -8  
-7  -7  -7  -7  -7  
-6  -6  -6  -6  -6  
EOF
if [ $? -ne 0 ] ; then fail ; fi

diff $TEMPDIR/foo2 $TEMPDIR/bar2
if [ $? -ne 0 ] ; then fail ; fi


pass
