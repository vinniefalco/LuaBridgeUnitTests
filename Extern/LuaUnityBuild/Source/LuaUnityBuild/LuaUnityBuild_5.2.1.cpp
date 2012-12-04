//------------------------------------------------------------------------------
/*
  https://github.com/vinniefalco/LuaUnityBuild

  Copyright 2012, Vinnie Falco <vinnie.falco@gmail.com>

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

#include "AppConfig.h"

#define LUAUNITYBUILD_COMPILE_SOURCE
#include "LuaUnityBuild.h"

#if LUAUNITYBUILD_VERSION == 502

#if _MSC_VER
#pragma push_macro("_CRT_SECURE_NO_WARNINGS")
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* luaconf.h only declares some things if it is being included
   from certain source files. We define all the relevant macros
   and include luaconf.h once so we get all the declarations.
*/
#define lobject_c
#define lvm_c
#define LUA_CORE
#define LUA_LIB
#include "Lua.5.2.1/src/luaconf.h"
#undef lobject_c
#undef lvm_c
#undef LUA_CORE
#undef LUA_LIB

#if _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4244) /* Possible loss of data */
#pragma warning (disable: 4702) /* Unreachable code */
#endif

/* Include this early to prevent the conflict with luai_hashnum
   and supress the warning caused by #define lua_assert
*/
#include "Lua.5.2.1/src/ltable.c"

#include "Lua.5.2.1/src/lauxlib.c"
#include "Lua.5.2.1/src/lbaselib.c"

#include "Lua.5.2.1/src/lbitlib.c"
#include "Lua.5.2.1/src/lcorolib.c"
#include "Lua.5.2.1/src/ldblib.c"
#include "Lua.5.2.1/src/linit.c"
#include "Lua.5.2.1/src/liolib.c"
#include "Lua.5.2.1/src/lmathlib.c"
#include "Lua.5.2.1/src/loslib.c"
#include "Lua.5.2.1/src/lstrlib.c"
#include "Lua.5.2.1/src/ltablib.c"

#include "Lua.5.2.1/src/lapi.c"
#include "Lua.5.2.1/src/lcode.c"
#include "Lua.5.2.1/src/lctype.c"
#include "Lua.5.2.1/src/ldebug.c"

#if _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4297) /* function assumed not to throw an exception but does */
#endif
#include "Lua.5.2.1/src/ldo.c"
#if _MSC_VER
#pragma warning (pop)
#endif

#include "Lua.5.2.1/src/ldump.c"
#include "Lua.5.2.1/src/lfunc.c"

#if _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4146) /* unary minus operator applied to unsigned type, result still unsigned */
#endif
#include "Lua.5.2.1/src/lgc.c"
#if _MSC_VER
#pragma warning (pop)
#endif

#include "Lua.5.2.1/src/llex.c"
#include "Lua.5.2.1/src/lmem.c"
#include "Lua.5.2.1/src/lobject.c"
#include "Lua.5.2.1/src/lopcodes.c"
#include "Lua.5.2.1/src/lparser.c"
#include "Lua.5.2.1/src/lstate.c"
#include "Lua.5.2.1/src/lstring.c"
#include "Lua.5.2.1/src/ltm.c"
#include "Lua.5.2.1/src/lundump.c"
#include "Lua.5.2.1/src/lvm.c"
#include "Lua.5.2.1/src/lzio.c"

/* loadlib.c includes Windows.h, which defines the LoadString macro,
   so include it last to prevent errors.
*/
#include "Lua.5.2.1/src/loadlib.c"

#if _MSC_VER
#pragma warning (pop)
#endif

#ifdef __cplusplus
}
#endif

#if _MSC_VER
#pragma pop_macro("_CRT_SECURE_NO_WARNINGS")
#endif

#endif