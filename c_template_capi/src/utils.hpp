#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <fstream>

const std::vector<std::string> split_string(const std::string& input, const char limit) {
    std::vector<std::string> lines;
    std::istringstream ss(input);
    std::string line;

    while (std::getline(ss, line, limit)) {
        lines.push_back(line);
    }

    return lines;
}

const std::vector<std::string> split_string_into_lines(const std::string& input) {
    std::vector<std::string> lines;
    std::istringstream ss(input);
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    return lines;
}

std::string string_strip_whitespace(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == std::string::npos) {
        // String is all whitespace
        return "";
    }

    return str.substr(first, last - first + 1);
}

const std::string condence_string_with_pattern(const std::vector<std::string>& input, const std::string prefix, const std::string subfix, const std::string separator){
 std::string out;
 out.reserve(input.size() + input.size()*(prefix.size()+subfix.size()+(separator.size() - 1)));

 for(uint64_t i = 0; i < input.size() - 1; i++){
   out.append(prefix + input[i]);
   out.append(subfix + separator);
 }
 out.append(prefix + input[input.size() - 1]);
  
 return out;
}


std::string create_macro(std::string macro_name, std::string macro_args, std::string macro_body){
  return std::string( "#define " + macro_name + "(" + macro_args + ")" + macro_body + "\\\n" );
}

std::string create_macro(const std::string macro_name, const std::vector<std::string> macro_args, const std::string macro_body){
  std::string args;
  for(uint64_t i = 0; i < macro_args.size(); i++){
    args.append( macro_args[i]);
    if(i != macro_args.size() - 1) args.append(",");
  }

  return std::string( "#define " + macro_name + "(" + args + ")" + macro_body + "\\\n" );
}

// struct record_meta{
//   std::string original_name; 
//   std::string final_name;
//   std::vector<std::pair<std::string, std::string>> fields;
//   std::vector<std::string> ttypes;
//   bool is_union;
// };

std::string read_file_into_string(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return ""; // Return an empty string to indicate failure
    }

    std::ostringstream content;
    content << inputFile.rdbuf();
    inputFile.close();

    return content.str();
}

const std::string str_erase(std::string input, std::string remove){
  auto pos = input.find(remove);
  if(pos != std::string::npos){
    input.erase(pos,remove.length());
  }
  return input;
}

const std::string str_erase_all(std::string input, std::string remove){
  auto pos = input.find(remove);
  while(pos != std::string::npos){
    input.erase(pos,remove.length());
    pos = input.find(remove);
  }
  return input;
}


