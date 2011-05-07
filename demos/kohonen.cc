# Test that Kohonen networks can be created and that they actually work

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

here=`pwd`
if [ $? -ne 0 ] ; then fail ; fi

TEMPDIR=/tmp/$$
mkdir -p $TEMPDIR
if [ $? -ne 0 ] ; then fail ; fi

cd $TEMPDIR
if [ $? -ne 0 ] ; then fail ; fi


cat > data <<EOF
0.96  1.55  
0.98  1.45  
0.91  1.40 
0.78  1.51  
              
1.43  0.91 
1.39  0.96  
1.33  0.99  
1.51  1.02 
              
-0.39 0.21  
-0.28 0.14 
-0.57 0.05  
-0.59 0.28  

EOF
if [ $? -ne 0 ] ; then fail ; fi

cat > test-kohonen.cc <<EOF
#line $[$LINENO+2]  "$0"

#include <kohonen.h>
#include <vector>
#include <set>
#include <layer.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace ann;
using namespace std;


int 
main(int argc, char **argv)
{

    // Kohonen net with 2 inputs and 4 outputs
    Kohonen net(2,5);


    if ( argc < 1 ) { 
     exit(1);
    }



    ifstream inputFile(argv[1]);

    set<ann::ExtInput> trainingData ;
    typedef set<ann::ExtInput>::const_iterator CI;

    while( !inputFile.eof()) {
     ExtInput ei(2);
     for(int i = 0 ; i < 2 ; ++i ) {
      float x;
      inputFile >> x;
      if ( inputFile.eof() ) 
       goto out;
      ei[i]=x;
     }
     trainingData.insert(ei);
    }
    out:

    // tolerance, adaption rate, shrink rate
    net.train(trainingData, 0.05, 0.40, 0.40);




    for( CI ci = trainingData.begin() ; ci != trainingData.end() ; ++ci ) {
     ann::vector output(net.recall(*ci));
     string filename;
     for ( int i = 0  ; i < output.size() ; ++i ) {
       if ( output[i] == 1.0) 
        filename+="1";
       else
        filename+="0";
     }

     ofstream of(filename.c_str(), ios::ate);
     for(int i = 0 ; i < ci->size() ; ++i) {
       of << (*ci)[i] << " " ;
     }
     of << endl;
     of.close();
    }



    return 0;
}

EOF
if [ $? -ne 0 ] ; then fail ; fi


g++  -I $here/inst/include/ann -L $here/inst/lib -lann test-kohonen.cc
if [ $? -ne 0 ] ; then fail ; fi


LD_LIBRARY_PATH=$here/inst/lib ./a.out $TEMPDIR/data
if [ $? -ne 0 ] ; then fail ; fi

cmd="graph -T X "
n=2;
for  i in [01]* ; do 
    n=$[$n +1];
    cmd="$cmd -m 0 -S $n ./$i"
done

echo "$cmd"
$cmd

pass

