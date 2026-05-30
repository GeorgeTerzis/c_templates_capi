#pragma once

#include <regex>
#include <iostream>

typedef enum{
  TEMPLATE_INPUT,
  TEMPLATE_ENTITY,
  TEMPLATE_AUXILIARY,
  NOT_TEMPLATE,
} template_type;

inline
const std::regex template_input_r(){
  const static std::regex r = std::regex(R"(\w+_ti\b)");
  return r;
}
inline
const bool is_ti(std::string name){
  return regex_search(name,template_input_r());
}



inline
const std::regex template_entity_r(){
  const static std::regex r = std::regex(R"(\w+_te\b)");
  return r;
}
inline 
const bool is_te(std::string name){
  return regex_search(name,template_entity_r());
}



inline
const std::regex template_auxilary_r(){
  const static std::regex r = std::regex(R"(\w+_ta\b)");
  return r;
}
inline
const bool is_ta(std::string name){
  return regex_search(name,template_auxilary_r());
}



inline
const template_type is_ttype(std::string name){
  if(is_ti(name)) return TEMPLATE_INPUT;
  else if(is_te(name)) return TEMPLATE_ENTITY;
  else if(is_ta(name)) return TEMPLATE_AUXILIARY;

  return NOT_TEMPLATE;
}

inline
void ctem_bools_print(template_type c){
  if(c == NOT_TEMPLATE) std::cout <<  "------not template" << "\n";
  if(c == TEMPLATE_INPUT) std::cout <<  "------template input" << "\n";
  if(c == TEMPLATE_ENTITY) std::cout <<  "------template entity" << "\n";
  if(c == TEMPLATE_AUXILIARY) std::cout <<  "------template auxiliary" << "\n";
}

