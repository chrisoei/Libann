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
const static char RCSID[]="$Id: direntry.cc,v 1.6 2003/05/14 05:50:16 john Exp $";

#include <sys/types.h>
#include <dirent.h>
#include "direntry.h"
#include "regexpr.h"
#include <stdexcept>

using namespace std;

// Return a set of the directory entries in the named directory
std::set<std::string>
getDirEntries(const std::string &dirname, const std::string &regex)
{

    DIR *dir =opendir(dirname.c_str());
    if ( !dir ) { 
      throw runtime_error("Cannot open directory");
    }


    Regexpr re(regex);

    set <string> entries;
    struct dirent *de = 0; 
    while (de = readdir(dir)) { 
      const string filename(de->d_name);
      if ( filename == ".."  ||
	   filename == "."  ) {
	continue;
      }
      if ( ! re.exec(filename) ) 
	continue;
      entries.insert(dirname+"/"+filename);
    };
    
    return entries;
}

