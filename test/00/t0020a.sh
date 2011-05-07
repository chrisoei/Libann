# Test the ann::vector class' modulus, normalise and /= methods

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
	ann::vector v(4);

	const float m00=9.293;
	const float m01=92.1293;
	const float m02=3912.23;
	const float m03=12.3;


	v[0]=m00; v[1]=m01; v[2]=m02; v[3]=m03;

	ann::vector v2(v);
	assert(v2 == v);

	// Test /= operator
	v2/=1.0;
	assert(v2 == v);

	v2.normalise();
	assert(v2.modulus() == 1.0);

	return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path  -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi


pass
