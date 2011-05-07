#!/bin/sh
# A script to test the Mesh::getSourceAsynchronous method

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


cat <<EOF > $TEMPDIR/test.cc
#line $[$LINENO +2]

#include <ann/mesh.h>
#include "ann/layer.h"
#include <ann/log.h>

using namespace std;
using namespace ann;


// This activation function works as follows:
// +1 if x > 0 
// -1 if x < 0
// same as previous value if x==0
//
class BiPolarStep : public ActivationFunction { 
public:
  float operator()(float x) const {
    float retval;
    if ( x == 0.0 ) 
      retval=previousValue;
    else if ( x > 0 ) 
      retval =  1.0;
    else
      retval = -1.0;

    previousValue = retval;
    return retval;
  }

  void save(std::ostream &os) const {};
  void load(std::istream &os)  {};
private:
  mutable float previousValue;
};



int
main(int argc, char **argv)
{

    setLogStream(cout);

    BiPolarStep x;

    Mesh mesh(9,x,false);

    mesh[3]=-1.0;
    mesh[1]=-1.0;
    mesh[0]=-1.0;
    mesh[8]=-1.0;


    mesh.clamp(1,-0.56);
    mesh.clamp(5,-0.56);
    mesh.clamp(3,-0.96);

    
    mesh.getSource();

    cout << mesh.getSource() << endl;

    return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi

$CXX $include_path $TEMPDIR/test.cc -lann -L $builddir
if [ $? -ne 0 ] ; then fail ; fi


LD_LIBRARY_PATH=$builddir ./a.out > /dev/null
if [ $? -ne 0 ] ; then fail ; fi

# If the getSource function call completes in finite time, then we assume it's
# ok

pass
