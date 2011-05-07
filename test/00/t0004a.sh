# Test the ann:vector class' juxtapose method

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
	ann::vector v(3);

	v[0]=1;
	v[1]=2;
	v[2]=3;

	cout << "Vector v is " << v << endl;

	ann::vector u(2);
	u[0]=9;
	u[1]=19;
	cout << "Vector u is " << u << endl;


	ann::vector result(v.juxtapose(u)) ;
	cout << "v juxtapose u  is " << result << endl;


	assert(result[0] == u[0]);
	assert(result[1] == u[1]);
	assert(result[2] == v[0]);
	assert(result[3] == v[1]);
	assert(result[4] == v[2]);

	ann::vector z(0);

	assert( v == v.juxtapose(z));


	return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir./a.out
if [ $? -ne 0 ] ; then fail ; fi

pass
