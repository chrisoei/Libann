#!/bin/sh

# Copyright 2003, John Darrington

# This script  checks certain files for prescence of copyright notices




echo $1 | grep -e '.txt$' > /dev/null
if [ $? -eq 0 ] ; then exit 0 ; fi

echo $1 | grep -e '.char$' > /dev/null
if [ $? -eq 0 ] ; then exit 0 ; fi

grep -e "$2" $1 > /dev/null

