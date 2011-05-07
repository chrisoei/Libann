#!/bin/sh
# A script to test that MLP networks are persisted properly

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

cat <<EOF >> $TEMPDIR/test.cc
#line $[$LINENO +2]
#include <ann/mlp.h>

#include <fstream>

using namespace std;

using namespace ann;
int
main()
{


	{
	ofstream ofs("$TEMPDIR/foo");

	Mlp mlp(100,20,1,2,3,4,5);

	mlp.save(ofs,"blank");
	ofs.close();

	ofstream netDesc("$TEMPDIR/net1");
	netDesc << mlp ;
	netDesc.close();


	}

    {
    ifstream ifs("$TEMPDIR/foo");
    cout << "Loading\n";
 
    Mlp mlp(ifs);
    cout << "Done\n";
    ifs.close();
	ofstream netDesc("$TEMPDIR/net2");
	netDesc << mlp ;
	netDesc.close();
    }


    return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi

$CXX -g $TEMPDIR/test.cc $include_path -I $here/demos/ -L $builddir \
-lann
if [ $? -ne 0 ] ; then fail ; fi




LD_LIBRARY_PATH=$builddir ./a.out
if [ $? -ne 0 ] ; then fail ; fi


test -f $TEMPDIR/foo
if [ $? -ne 0 ] ; then fail ; fi

# Ensure that the two streamed descriptions are identical
diff $TEMPDIR/net1 $TEMPDIR/net2
if [ $? -ne 0 ] ; then fail ; fi

pass
