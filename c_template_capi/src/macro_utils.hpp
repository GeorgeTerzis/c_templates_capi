#pragma once

#include "./utils.hpp"
#include <string>
#include <vector>

#define CONCAT_STR "CONCAT_"

const std::string str_defined(std::string def) {
  return "defined(" + def + ")";
}

const std::string str_if(std::string expr) { return "#if " + expr; }

const std::string str_else() { return "#else\n"; }

const std::string str_error(std::string message) {
  return "  #error \"" + message + "\"\n";
}

const std::string str_warn(std::string message) {
  return "  #warning \"" + message + "\"\n";
}

const std::string str_ifndef(const std::string def) {
  return "\n#if !" + str_defined(def) + "\n";
}

const std::string str_ifdef(const std::string def) {
  return "\n#if " + str_defined(def) + "\n";
}

const std::string str_endif() { return "#endif\n"; }

const std::string str_ifndef_error(const std::string def,
                                   const std::string message) {
  return str_ifndef(def) + str_error(message);
}

const std::string str_ifndef_warn(const std::string def,
                                  const std::string message) {
  return str_ifndef(def) + str_warn(message);
}

const std::string str_ifdef_error(const std::string def,
                                  const std::string message) {
  return str_ifdef(def) + str_error(message);
}

const std::string str_ifdef_warn(const std::string def,
                                 const std::string message) {
  return str_ifdef(def) + str_warn(message);
}

const std::string str_typedef(const std::string &name,
                              const std::string &tdef_name) {
  return "typedef " + name + " " + tdef_name + ";";
}

const std::string str_include(const std::string file) {
  return "#include \"" + file + "\"";
}

const std::string str_concat_macro(const std::string prefix,
                                   const std::vector<std::string> types) {
  auto r = std::string(CONCAT_STR+ std::to_string(types.size()) + "(" +
                       prefix + ",");
  const std::string args = condence_string_with_pattern(types, "", "", ",");
  r += args + ")";
  return r;
}

const std::string str_record_begin(std::string name, bool is_union) {
  return ((is_union) ? "union " : "struct ") + name + "{\n";
}

const std::string str_def(std::string def_name) {
  return "#define " + def_name + " ";
}

const std::string str_udef(std::string def_name) {
  return str_ifdef(def_name) + "  #undef " + def_name + "\n" + str_endif();
}

const std::string str_record_end() { return "};"; }

const std::string str_comment(std::string comment) {
  return "/* " + comment + " */\n";
}

const std::string str_func_macro(std::string name,
                                 std::vector<std::string> &args) {
  return str_def(name) + "(" + condence_string_with_pattern(args, "", "", ",") +
         ")";
}

const std::string str_remove_pointers(std::string type) {
  return str_erase_all(type, "*");
}

const std::string str_remove_brackets(std::string type) {
  // make this a function
  type = str_erase_all(type, "[");
  type = str_erase_all(type, "]");
  return type;
}

const std::string str_remove_const(std::string type) {
  return str_erase(type, "const");
}

const std::string str_remove_struct(std::string type) {
  return str_erase(type, "struct");
}

const std::string str_fptr_name(bool is_block, std::string name,
                                std::string arguments) {
  std::string c = (is_block) ? "^" : "*";
  return "(" + std::string(c) + name + ")" + "(" + arguments + ")";
}
std::string str_sub(const std::string &file, const unsigned long long start,
                    const unsigned long long end) {
  return file.substr(start, end - start);
}

const std::string str_ifdef_def_else(const std::string ifdef,
                                     const std::string def,
                                     const std::string def_val) {
  return str_ifdef(ifdef) + "  " + str_def(def) + def_val + "\n" + str_else();
}
const std::string str_ifdef_def(const std::string ifdef, const std::string def,
                                const std::string def_val) {
  return str_ifdef(ifdef) + "  " + str_def(def) + def_val + "\n" + str_endif();
}

const std::string str_gen_concat_macros(int count) {
  const std::string concat1 = CONCAT_STR;
  std::string out = "";
  for (int i = 0; i < count; ++i) {
    std::string part1 = concat1 + std::to_string(i);
    std::string args = "";
    for (int j = 0; j <= i; ++j) {
      args += " T" + std::to_string(j);
      if (j != i)
        args += ",";
    }

    std::string arg_part = "(prefix, " + args + ")";
    std::string macro_part = "";
    for (int j = 0; j <= i; ++j) {
      if (j == 0)
        macro_part += "##";

      macro_part += "T" + std::to_string(j);

      if (j != i)
        macro_part += "##_";
    }

    std::string main_macro =
        "#define " + part1 + arg_part + " " + part1 + "_" + arg_part;
    std::string sec_macro =
        "#define " + part1 + "_" + arg_part + " prefix##_" + macro_part;

    out += sec_macro + "\n";
    out += main_macro + "\n";
    out += "\n";
  }

  return out;
}

