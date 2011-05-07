# Test the ann:version function

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

#include <ann/ann.h>

#include <iostream>

using namespace std;

int
main()
{
    cout << ann::version() << endl;
}
EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out > $TEMPDIR/vers
if [ $? -ne 0 ] ; then fail ; fi


ok_to_test=yes
aesub '$vers' > /dev/null
if [ $? -ne 0 ] ; then 
	echo This test not running in aegis environment
	ok_to_test=no
fi

if [ "$ok_to_test" != "yes" ] ; then 
	echo Therefore this test is not meaningfull
	echo ASSUMING it\'s ok
	pass
fi


if [ `cat $TEMPDIR/vers` != `aesub '$vers'` ] ; then fail ; fi

pass
