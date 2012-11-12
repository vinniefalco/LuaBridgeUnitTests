//------------------------------------------------------------------------------
/*
  https://github.com/vinniefalco/LuaBridgeUnitTests
  
  Copyright (C) 2012, Vinnie Falco <vinnie.falco@gmail.com>
  Copyright (C) 2007, Nathan Reed

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

/**
  Command line version of LuaBridge test suite.
*/

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "LuaUnityBuild/LuaUnityBuild.h"

#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/RefCountedPtr.h"

#include "BinaryData.h"
#include "SpeedTests.h"

#include "UnitTests.h"
#include "UnitTests.cpp"

using namespace std;

int main (int, char **)
{
  lua_State* L = luaL_newstate ();

  luaL_openlibs (L);

  int errorFunctionRef = LuaBridgeTests::addTraceback (L);

  LuaBridgeTests::addToState (L);

  // Execute lua files in order
  if (luaL_loadstring (L, BinaryData::UnitTests_lua) != 0)
  {
    // compile-time error
    cerr << lua_tostring(L, -1) << endl;
    lua_close(L);
    return 1;
  }
  else if (lua_pcall(L, 0, 0, errorFunctionRef) != 0)
  {
    // runtime error
    cerr << lua_tostring(L, -1) << endl;
    lua_close(L);
    return 1;
  }

  runSpeedTests ();

  lua_close(L);
  return 0;
}
