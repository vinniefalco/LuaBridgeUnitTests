//------------------------------------------------------------------------------
/*
  https://github.com/vinniefalco/LuaBridgeUnitTests
  
  Copyright 2012, Vinnie Falco <vinnie.falco@gmail.com>
  Copyright 2007, Nathan Reed

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
#include "LuaBridge/RefCountedObject.h"
#include "LuaBridge/RefCountedPtr.h"

#include "BinaryData.h"
#include "SpeedTests.h"

using namespace std;

//==============================================================================
/**
    Base for unit tests.
*/
class TestBase
{
public:
  explicit TestBase (char const* name)
    : m_name (name)
    , m_L (luaL_newstate ())
  {
    luaL_openlibs (m_L);

    // Create the traceback function.
    lua_pushcfunction (m_L, &traceback);
    m_errFunc = luaL_ref (m_L, LUA_REGISTRYINDEX);
  }

  ~TestBase ()
  {
    lua_close (m_L);
  }

  lua_State* state () const
  {
    return m_L;
  }

  /** Called when a test condition evaluates to false.
  */
  void fail (char const* cond, char const* file, int line)
  {
    file;
    line;

    using namespace std;

    cout << "FAIL: " << cond << endl;
  }

  /** Run the test.
  */
  int operator() ()
  {
    int result = 0;

    using namespace std;

    cout << "Test \"" << m_name << "\":" << endl;

    // Load the main chunk.
    //
    if (luaL_loadstring (m_L, getMainChunk ()) != 0)
    {
      // compile-time error
      cout << lua_tostring (m_L, -1) << endl;
      result = 1;
    }
    else if (lua_pcall (m_L, 0, 0, m_errFunc) != 0)
    {
      // runtime error
      cout << lua_tostring (m_L, -1) << endl;
      result = 1;
    }
    else
    {
      // Run the native portion
      result = runNativeCode ();
    }

    cout << endl;

    return result;
  }

  /** Retrieve the main Lua chunk.

      Subclasses should return a string with Lua script.
  */
  virtual char const* getMainChunk ()
  {
    return "";
  }

  /** Run the native portion of the test.
  */
  virtual int runNativeCode ()
  {
    int result = 0;

    return result;
  }

private:
  /** Lua stack traceback function.

      When a runtime error occurs, this will append the call stack to the
      error message. Adapted from lua.c.
  */
  static int traceback (lua_State* L)
  {
    // look up Lua's 'debug.traceback' function
    lua_getglobal (L, "debug");
    if (!lua_istable (L, -1))
    {
      lua_pop (L, 1);
      return 1;
    }
    
    lua_getfield (L, -1, "traceback");
    if (!lua_isfunction (L, -1))
    {
      lua_pop(L, 2);
      return 1;
    }
    
    lua_pushvalue (L, 1);   // pass error message
    lua_pushinteger (L, 2); // skip this function and traceback
    lua_call (L, 2, 1);     // call debug.traceback

    return 1;
  }

private:
  TestBase (TestBase const&);
  TestBase& operator= (TestBase const&);

protected:
  char const* m_name;
  lua_State* const m_L;
  int m_errFunc;
};

/** Verify a test condition.
*/
#define ASSURE(cond) (void)((cond)||(fail(#cond,__FILE__,__LINE__),0))

//------------------------------------------------------------------------------

/** Call all functions.

    This makes sure all the templates compile.
*/
class Test1 : public TestBase
{
public:
  static void f0 () { }
  static void f1 (int) { }
  static void f2 (int,int) { }
  static void f3 (int,int,int) { }
  static void f4 (int,int,int,int) { }
  static void f5 (int,int,int,int,int) { }
  static void f6 (int,int,int,int,int,int) { }
  static void f7 (int,int,int,int,int,int,int) { }
  static void f8 (int,int,int,int,int,int,int,int) { }

