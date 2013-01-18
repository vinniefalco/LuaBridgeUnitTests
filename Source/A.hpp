#ifndef A_H
#define A_H

#include <string>

#include "LuaUnityBuild/LuaUnityBuild.h"

struct A {
  std::string getName() const;
  static void register_lua(lua_State *L);
};

#endif
