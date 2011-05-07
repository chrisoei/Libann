#!/bin/sh

tempfile=/tmp/$$depends

# Clean up and exit with status $1
clearup ()
{
	rm -f $tempfile
	exit $1
}

RM='rm -f'
CXX=g++

if [ $# -lt 2 ] ; then 
	echo usage: generate-depends source-file output-file [ basedir ]
	exit 1
fi

source=$1
target=$2
basedir=$3



here=`pwd`
if [ -z "$3" ]  ; then 
	basedir=$here
fi 


# This ensures that basedir is absolute and not anything like . or ..
cd $basedir ; basedir=`pwd` 
cd $here


case $source in 
*.cc) COMP=c++ ;;
*.c)  COMP=cc ;;
* ) echo "Don't know how to generate dependencies for $source" ; exit 1 ;;
esac

dirname=`dirname $target`

shopt -s extglob
builddir=${here##$basedir*(/)}
rdirname=${dirname##${basedir}*(/)$builddir*(/)}
rtarget=${target##${basedir}*(/)$builddir*(/)}
rsource=${source##${basedir}*(/)}
shopt -u extglob

printf "$rtarget " > $target
printf $rdirname/ >> $target

${COMP} -MM ${CPPFLAGS} $source  > $tempfile
status=$?
if [ $status -ne 0 ] ; then 
  echo Failed to make dependency file $target
  ${RM} $target
  clearup 1
fi

sed -e "s%$basedir/*%%g" $tempfile >> $target
status=$?

clearup 0
