/*

    Copyright (C) 2002  John Darrington 

    This program is free software; you can redistibute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
const static char RCSID[]="$Id: leakDetect.cc,v 1.5 2003/05/14 05:50:16 john Exp $";


#include <cstdio>
#include <new>
#include <cstdlib>

#include <map>
#include <assert.h>

using namespace std;

/*! \file leakDetect.cc
 \brief A module to perform simple memory leak detection.

 This file can detect errors resulting from incorrect use of the \c new
 and \c delete operators.  It will not work if \c malloc and \c free
 have been used in the program.  It probably won't be happy if the
 placement new operator has been used either.

 This file does not have a corresponding header file.
 None of the symbols in this file should be explicitly referenced.
 It should, during testing/development be linked against a program
 to be checked.  The program, at termination will report memory usage
 and set the return status accordingly.

 The final product should then be linked without this
 file.
*/



/// The leak_detect namespace contains functions and structures
/// relating to the memory leak detector.  There are no classes in this
/// namespace. 
namespace leak_detect { 

  /// A map of sizes indexed by pointer.  These are used to keep track
  /// of the size of data chunks which have been allocated 
  typedef std::map<void *, size_t> lengthMap;

// This module contains two objects of type lengthMap
// The manner in which they get allocated and constructed is bizarre.
// The reason is, that they have to be available before absolutely
// anything else, even statically allocated objects.
// The way we do this is to preallocate storage space to them , and
// then use placement new to explicitly inititalise them.

static lengthMap *lengthMapPtr = 0;
static lengthMap *lengthMapArray = 0;

static bool initialised=false;

static unsigned char mapStorageA[sizeof(lengthMap)];
static unsigned char mapStorageP[sizeof(lengthMap)];




  void leakage_report();
  
 
  void 
  initialise() 
  {

    if ( !initialised)  { 
      atexit(leakage_report);
    }

    initialised=true;

    if ( lengthMapPtr == 0 ) { 

      lengthMapPtr = new(&mapStorageP) lengthMap;
    }


    if ( lengthMapArray == 0 ) { 
      lengthMapArray = new(&mapStorageA) lengthMap;
    }

  }

  void deAllocateMaps()
  {
    lengthMapPtr->~lengthMap();
    lengthMapPtr=0;

    lengthMapArray->~lengthMap();
    lengthMapArray=0;
    
  }

  size_t 
  totalLeakage() 
  {

    initialise();

    size_t bytesLeaked = 0;

    typedef std::map<void *,size_t>::const_iterator CI;

    for ( CI ci = lengthMapPtr->begin() ; ci != lengthMapPtr->end() ; ++ ci ) 
      bytesLeaked+=ci->second;


    for ( CI ci = lengthMapArray->begin() ; ci != lengthMapArray->end() ; ++ ci )
      bytesLeaked+=ci->second;

    return bytesLeaked;

  }



  /*! \brief Display a report of unallocated memory to \c stderr

    This function reports the total amount of memory leaked.  If this
    is not zero, then it'll terminate the program with non-zero status.
  */
  void 
  leakage_report()
  {

    initialise();

    FILE *os = stderr;


    fprintf(os,"Leakage Report\n");
    fprintf(os,"==============\n\n");

    fprintf(os,"Pointer Allocation:\n");
    typedef std::map<void *,size_t>::const_iterator CI;
    for ( CI ci = lengthMapPtr->begin() ; ci != lengthMapPtr->end() ; ++ ci ) 
      fprintf(os,"0x%08x: %d\n",ci->first,ci->second);

    fprintf(os,"\nArray Allocation:\n");
    for ( CI ci = lengthMapArray->begin() ; ci != lengthMapArray->end() ; ++ ci ) 
      fprintf(os,"0x%08x: %d\n",ci->first,ci->second);


    fprintf(os,"\nTotal bytes leaked: %d\n", totalLeakage());


    fflush(os);


    if (totalLeakage() != 0 ) 
      abort();

  }


};


using namespace leak_detect;


void *operator new(size_t size) 
{

  void *ptr = malloc (size) ; 

  if (!ptr) 
    throw bad_alloc();

  initialise();


  (*lengthMapPtr)[ptr]=size;


  return ptr;
}



void operator delete(void *p)
{

  // delete is guaranteed to ignore zero
  if ( p == 0 ) 
    return;

  const map<void*,size_t>::const_iterator x = lengthMapPtr->find(p);

  if ( x == lengthMapPtr->end() ) 
    fprintf(stderr,"Bad Deallocation of location 0x%08x\n",p);

  lengthMapPtr->erase(p);
  
  free(p);

}



void *operator new[](size_t size) 
{
  void *ptr = malloc (size) ; 

  if (!ptr) 
    throw bad_alloc();

  initialise();

  (*lengthMapArray)[ptr]=size;

  return ptr;

}


void operator delete[](void *p)
{

  // delete is guaranteed to ignore zero
  if ( p == 0 ) 
    return;

  const map<void*,size_t>::const_iterator x = lengthMapArray->find(p);

  if ( x == lengthMapArray->end() ) 
    fprintf(stderr,"Bad Deallocation of location 0x%08x\n",p );


  lengthMapArray->erase(p);

  free(p);

}