  static int rf0 () { return 0; }
  static int rf1 (int) { return 0; }
  static int rf2 (int,int) { return 0; }
  static int rf3 (int,int,int) { return 0; }
  static int rf4 (int,int,int,int) { return 0; }
  static int rf5 (int,int,int,int,int) { return 0; }
  static int rf6 (int,int,int,int,int,int) { return 0; }
  static int rf7 (int,int,int,int,int,int,int) { return 0; }
  static int rf8 (int,int,int,int,int,int,int,int) { return 0; }

#if defined (LUABRIDGE_THROWSPEC)
  static void tf0 () LUABRIDGE_THROWSPEC { }
  static void tf1 (int) LUABRIDGE_THROWSPEC { }
  static void tf2 (int,int) LUABRIDGE_THROWSPEC { }
  static void tf3 (int,int,int) LUABRIDGE_THROWSPEC { }
  static void tf4 (int,int,int,int) LUABRIDGE_THROWSPEC { }
  static void tf5 (int,int,int,int,int) LUABRIDGE_THROWSPEC { }
  static void tf6 (int,int,int,int,int,int) LUABRIDGE_THROWSPEC { }
  static void tf7 (int,int,int,int,int,int,int) LUABRIDGE_THROWSPEC { }
  static void tf8 (int,int,int,int,int,int,int,int) LUABRIDGE_THROWSPEC { }

