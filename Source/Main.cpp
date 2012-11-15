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

#include "BinaryData.h"
#include "SpeedTests.h"

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

//------------------------------------------------------------------------------

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
  typedef std::string string;

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

class Test4 : public TestBase
{
public:
  Test4 () : TestBase ("LuaRef and LuaVal")
  {
  }

  int runNativeCode ()
  {
    int result = 0;

    using namespace luabridge;

    LuaRef func1 (m_L, "func1");

    double retval = func1 ();

    std::cout << "retval = " << retval << std::endl;

    return result;
  }

private:
  char const* getMainChunk ()
  {
    return "\
      function func1 () \
        return 42 \
      end \
           ";
  }
};

//==============================================================================

int main (int, char **)
{
  using namespace std;

  Test1 () ();
  Test2 () ();
  Test3 () ();
  Test4 () ();

  runSpeedTests ();

  return 0;
}
