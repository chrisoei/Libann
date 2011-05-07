# Test the ann::matrix class' subc method

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
	ann::matrix m(2,4);

	const float m01=92.1293;
	const float m02=3912.23;
	const float m03=12.3;

	const float m11=-912.1;
	const float m12=-4012.103;
	const float m13=103.123;

	m.set(0,1,  m01);
	m.set(0,2,  m02);
	m.set(0,3,  m03);

	m.set(1,1,  m11);
	m.set(1,2,  m12);
	m.set(1,3,  m13);

	ofstream ofs0("$TEMPDIR/out0");
	ofs0  << m << endl;

	ofstream ofs1("$TEMPDIR/out1");
	ofs1  << m.subc(2,4) << endl;

	return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L $builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi

sed -e 's/^[0-9\-\.]*\ *//' $TEMPDIR/out0 > $TEMPDIR/out2
if [ $? -ne 0 ] ; then fail ; fi


diff $TEMPDIR/out1 $TEMPDIR/out2
if [ $? -ne 0 ] ; then fail ; fi


pass
