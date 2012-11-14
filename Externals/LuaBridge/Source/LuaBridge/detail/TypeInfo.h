//------------------------------------------------------------------------------
/*
  https://github.com/vinniefalco/LuaBridge
  
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

#ifndef LUABRIDGE_TYPEINFO_HEADER
#define LUABRIDGE_TYPEINFO_HEADER

/* THIS CLASS SEEMS TO BE UNUSED
*/
/** Type extractor.

    These templates extract information about types.
*/
template <typename T>
struct TypeInfo
{
  typedef T Type;
  static bool const is_const     = false;
  static bool const is_pointer   = false;
  static bool const is_reference = false;
};

template <typename T>
struct TypeInfo <T const>
{
  typedef T Type;
  static bool const is_const     = true;
  static bool const is_pointer   = false;
  static bool const is_reference = false;
};

template <typename T>
struct TypeInfo <T*>
{
  typedef T Type;
  static bool const is_const     = false;
  static bool const is_pointer   = true;
  static bool const is_reference = false;
};

template <typename T>
struct TypeInfo <T const*>
{
  typedef T Type;
  static bool const is_const     = true;
  static bool const is_pointer   = true;
  static bool const is_reference = false;
};

template <typename T>
struct TypeInfo <T&>
{
  typedef T Type;
  static bool const is_const     = false;
  static bool const is_pointer   = false;
  static bool const is_reference = true;
};

template <typename T>
struct TypeInfo <T const&>
{
  typedef T Type;
  static bool const is_const     = true;
  static bool const is_pointer   = false;
  static bool const is_reference = true;
};

#endif