
#include <iostream>

#include "A.hpp"
#include <LuaBridge/LuaBridge.h>

//#include "A.cpp"

void printA(A *a)
{
  std::cout << a->getName() << std::endl;
}

int main(int argc, char *argv[]) {
  int iarg;
  lua_State *L = luaL_newstate();

  luaL_openlibs(L);
  A::register_lua(L);

  {
    using namespace luabridge;
    getGlobalNamespace (L)
      .addFunction ("printA", &printA);
  }

  int s = luaL_dostring(L,
    "a = test.A()\n"
    "printA(a)\n"
    );
  if(s != 0) {
    std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
    lua_pop(L, 1);
  }

  lua_close(L);

  return 0;
}

