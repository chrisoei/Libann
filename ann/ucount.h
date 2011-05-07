#ifndef UCOUNT_H
#define UCOUNT_H
/*

    Copyright (C) 2002  John Darrington 

    $Id: ucount.h,v 1.2 2003/05/14 05:50:16 john Exp $

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

namespace ann {


  // A template class to count unique instances of variables pushed to it

  template <class T>
    class Ucount { 
    public:
  
    // Initialise the count to zero
    Ucount<T>():theCount(0),last(0){} ;

    ~Ucount() { delete last;};


    // push a value to be counted
    void 
    push(const T &t) 
    {
	if ( theCount == 0 ) { 
	  if ( last == 0 ) { 
	    last = new T(t);
	  }
	  *last = t;
	  theCount = 1;

	}
	else { 
	  if ( t == *last ) 
	    ++theCount;
	  else { 
	    theCount = 1;
	    *last = t;
	  }
        
	}
    }
  

    // Clear the count back to zero
    void clear() 
    {
	theCount = 0;
    }


    // return the current value of the count
    // (ie how many identical instances have been pushed ) 
    int count() const 
    {
	return theCount;
    }

    private:
     int theCount;
     T *last;
  };

};

#endif /* UCOUNT_H */
