#!/bin/sh
# A script to test the regex parameter of getDirEntries

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


mkdir -p $TEMPDIR/data
if [ $? -ne 0 ] ; then fail ; fi

touch $TEMPDIR/data/foo.xyz
if [ $? -ne 0 ] ; then fail ; fi
touch $TEMPDIR/data/bar.xyz
if [ $? -ne 0 ] ; then fail ; fi
touch $TEMPDIR/data/wish.xyz
if [ $? -ne 0 ] ; then fail ; fi
touch $TEMPDIR/data/fiz.yza
if [ $? -ne 0 ] ; then fail ; fi
touch $TEMPDIR/data/fiz.abc
if [ $? -ne 0 ] ; then fail ; fi


cat <<EOF > $TEMPDIR/test.cc
#line $[$LINENO + 2]

#include <set>
#include <string>
#include <direntry.h>
#include <iostream>

using namespace std;

int
main()
{
    try { 
    set<string> files=getDirEntries("$TEMPDIR/data", ".xyz$");

    typedef set<string>::const_iterator CI;
    for ( CI ci = files.begin() ; ci != files.end() ; ++ci ) {
	cout << *ci << endl;
    }

    return 0;
    } 
    catch ( const exception &e ) { 
      cerr << "An exception occured : " << e.what() << endl;
      return 1;
    }
    catch ( ...) {
      cerr << "An unknown exception occured" << endl;
      return 1;
    }

}
EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $TEMPDIR/test.cc $builddir/demos/common/direntry.o \
 $builddir/demos/common/regexpr.o \
 $include_path -I $here/demos/common 
if [ $? -ne 0 ] ; then fail ; fi


LD_LIBRARY_PATH=$builddir ./a.out > $TEMPDIR/out
if [ $? -ne 0 ] ; then fail ; fi

grep 'abc' $TEMPDIR/out
test $? -ne 0 
if [ $? -ne 0 ] ; then fail ; fi

grep 'yza' $TEMPDIR/out
test $? -ne 0 
if [ $? -ne 0 ] ; then fail ; fi


pass



