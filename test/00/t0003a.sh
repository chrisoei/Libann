#!/bin/sh

# Copyright (C) 2003  John Darrington 

# Test that  the parser correctly reads matrices

cleanup()
{
    cd / 
    rm -rf $tempdir
}

fail()
{
    cleanup
    exit 1;
}

pass()
{
    cleanup
    exit 0
}

include_path='-I '`echo $search_path | sed -e 's/:/ -I /g'`
if [ $? -ne 0 ] ; then fail ; fi


sp=(${search_path//:/ })

rbd=${builddir##${sp[0]}}
rbd=$rbd/demos


for obj in parse.o tokenise.o syms.o ; do 
	for i in ${sp[@]} ; do
	 if [ -e $i/${rbd}/$obj ]  ; then 
	   objs="$objs $i/${rbd}/$obj"
	   break
	 fi
	done
done

here=`pwd`
tempdir=/tmp/$$

mkdir $tempdir

cd $tempdir
if [ $? -ne 0 ] ; then fail ; fi    


cat <<EOF > file.oct
a = [ 1 2 ; 3 4.5 ];
b = [ 1.2 10.3 ; 3.012 4.5 ];
save matrices.m
EOF
if [ $? -ne 0 ] ; then fail ; fi

octave -q file.oct
if [ $? -ne 0 ] ; then fail ; fi

test -f matrices.m
if [ $? -ne 0 ] ; then fail ; fi

cat <<EOF >$tempdir/tp.cc
#line $[$LINENO +2]
#include <ann/fm.h>
#include <ann/mlp.h>


using namespace std;
using namespace ann;
int yyparse();

FeatureMap featureMap;

typedef FeatureMap::const_iterator CI;

int
main(int argc, char **argv)
{

  if ( 0 != yyparse() ) 
    return 1;


  typedef FeatureMap::const_iterator CI;
  
  for (CI ci = featureMap.begin() ; 
    ci != featureMap.end() ; ++ci ) {
    
    set<ExtInput> theSet=ci->second;
    ExtInput any=*(theSet.begin());
    cout <<  "# name: " << ci->first << endl;
    cout <<  "# type: matrix\n";
    cout <<  "# rows: " <<  ci->second.size() << endl;
    cout <<  "# columns: " << any.size() << endl;
    
    typedef set<ExtInput>::const_iterator CSI;
    for (CSI csi = theSet.begin(); csi != theSet.end() ; ++csi ) { 
      const ExtInput &ei=*csi;
      typedef ExtInput::const_iterator CEI;
      for (CEI cei = ei.begin() ; cei != ei.end() ; ++cei ) {
       cout << *cei << " " ;
      }
      cout << endl;
    }
    cout << endl;
  }

  return 0;
}
EOF
if [ $? -ne 0 ] ; then fail ; fi


$CXX $tempdir/tp.cc $include_path -I $here/demos/ -L $builddir \
$objs \
-lann
if [ $? -ne 0 ] ; then fail ; fi


LD_LIBRARY_PATH=$builddir ./a.out < matrices.m > out.m
if [ $? -ne 0 ] ; then fail ; fi

test -f out.m
if [ $? -ne 0 ] ; then fail ; fi

cat <<EOF > file.oct
try 
load matrices.m
ap = a;
bp = b;
load -f out.m
if any (!(a == ap)) 
    exit(1);
endif
if any(!(b == bp))
    exit(1);
endif
catch 
    exit (1)
end_try_catch
EOF
if [ $? -ne 0 ] ; then fail ; fi

octave -q file.oct
if [ $? -ne 0 ] ; then fail ; fi

pass

