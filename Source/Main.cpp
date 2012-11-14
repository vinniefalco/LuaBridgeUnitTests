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

//==============================================================================

class Test1
{
public:
  Test1 ()
    : m_L (luaL_newstate ())
    , m_refErrFunc (LuaBridgeTests::addTraceback (m_L))
  {
    luaL_openlibs (m_L);

    using namespace luabridge;

    getGlobalNamespace (m_L)
      .beginClass <Set_get> ("Set_get")
        .addConstructor<void(*)(void)> ()
        .addFunction ("set", &Set_get::set)
        .addFunction ("get", &Set_get::get)
      .endClass ();
  }

  ~Test1 ()
  {
    lua_close (m_L);
  }

  int operator() ()
  {
    int result = 0;

    using namespace std;

    if (luaL_loadstring (m_L, getLua ()) != 0)
    {
      // compile-time error
      cerr << lua_tostring (m_L, -1) << endl;
      result = 1;
    }
    else if (lua_pcall (m_L, 0, 0, m_refErrFunc) != 0)
    {
      // runtime error
      cerr << lua_tostring (m_L, -1) << endl;
      result = 1;
    }

    return result;
  }

private:
  class Set_get
  {
  public:
    Set_get():_i(0.0){}
    void set(double i)
    {
      _i = i;
    }
    double get()const
    {
      return _i;
    }
  private:
    double _i;
  };

  static char const* getLua ()
  {
    return
      "\
      local N = 10 \
      local ave = 0 \
      local times = 1000 \
      for i = 0, N do \
      local obj = Set_get() \
      local t0 = os.clock() \
      for i=1,times do \
      obj:set(i) \
      if(obj:get() ~= i)then \
      error('failed') \
      end \
      end \
      local dt = os.clock()-t0 \
      if i~=0 then \
      ave = ave + dt \
      end \
      end \
      print('LuaBridge access (average elapsed time):',ave/N) \
      ";
  }

  lua_State* m_L;
  int m_refErrFunc;
};

//==============================================================================

class Test2
{
public:
  Test2 ()
    : m_L (luaL_newstate ())
    , m_refErrFunc (LuaBridgeTests::addTraceback (m_L))
  {
    luaL_openlibs (m_L);

    using namespace luabridge;
    getGlobalNamespace (m_L)
      .beginClass <ProfileBase> ("ProfileBase")
        .addFunction ("increment_a_base", &ProfileBase::increment_a_base)
        .addFunction ("virtual_func", &ProfileBase::virtual_func)
        .addFunction ("pure_virtual_func", &ProfileBase::pure_virtual_func)
      .endClass()
      .beginClass<ProfileAnotherBase>("ProfileAnotherBase")
        .addConstructor<void(*)(void)>()
      .endClass()
      .deriveClass <ProfileDerived, ProfileBase> ("ProfileDerived")
        .addConstructor<void(*)(void)>()
      .endClass()
      ;
  }

  ~Test2 ()
  {
    lua_close (m_L);
  }

  int operator() ()
  {
    int result = 0;

    using namespace std;

    if (luaL_loadstring (m_L, getLua ()) != 0)
    {
      // compile-time error
      cerr << lua_tostring (m_L, -1) << endl;
      result = 1;
    }
    else if (lua_pcall (m_L, 0, 0, m_refErrFunc) != 0)
    {
      // runtime error
      cerr << lua_tostring (m_L, -1) << endl;
      result = 1;
    }

    return result;
  }

private:
  class ProfileBase
  {
  public:
    ProfileBase():_i(0){}
    virtual ~ProfileBase(){}
    void increment_a_base(ProfileBase* base)
    {
      ++base->_i;
    }
    virtual void virtual_func()
    {
      ++_i;
    }
    virtual void pure_virtual_func() = 0;
  private:
    int _i;
  };

  class ProfileAnotherBase
  {
  public:
    virtual ~ProfileAnotherBase(){}
  };
  class ProfileDerived : public ProfileBase
  {
  public:
    virtual ~ProfileDerived(){}
    virtual void pure_virtual_func()
    {
      ++_i;
    }
  private:
    int _i;
  };

  class ProfileMultiBases : public ProfileDerived, public ProfileAnotherBase
  {
  public:
    void virtual_func()
    {
      ++_i;
    }
  private:
    int _i;
  };

  static char const* getLua ()
  {
    return
      "\
      local N = 10 \
      local ave = 0 \
      local times = 1000 \
      for i = 0, N do \
      local obj = ProfileDerived() \
      local increment_me = ProfileDerived() \
      local t0 = os.clock() \
      for i=1,times do \
      obj:increment_a_base(increment_me) \
      end \
      local dt = os.clock()-t0 \
      if i~=0 then \
      ave = ave + dt \
      end \
      end \
      print('Luabridge passing derived to a function that wants a base (average elapsed time):',ave/N) \
      ";
  }

  lua_State* m_L;
  int m_refErrFunc;
};

//==============================================================================

int main (int, char **)
{
  using namespace std;

  Test1 () ();

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
