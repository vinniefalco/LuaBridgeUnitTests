//------------------------------------------------------------------------------
/*
  https://github.com/vinniefalco/LuaUnityBuild

  Copyright (C) 2012, Vinnie Falco <vinnie.falco@gmail.com>

  License: The MIT License (http://www.opensource.org/licenses/mit-license.php)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
//==============================================================================

/** Include file for the Lua unity build.
*/

#ifndef LUAUNITYBUILD_HEADER
#define LUAUNITYBUILD_HEADER

// This determines which version of Lua to use.
// The value is the same as LUA_VERSION_NUM in lua.h

#ifndef LUAUNITYBUILD_VERSION
#define LUAUNITYBUILD_VERSION 502   // use 5.2.1
//#define LUAUNITYBUILD_VERSION 501   // use 5.1.5
#endif

#ifndef LUAUNITYBUILD_COMPILE_SOURCE

#if LUAUNITYBUILD_VERSION >= 502
# include "Lua.5.2.1/src/lua.hpp"

#elif LUAUNITYBUILD_VERSION >= 501
extern "C"
{
# include "Lua.5.1.5/src/lua.h"
# include "Lua.5.1.5/src/lualib.h"
# include "Lua.5.1.5/src/lauxlib.h"
}

#else
# error "Unknown LUA_VERSION_NUM"

#endif

#endif

#endif
