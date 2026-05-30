#pragma once

#include <clang-c/Index.h>
#include <string>

std::string clang_get_kind_str(CXCursor cursor){
  CXString cspelling= clang_getCursorKindSpelling(clang_getCursorKind(cursor));
  std::string spelling = clang_getCString(cspelling);
  clang_disposeString(cspelling);
  return spelling;
}
std::string clang_get_type_str(CXCursor cursor){
  CXString cspelling = clang_getTypeSpelling(clang_getCursorType(cursor));
  std::string spelling = clang_getCString(cspelling);
  clang_disposeString(cspelling);
  return spelling;
}
std::string clang_get_type_str(CXType ctype){
  CXString cspelling = clang_getTypeSpelling(ctype);
  std::string spelling = clang_getCString(cspelling);
  clang_disposeString(cspelling);
  return spelling;
}

std::string clang_get_name_str(CXCursor cursor){
  CXString cspelling= clang_getCursorSpelling(cursor);
  std::string spelling= clang_getCString(cspelling);
  clang_disposeString(cspelling);
  return spelling;
}


void print_cursor(CXCursor cursor){
  printf("Cursor Kind: \"%s\" Spelling: \"%s\"\n", clang_get_kind_str(cursor).c_str(),clang_get_name_str(cursor).c_str());
}