  static int trf0 () LUABRIDGE_THROWSPEC { return 0; }
  static int trf1 (int) LUABRIDGE_THROWSPEC { return 0; }
  static int trf2 (int,int) LUABRIDGE_THROWSPEC { return 0; }
  static int trf3 (int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  static int trf4 (int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  static int trf5 (int,int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  static int trf6 (int,int,int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  static int trf7 (int,int,int,int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  static int trf8 (int,int,int,int,int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
#endif

  void m0 () { }
  void m1 (int) { }
  void m2 (int,int) { }
  void m3 (int,int,int) { }
  void m4 (int,int,int,int) { }
  void m5 (int,int,int,int,int) { }
  void m6 (int,int,int,int,int,int) { }
  void m7 (int,int,int,int,int,int,int) { }
  void m8 (int,int,int,int,int,int,int,int) { }

  int rm0 () { return 0; }
  int rm1 (int) { return 0; }
  int rm2 (int,int) { return 0; }
  int rm3 (int,int,int) { return 0; }
  int rm4 (int,int,int,int) { return 0; }
  int rm5 (int,int,int,int,int) { return 0; }
  int rm6 (int,int,int,int,int,int) { return 0; }
  int rm7 (int,int,int,int,int,int,int) { return 0; }
  int rm8 (int,int,int,int,int,int,int,int) { return 0; }

  void cm0 () const { }
  void cm1 (int) const { }
  void cm2 (int,int) const { }
  void cm3 (int,int,int) const { }
  void cm4 (int,int,int,int) const { }
  void cm5 (int,int,int,int,int) const { }
  void cm6 (int,int,int,int,int,int) const { }
  void cm7 (int,int,int,int,int,int,int) const { }
  void cm8 (int,int,int,int,int,int,int,int) const { }

  int crm0 () const { return 0; }
  int crm1 (int) const { return 0; }
  int crm2 (int,int) const { return 0; }
  int crm3 (int,int,int) const { return 0; }
  int crm4 (int,int,int,int) const { return 0; }
  int crm5 (int,int,int,int,int) const { return 0; }
  int crm6 (int,int,int,int,int,int) const { return 0; }
  int crm7 (int,int,int,int,int,int,int) const { return 0; }
  int crm8 (int,int,int,int,int,int,int,int) const { return 0; }

#if defined (LUABRIDGE_THROWSPEC)
  void tm0 () LUABRIDGE_THROWSPEC { }
  void tm1 (int) LUABRIDGE_THROWSPEC { }
  void tm2 (int,int) LUABRIDGE_THROWSPEC { }
  void tm3 (int,int,int) LUABRIDGE_THROWSPEC { }
  void tm4 (int,int,int,int) LUABRIDGE_THROWSPEC { }
  void tm5 (int,int,int,int,int) LUABRIDGE_THROWSPEC { }
  void tm6 (int,int,int,int,int,int) LUABRIDGE_THROWSPEC { }
  void tm7 (int,int,int,int,int,int,int) LUABRIDGE_THROWSPEC { }
  void tm8 (int,int,int,int,int,int,int,int) LUABRIDGE_THROWSPEC { }

  int trm0 () LUABRIDGE_THROWSPEC { return 0; }
  int trm1 (int) LUABRIDGE_THROWSPEC { return 0; }
  int trm2 (int,int) LUABRIDGE_THROWSPEC { return 0; }
  int trm3 (int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  int trm4 (int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  int trm5 (int,int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  int trm6 (int,int,int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  int trm7 (int,int,int,int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }
  int trm8 (int,int,int,int,int,int,int,int) LUABRIDGE_THROWSPEC { return 0; }

  void tcm0 () const LUABRIDGE_THROWSPEC { }
  void tcm1 (int) const LUABRIDGE_THROWSPEC { }
  void tcm2 (int,int) const LUABRIDGE_THROWSPEC { }
  void tcm3 (int,int,int) const LUABRIDGE_THROWSPEC { }
  void tcm4 (int,int,int,int) const LUABRIDGE_THROWSPEC { }
  void tcm5 (int,int,int,int,int) const LUABRIDGE_THROWSPEC { }
  void tcm6 (int,int,int,int,int,int) const LUABRIDGE_THROWSPEC { }
  void tcm7 (int,int,int,int,int,int,int) const LUABRIDGE_THROWSPEC { }
  void tcm8 (int,int,int,int,int,int,int,int) const LUABRIDGE_THROWSPEC { }

  int tcrm0 () const LUABRIDGE_THROWSPEC { return 0; }
  int tcrm1 (int) const LUABRIDGE_THROWSPEC { return 0; }
  int tcrm2 (int,int) const LUABRIDGE_THROWSPEC { return 0; }
  int tcrm3 (int,int,int) const LUABRIDGE_THROWSPEC { return 0; }
  int tcrm4 (int,int,int,int) const LUABRIDGE_THROWSPEC { return 0; }
  int tcrm5 (int,int,int,int,int) const LUABRIDGE_THROWSPEC { return 0; }
  int tcrm6 (int,int,int,int,int,int) const LUABRIDGE_THROWSPEC { return 0; }
  int tcrm7 (int,int,int,int,int,int,int) const LUABRIDGE_THROWSPEC { return 0; }
  int tcrm8 (int,int,int,int,int,int,int,int) const LUABRIDGE_THROWSPEC { return 0; }
#endif

  Test1 () : TestBase ("Compile Check")
  {
    using namespace luabridge;

    // Oddly, if there is a compile error in the body of FuncTraits<>::call,
    // Visual Studio does not generate an error! Yet if the entire struct is
    // omitted, a compile error results.
    //
    // This problem seems to appear and disappear...
    //
#if 0
    getGlobalNamespace (m_L)
      .beginNamespace ("test")
        .addFunction ("trf8", &Test1::trf8)
        .beginClass <Test1> ("test1")
          .addFunction ("tcrm8", &Test1::tcrm8)
        .endClass ()
      .endNamespace ();
#endif

#if 1
    getGlobalNamespace (m_L)
      .beginNamespace ("test")
        .addFunction ("f0",   &Test1::f0)
        .addFunction ("f1",   &Test1::f1)
        .addFunction ("f2",   &Test1::f2) 
        .addFunction ("f3",   &Test1::f3)
        .addFunction ("f4",   &Test1::f4)
        .addFunction ("f5",   &Test1::f5)
        .addFunction ("f6",   &Test1::f6)
        .addFunction ("f7",   &Test1::f7)
        .addFunction ("f8",   &Test1::f8)
        .addFunction ("rf0",  &Test1::rf0)
        .addFunction ("rf1",  &Test1::rf1)
        .addFunction ("rf2",  &Test1::rf2)
        .addFunction ("rf3",  &Test1::rf3)
        .addFunction ("rf4",  &Test1::rf4)
        .addFunction ("rf5",  &Test1::rf5)
        .addFunction ("rf6",  &Test1::rf6)
        .addFunction ("rf7",  &Test1::rf7)
        .addFunction ("rf8",  &Test1::rf8)
#if defined (LUABRIDGE_THROWSPEC)
        .addFunction ("tf0",  &Test1::tf0)
        .addFunction ("tf1",  &Test1::tf1)
        .addFunction ("tf2",  &Test1::tf2)
        .addFunction ("tf3",  &Test1::tf3)
        .addFunction ("tf4",  &Test1::tf4)
        .addFunction ("tf5",  &Test1::tf5)
        .addFunction ("tf6",  &Test1::tf6)
        .addFunction ("tf7",  &Test1::tf7)
        .addFunction ("tf8",  &Test1::tf8)
        .addFunction ("trf0", &Test1::trf0)
        .addFunction ("trf1", &Test1::trf1)
        .addFunction ("trf2", &Test1::trf2)
        .addFunction ("trf3", &Test1::trf3)
        .addFunction ("trf4", &Test1::trf4)
        .addFunction ("trf5", &Test1::trf5)
        .addFunction ("trf6", &Test1::trf6)
        .addFunction ("trf7", &Test1::trf7)
        .addFunction ("trf8", &Test1::trf8)
#endif
        .beginClass <Test1> ("test1")
          .addFunction ("m0",   &Test1::m0)
          .addFunction ("m1",   &Test1::m1)
          .addFunction ("m2",   &Test1::m2)
          .addFunction ("m3",   &Test1::m3)
          .addFunction ("m4",   &Test1::m4)
          .addFunction ("m5",   &Test1::m5)
          .addFunction ("m6",   &Test1::m6)
          .addFunction ("m7",   &Test1::m7)
          .addFunction ("m8",   &Test1::m8)
          .addFunction ("rm0",  &Test1::rm0)
          .addFunction ("rm1",  &Test1::rm1)
          .addFunction ("rm2",  &Test1::rm2)
          .addFunction ("rm3",  &Test1::rm3)
          .addFunction ("rm4",  &Test1::rm4)
          .addFunction ("rm5",  &Test1::rm5)
          .addFunction ("rm6",  &Test1::rm6)
          .addFunction ("rm7",  &Test1::rm7)
          .addFunction ("rm8",  &Test1::rm8)
          .addFunction ("cm0",  &Test1::cm0)
          .addFunction ("cm1",  &Test1::cm1)
          .addFunction ("cm2",  &Test1::cm2)
          .addFunction ("cm3",  &Test1::cm3)
          .addFunction ("cm4",  &Test1::cm4)
          .addFunction ("cm5",  &Test1::cm5)
          .addFunction ("cm6",  &Test1::cm6)
          .addFunction ("cm7",  &Test1::cm7)
          .addFunction ("cm8",  &Test1::cm8)
          .addFunction ("crm0", &Test1::crm0)
          .addFunction ("crm1", &Test1::crm1)
          .addFunction ("crm2", &Test1::crm2)
          .addFunction ("crm3", &Test1::crm3)
          .addFunction ("crm4", &Test1::crm4)
          .addFunction ("crm5", &Test1::crm5)
          .addFunction ("crm6", &Test1::crm6)
          .addFunction ("crm7", &Test1::crm7)
          .addFunction ("crm8", &Test1::crm8)
#if defined (LUABRIDGE_THROWSPEC)
          .addFunction ("tm0",   &Test1::tm0)
          .addFunction ("tm1",   &Test1::tm1)
          .addFunction ("tm2",   &Test1::tm2)
          .addFunction ("tm3",   &Test1::tm3)
          .addFunction ("tm4",   &Test1::tm4)
          .addFunction ("tm5",   &Test1::tm5)
          .addFunction ("tm6",   &Test1::tm6)
          .addFunction ("tm7",   &Test1::tm7)
          .addFunction ("tm8",   &Test1::tm8)
          .addFunction ("trm0",  &Test1::trm0)
          .addFunction ("trm1",  &Test1::trm1)
          .addFunction ("trm2",  &Test1::trm2)
          .addFunction ("trm3",  &Test1::trm3)
          .addFunction ("trm4",  &Test1::trm4)
          .addFunction ("trm5",  &Test1::trm5)
          .addFunction ("trm6",  &Test1::trm6)
          .addFunction ("trm7",  &Test1::trm7)
          .addFunction ("trm8",  &Test1::trm8)
          .addFunction ("tcm0",  &Test1::tcm0)
          .addFunction ("tcm1",  &Test1::tcm1)
          .addFunction ("tcm2",  &Test1::tcm2)
          .addFunction ("tcm3",  &Test1::tcm3)
          .addFunction ("tcm4",  &Test1::tcm4)
          .addFunction ("tcm5",  &Test1::tcm5)
          .addFunction ("tcm6",  &Test1::tcm6)
          .addFunction ("tcm7",  &Test1::tcm7)
          .addFunction ("tcm8",  &Test1::tcm8)
          .addFunction ("tcrm0", &Test1::tcrm0)
          .addFunction ("tcrm1", &Test1::tcrm1)
          .addFunction ("tcrm2", &Test1::tcrm2)
          .addFunction ("tcrm3", &Test1::tcrm3)
          .addFunction ("tcrm4", &Test1::tcrm4)
          .addFunction ("tcrm5", &Test1::tcrm5)
          .addFunction ("tcrm6", &Test1::tcrm6)
          .addFunction ("tcrm7", &Test1::tcrm7)
          .addFunction ("tcrm8", &Test1::tcrm8)
#endif
          .endClass ()
      .endNamespace ();
#endif
  }
};

//------------------------------------------------------------------------------

#if 0
class Test1 : public TestBase
{
public:
  Test1 () : TestBase ("Set_get")
  {
    using namespace luabridge;

    getGlobalNamespace (m_L)
      .beginClass <Set_get> ("Set_get")
        .addConstructor<void(*)(void)> ()
        .addFunction ("set", &Set_get::set)
        .addFunction ("get", &Set_get::get)
      .endClass ();
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

  char const* getMainChunk ()
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
};

//==============================================================================

class Test2 : public TestBase
{
public:
  Test2 () : TestBase ("ProfileHierarchy")
  {
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

  char const* getMainChunk ()
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
};
#endif

//==============================================================================

// This is the original set of LuaBridge tests
//
class Test3 : public TestBase
{
public:
  Test3 () : TestBase ("Original Tests")
  {
    using namespace luabridge;

    getGlobalNamespace (m_L)
      .addFunction ("testSucceeded", &testSucceeded)
      .addFunction ("testAFnCalled", &testAFnCalled)
      .addFunction ("testBFnCalled", &testBFnCalled)
      .addFunction ("testRetInt", &testRetInt)
      .addFunction ("testRetFloat", &testRetFloat)
      .addFunction ("testRetConstCharPtr", &testRetConstCharPtr)
      .addFunction ("testRetStdString", &testRetStdString)
      .addFunction ("testParamInt", &testParamInt)
      .addFunction ("testParamBool", &testParamBool)
      .addFunction ("testParamFloat", &testParamFloat)
      .addFunction ("testParamConstCharPtr", &testParamConstCharPtr)
      .addFunction ("testParamStdString", &testParamStdString)
      .addFunction ("testParamStdStringRef", &testParamStdStringRef)
      .beginClass <A> ("A")
        .addConstructor <void (*) (const string &), RefCountedPtr <A> > ()
        .addFunction ("testVirtual", &A::testVirtual)
        .addFunction ("getName", &A::getName)
        .addFunction ("testSucceeded", &A::testSucceeded)
        .addFunction ("__add", &A::operator+)
        .addData ("testProp", &A::testProp)
        .addProperty ("testProp2", &A::testPropGet, &A::testPropSet)
        .addStaticFunction ("testStatic", &A::testStatic)
        .addStaticData ("testStaticProp", &A::testStaticProp)
        .addStaticProperty ("testStaticProp2", &A::testStaticPropGet, &A::testStaticPropSet)
      .endClass ()
      .deriveClass <B, A> ("B")
        .addConstructor <void (*) (const string &), RefCountedPtr <B> > ()
        .addStaticFunction ("testStatic2", &B::testStatic2)
      .endClass ()
      .addFunction ("testParamAPtr", &testParamAPtr)
      .addFunction ("testParamAPtrConst", &testParamAPtrConst)
      .addFunction ("testParamConstAPtr", &testParamConstAPtr)
      .addFunction ("testParamSharedPtrA", &testParamSharedPtrA)
      .addFunction ("testRetSharedPtrA", &testRetSharedPtrA)
      .addFunction ("testRetSharedPtrConstA", &testRetSharedPtrConstA)
    ;
  }

private:
  typedef string string;

  static bool g_success;

  static bool testSucceeded ()
  {
    bool b = g_success;
    g_success = false;
    return b;
  }

  typedef int fn_type;
  enum {
    FN_CTOR,
    FN_DTOR,
    FN_STATIC,
    FN_VIRTUAL,
    FN_PROPGET,
    FN_PROPSET,
    FN_STATIC_PROPGET,
    FN_STATIC_PROPSET,
    FN_OPERATOR,
    NUM_FN_TYPES
  };

  struct fn_called {
    bool called [NUM_FN_TYPES];
    fn_called () { memset(called, 0, NUM_FN_TYPES * sizeof(bool)); }
  };

  static fn_called A_functions;
  static fn_called B_functions;

  static bool testAFnCalled (fn_type f)
  {
    bool b = A_functions.called[f];
    A_functions.called [f] = false;
    return b;
  }

  static bool testBFnCalled (fn_type f)
  {
    bool b = B_functions.called[f];
    B_functions.called [f] = false;
    return b;
  }

  class A
  {
  protected:
    string name;
    mutable bool success;
  public:
    A (string const& name_) : name (name_), success (false), testProp (47)
    {
      A_functions.called [FN_CTOR] = true;
    }
    virtual ~A ()
    {
      A_functions.called [FN_DTOR] = true;
    }

    virtual void testVirtual ()
    {
      A_functions.called [FN_VIRTUAL] = true;
    }

    const char * getName () const
    {
      return name.c_str();
    }

    void setSuccess () const
    {
      success = true;
    }

    bool testSucceeded () const
    {
      bool b = success;
      success = false;
      return b;
    }

    static void testStatic ()
    {
      A_functions.called [FN_STATIC] = true;
    }

    int testProp;
    int testPropGet () const
    {
      A_functions.called [FN_PROPGET] = true;
      return testProp;
    }
    void testPropSet (int x)
    {
      A_functions.called [FN_PROPSET] = true;
      testProp = x;
    }

    static int testStaticProp;
    static int testStaticPropGet ()
    {
      A_functions.called [FN_STATIC_PROPGET] = true;
      return testStaticProp;
    }
    static void testStaticPropSet (int x)
    {
      A_functions.called [FN_STATIC_PROPSET] = true;
      testStaticProp = x;
    }
  
    RefCountedPtr <A> operator + (A const& other)
    {
      A_functions.called [FN_OPERATOR] = true;
      return new A (name + " + " + other.name);
    }
  };

  class B: public A
  {
  public:
    explicit B (string const& name_) : A (name_)
    {
      B_functions.called [FN_CTOR] = true;
    }

    virtual ~B ()
    {
      B_functions.called [FN_DTOR] = true;
    }

    virtual void testVirtual ()
    {
      B_functions.called [FN_VIRTUAL] = true;
    }

    static void testStatic2 ()
    {
      B_functions.called [FN_STATIC] = true;
    }
  };

  /*
   * Test functions
   */

  static int testRetInt ()
  {
    return 47;
  }

  static float testRetFloat ()
  {
    return 47.0f;
  }

  static char const* testRetConstCharPtr ()
  {
    return "Hello, world";
  }

  static string testRetStdString ()
  {
    static string ret ("Hello, world");
    return ret;
  }

  static void testParamInt (int a)
  {
    g_success = (a == 47);
  }

  static void testParamBool (bool b)
  {
    g_success = b;
  }

  static void testParamFloat (float f)
  {
    g_success = (f == 47.0f);
  }

  static void testParamConstCharPtr (char const* str)
  {
    g_success = !strcmp (str, "Hello, world");
  }

  static void testParamStdString (string str)
  {
    g_success = !strcmp (str.c_str(), "Hello, world");
  }

  static void testParamStdStringRef (const string &str)
  {
    g_success = !strcmp (str.c_str(), "Hello, world");
  }

  static void testParamAPtr (A * a)
  {
    a->setSuccess();
  }

  static void testParamAPtrConst (A * const a)
  {
    a->setSuccess();
  }

  static void testParamConstAPtr (const A * a)
  {
    a->setSuccess();
  }

  static void testParamSharedPtrA (RefCountedPtr <A> a)
  {
    a->setSuccess();
  }

  static RefCountedPtr <A> testRetSharedPtrA ()
  {
    static RefCountedPtr <A> sp_A (new A("from C"));
    return sp_A;
  }

  static RefCountedPtr <A const> testRetSharedPtrConstA ()
  {
    static RefCountedPtr <A> sp_A (new A("const A"));
    return sp_A;
  }

  char const* getMainChunk ()
  {
    return BinaryData::UnitTests_lua;
  }
};

bool Test3::g_success = true;
int Test3::A::testStaticProp = 47;
Test3::fn_called Test3::A_functions;
Test3::fn_called Test3::B_functions;

//------------------------------------------------------------------------------

/**
    Tests of LuaRef
*/
class Test4 : public TestBase
{
private:
  struct A
  {
    A ()
    {
    }

    virtual ~A ()
    {
    }

    virtual char const* name () const
    {
      return "A";
    }
  };

public:
  Test4 () : TestBase ("LuaRef")
  {
    using namespace luabridge;

    getGlobalNamespace (m_L)
      .beginClass <A> ("A")
        .addConstructor <void (*) (void)> ()
      .endClass ()
      ;
  }

  static int cfunc (lua_State* L)
  {
    int nret = 0;
    lua_pushstring (L, "cfunc error");
    lua_error (L);
    return nret;
  }

  int runNativeCode ()
  {
    int result = 0;

    using namespace std;
    using namespace luabridge;

    // Global function
    LuaRef eq = LuaRef::getGlobal (m_L, "areEqual");
    ASSURE (eq.isFunction ());
    ASSURE (eq (1, 1).cast <bool> ());
    ASSURE (!eq (1, 2).cast <bool> ());
    ASSURE (eq (3., 3.).cast <bool> ());
    ASSURE (!eq (3., 4.).cast <bool> ());
    ASSURE (eq ("a", "a").cast <bool> ());
    ASSURE (!eq ("a", "b").cast <bool> ());
    ASSURE (!eq (1, Nil ()).cast <bool> ());
    ASSURE (eq (Nil (), Nil ()).cast <bool> ());

    // Default constructor
    {
      LuaRef v (m_L);
      ASSURE (v.isNil ());
    }
    {
      LuaRef v (m_L);
      ASSURE (v.isNil ());
    }

    // Conversion constructors
    {
      LuaRef v (m_L, Nil ());
      ASSURE (v.isNil ());
    }
    {
      LuaRef v (m_L, 1);
      ASSURE (eq (v, 1.).cast <bool> ());
      ASSURE (!eq (v, 2.).cast <bool> ());
    }
    {
      LuaRef v (m_L, "a");
      ASSURE (eq (v, "a").cast <bool> ());
    }


    LuaRef v (m_L);
    v = 1;                            // assign integer
    ASSURE (v.cast <int> () == 1);
    ASSURE (eq (v, 1).cast <bool> ());

    // tables
    LuaRef t (LuaRef::newTable (m_L));
    ASSURE (t.isTable ());            // make sure its a table
    t [1] = 1;
    ASSURE (eq (t[1], 1).cast <bool> ());
    t [1] = Nil ();
    ASSURE (LuaRef (t [1]).isNil ());

    t = Nil ();

    try
    {
      LuaRef t (LuaRef::newTable (m_L));

      t[1] = 2;
      eq (t[1], 2);
      t[1] = 3;
      eq (3, t[1]);
      t[2] = "hey";
      eq (t[2], "hey");
    }
    catch (exception const& e)
    {
      cout << e.what () << endl;
    }

    //--------------------------------------------------------------------------

    // Tests for C++ objects registered to Lua

    {
      // Get constructor for A.
      LuaRef c = LuaRef::getGlobal (state (), "A");

      LuaRef result = c ();
      int type = result.type ();
      type;
      A* a = result;
      ASSURE (eq (a->name (), "A").cast <bool> ());
    }

    //--------------------------------------------------------------------------

    return result;
  }

private:
  char const* getMainChunk ()
  {
    return "\
      function assert (expr) \n\
        if (not expr) then error('assert failed', 2) end \n\
      end \n\
      function areEqual (v1, v2) \n\
        return v1 == v2 \n\
      end \n\
           ";
  }
};

//==============================================================================
/**
    Tests of LuaRef with registered functions.
*/
class Test5 : public TestBase
{
public:
  static void cf (luabridge::LuaRef v)
  {
    assert (int(v)==42);
  };

public:
  Test5 () : TestBase ("LuaRef as arguments")
  {
    using namespace luabridge;

    getGlobalNamespace (m_L)
      .addFunction ("cf", &Test5::cf)
      ;
  }

  int runNativeCode ()
  {
    int result = 0;

    using namespace std;
    using namespace luabridge;

    LuaRef f = getGlobal (m_L, "f");

    try
    {
      LuaRef t = f ();
      int type = t.type ();
      ASSURE (t.isTable ());
      ASSURE (t [1] == 1);
      ASSURE (t [1] > 0);
      ASSURE (t [1] >= -1);
      ASSURE (t [1] < 2);
      ASSURE (t [1] <= 3);
      ASSURE (t ['a'] == 'a');
    }
    catch (LuaException& e)
    {
      cerr << "Fail: " << e.what () << endl;
    }

    return result;
  }

private:
  char const* getMainChunk ()
  {
    return
      "\
      function f () \
        cf (42) \
        local t = { } \
        t [1] = 1 \
        t ['a'] = 'a' \
        return t \
      end \
      ";
  }
};

//==============================================================================
/**
    Tests of LuaRef iterators.
*/
class Test6 : public TestBase
{
public:
  Test6 () : TestBase ("LuaRef iterators")
  {
  }

  int runNativeCode ()
  {
    int result = 0;

    using namespace std;
    using namespace luabridge;

    LuaRef t = newTable (m_L);

    t [1] = "hello";
    t [2] = "world";
    t ["pet"] = "cat";

    // Iterate using C API
    t.push (m_L);
    push (m_L, Nil ());
    while (lua_next (m_L, -2))
    {
      LuaRef key = LuaRef::fromStack (m_L, -2);
      LuaRef val = LuaRef::fromStack (m_L, -1);
      cout << "[" << key << "] = " << val << endl;
      lua_pop (m_L, 1);
    }
    cout << endl;

    // Iterate using iterator
    for (Iterator iter (t); !iter.isNil (); ++iter)
    {
      LuaRef key = iter.key ();
      LuaRef val = *iter;
      cout << "[" << key << "] = " << val << endl;
    }

    return result;
  }
};

//==============================================================================

int main (int, char **)
{
  using namespace std;

  Test1 () ();
  Test3 () ();
  Test4 () ();
  Test5 () ();
  Test6 () ();

  runSpeedTests ();

  return 0;
}

// This code snippet fails to compile in vs2010
#if 0
struct S {
  template <class T> inline operator T () const { return T (); }
};

int main () {
  S () || false;
  return 0;
}
#endif
