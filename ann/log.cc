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
const static char RCSID[]="$Id: log.cc,v 1.7 2003/05/14 05:50:16 john Exp $";

#include <iostream>
#include <fstream>
#include "log.h"
#include <string>
#include <stdexcept>
#include <assert.h>

namespace ann { 


  class logInstance { 

    friend void setLogFile(const std::string  &filename);
    friend void setLogStream(std::ostream &str);
    friend std::ostream &log();


  public:
    logInstance()
    {
      fileStream = new std::ofstream("/dev/null");
      if ( !fileStream->is_open()) { 
	throw std::runtime_error("Cannot open /dev/null");
      }
      theStream = fileStream;

    }


    ~logInstance()
    {
      if (fileStream) {
       fileStream->close();
       delete fileStream;
       fileStream = 0;
      }
      theStream=0;
    }
    

  private:
    static std::ofstream *fileStream;
    static std::ostream *theStream;

  };

  
void
setLogStream(std::ostream &str)
{

  if ( logInstance::fileStream ) { 
    logInstance::fileStream->close();
    delete logInstance::fileStream;
    logInstance::fileStream = 0 ;
  }
  
  logInstance::theStream = &str;
}


void
setLogFile(const std::string  &filename)
{
      logInstance::fileStream->close();
      delete logInstance::fileStream;
      logInstance::fileStream = new std::ofstream(filename.c_str());
      if ( !logInstance::fileStream->is_open()) { 
	delete logInstance::fileStream;
	logInstance::fileStream = 0;
	throw std::runtime_error(std::string("Cannot open "+filename).c_str());
      }
      logInstance::theStream = logInstance::fileStream;
}  

std::ostream &
log()
{
  assert( logInstance::theStream) ;
  return *logInstance::theStream;
}



Trace::Trace(const std::string &file, int line, const std::string &func)
{
  ++stackDepth;
  text = file + ":" + func ;
  log() << "> " << stackDepth << " " << file  << ":" << line <<  " " << func <<  std::endl;
}
   
Trace::~Trace()
{
  log() << "< " << stackDepth << " " << text << std::endl;
  --stackDepth;
}


};


    
std::ofstream *ann::logInstance::fileStream;
std::ostream *ann::logInstance::theStream;


static ann::logInstance theInstance;

int ann::Trace::stackDepth=0;


