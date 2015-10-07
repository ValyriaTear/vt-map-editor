///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2011 by The Allacrost Project
//            Copyright (C) 2012-2015 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

/** ***************************************************************************(
*** \file    utils_pch.h
*** \author  Tyler Olsen, roots@allacrost.org
*** \author  Yohann Ferreira, yohann ferreira orange fr
*** \brief   Precompiled Header file for the project.
***
*** This code includes the headers and common types used all around the code.
***
**/

#ifndef __UTILS_PCH_HEADER__
#define __UTILS_PCH_HEADER__

//
// Include Common Headers
//

#ifdef _WIN32
#   include <windows.h>
#   include <direct.h>
#   include <shlobj.h>
#   include <stdlib.h>
#   ifndef PATH_MAX
#       define PATH_MAX _MAX_PATH   // redefine _MAX_PATH to be compatible with Darwin's PATH_MAX
#   endif
#else
#   include <dirent.h>
#   include <pwd.h>
#   include <sys/types.h>
#   include <unistd.h>
#endif

#ifdef __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <unistd.h>
#   undef check
#else
#   include <GL/glew.h>
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#ifdef __linux__
#   include <limits.h>
#endif

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iconv.h>
#include <iostream>

#include <list>
#include <map>
#include <math.h>
#include <set>
#include <sstream>
#include <stack>
#include <stdarg.h>

#ifdef HAVE_STDINT_H
#   include <stdint.h> // Using the C header, because the C++ header, <cstdint> is only available in ISO C++0x
#endif

#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <vector>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/object.hpp>

#if LUA_VERSION_NUM < 502
# define lua_pushglobaltable(L) lua_pushvalue(L, LUA_GLOBALSINDEX)
#endif

// The Windows API defines GetMessage and CreateSemaphore.
// Undefine it here to prevent conflicts within the code base.
// Case-insensitive string compare is called stricmp in Windows and strcasecmp everywhere else.
#ifdef _WIN32
#   undef GetMessage
#   undef CreateSemaphore
#   ifndef strcasecmp
#       define strcasecmp stricmp
#   endif
#endif

//
// Common Defines and Typedefs
//

/** \name Print Message Helper Macros
*** These macros assist programmers with writing debug, warning, or error messages that are to be printed to
*** a user's terminal. They are formatted as follows: `MSGTYPE:FILE:FUNCTION:LINE: `. To use the macro, all
*** that is needed is to add `<< "print message" << std::endl;` after the macro name.
**/
//@{
#define PRINT_DEBUG std::cout << "DEBUG:" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ": " << std::endl
#define PRINT_WARNING std::cout << "WARNING:" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ": " << std::endl
#define PRINT_ERROR std::cout << "ERROR:" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ": " << std::endl
//@}

/** \name Print Message Helper Macros With Conditional
*** \param var Any type of variable that can be used to evaluate a true/false condition
*** These macros perform the exact same function as the previous set of print message macros, but these include a conditional
*** parameter. If the parameter is true the message will be printed and if it is false, no message will be printed. Note that
*** the if statement is not enclosed in brackets, so the programmer is not required to add a terminating bracket after they
*** append their print message.
*** \note There is no error conditional macro because detected errors should always be printed when they are discovered
**/
//@{
#define IF_PRINT_DEBUG(var) if (var) std::cout << "DEBUG:" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ": " << std::endl
#define IF_PRINT_WARNING(var) if (var) std::cout << "WARNING:" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ": " << std::endl
//@}

//! Contains utility code used across the entire source code
namespace vt_utils
{

//! Determines whether the code in the vt_utils namespace should print debug statements or not.
extern bool UTILS_DEBUG;

} // vt_utils

#endif // __UTILS_PCH_HEADER__
