#include "A.hpp"

#include <iostream>
#include <string>

#include <LuaBridge/LuaBridge.h>

std::string A::getName() const { return std::string("A"); }

void A::register_lua(lua_State *L)
{
  using namespace luabridge;

  getGlobalNamespace (L)
    .beginNamespace ("test")
      .beginClass <A> ("A")
        .addConstructor<void(*)(void)>()
        .addProperty("name", &A::getName)
      .endClass ()
    .endNamespace ();
}
