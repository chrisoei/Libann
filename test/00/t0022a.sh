# Test the Kohonen networks are persisted properly

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
#include <ann/kohonen.h>

#include <iostream>
#include <fstream>

using namespace std;

int
main(int argc, char **argv)
{

        srand(0);

	ann::Kohonen net(2,4);

	ofstream ofs("$TEMPDIR/netfile");
	net.save(ofs);
	ofs.close();
	
        net.print();


	return 0;
}
EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $include_path  -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out > $TEMPDIR/file1
if [ $? -ne 0 ] ; then fail ; fi


# Create a program to read a net from a file
cat << EOF > $TEMPDIR/read.cc
#line $[$LINENO+2]  "$0"

#include <ann/ann.h>
#include <ann/kohonen.h>

#include <iostream>
#include <fstream>

using namespace std;

int
main(int argc, char **argv)
{

    // Random entropy . . .
    srand(1);  ann::Kohonen net(3,8);
    
    ifstream ifs("$TEMPDIR/netfile");

    ann::Kohonen net2(ifs);
	
    net2.print();
	
    ifs.close();
    return 0;
}
EOF
if [ $? -ne 0 ] ; then fail ; fi

$CXX $TEMPDIR/read.cc $include_path  -L$builddir -lann
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir ./a.out > $TEMPDIR/file2
if [ $? -ne 0 ] ; then fail ; fi


diff $TEMPDIR/file1 $TEMPDIR/file2
if [ $? -ne 0 ] ; then fail ; fi

pass
