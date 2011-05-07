# Test the ann::matrix class' normalise method

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
	ann::matrix w(2,4);

	const float m00=9.293;
	const float m01=92.1293;
	const float m02=3912.23;
	const float m03=12.3;

	const float m10=29.293;
	const float m11=-2.1293;
	const float m12=91.23;
	const float m13=109;


	w.set(0,0,m00); w.set(0,1,m01);	w.set(0,2,m02);	w.set(0,3,m03);
	w.set(1,0,m10); w.set(1,1,m11);	w.set(1,2,m12);	w.set(1,3,m13);
	
	w.normaliseRows();

	for ( int i = 0 ; i < w.rows(); ++i ) { 
	    ann::vector row=w.row(i);
	    assert( row.modulus() == 1.0);
	}

	return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path  -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi


pass
