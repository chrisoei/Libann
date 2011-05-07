# Test that a network can train on a simple data set
# and can recall correctly

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

# Create a data set
cat <<EOF > $TEMPDIR/makedata.m
1 ;
%Generate some data clustered into classes
%
%gendata(N,A)         -- generate a N x size(A) matrix.   The columns are
%                        from a normal distribution, eacd with a mean 
%                        taken from the columns of  A.  

function a = gendata ( qty, mean  )

  if ( nargin != 2 ) 
    usage("gendata(qty,[mean])");
  endif

  if ( ! is_vector(mean))
    error( "%s is not a vector", sprintf( "%f ",mean));
  endif
  
  oldelok = empty_list_elements_ok;
  empty_list_elements_ok= 1;
  a = [];
  for i = 1:qty 
    row = [];
    for j = 1:columns(mean) 
      element = randn + mean(j);
      row = [ row , element ]; 
    endfor
    a = [ [a] ; [ row ] ] ;
  endfor
  empty_list_elements_ok= oldelok;
endfunction

try
samples_per_class = 4;

a = gendata ( samples_per_class , [ 1 , 2 , 3 ]);
b = gendata ( samples_per_class , [ 3 , 2 , 1 ]);
c = gendata ( samples_per_class , [ -4 , 2 , 1 ]);

xb = gendata ( 1 , [ 3 , 2 , 1 ]);

save data.m a b c
save x.m xb

catch
   exit (1);
end_try_catch

EOF
if [ $? -ne 0 ] ; then fail ; fi

octave -q -f $TEMPDIR/makedata.m
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir $builddir/mlp -w -n -t 0.3  -a 1 -d -i 3 < $TEMPDIR/data.m
if [ $? -ne 0 ] ; then fail ; fi

test -f $TEMPDIR/mlp.net
if [ $? -ne 0 ] ; then fail ; fi

LD_LIBRARY_PATH=$builddir $builddir/mlp -r < $TEMPDIR/x.m > $TEMPDIR/out
if [ $? -ne 0 ] ; then fail ; fi

grep 'belongs to class "b"' $TEMPDIR/out > /dev/null
if [ $? -ne 0 ] ; then fail ; fi


pass




