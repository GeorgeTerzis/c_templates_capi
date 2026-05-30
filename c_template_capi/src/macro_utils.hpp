#pragma once

#include "./utils.hpp"
#include <string>
#include <vector>

#define CONCAT_STR "CONCAT_"

const std::string str_defined(std::string def) {
    return "defined(" + def + ")";
}

const std::string str_if(std::string expr) {
    return "#if " + expr;
}

const std::string str_else() {
    return "#else\n";
}

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

const std::string str_endif() {
    return "#endif\n";
}

const std::string str_ifndef_error(const std::string def, const std::string message) {
    return str_ifndef(def) + str_error(message);
}

const std::string str_ifndef_warn(const std::string def, const std::string message) {
    return str_ifndef(def) + str_warn(message);
}

const std::string str_ifdef_error(const std::string def, const std::string message) {
    return str_ifdef(def) + str_error(message);
}

const std::string str_ifdef_warn(const std::string def, const std::string message) {
    return str_ifdef(def) + str_warn(message);
}

const std::string str_typedef(const std::string& name, const std::string& tdef_name) {
    return "typedef " + name + " " + tdef_name + ";";
}

const std::string str_include(const std::string file) {
    return "#include \"" + file + "\"";
}

const std::string str_concat_macro(const std::string prefix,
                                   const std::vector<std::string> types) {
    if (types.empty())
        return prefix; // edge case: no type params
    auto r =
        std::string(CONCAT_STR + std::to_string(types.size() - 1) + "(" + prefix + ",");
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

const std::string str_record_end() {
    return "};";
}

const std::string str_comment(std::string comment) {
    return "/* " + comment + " */\n";
}

const std::string str_func_macro(std::string name, std::vector<std::string>& args) {
    return str_def(name) + "(" + condence_string_with_pattern(args, "", "", ",") + ")";
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

const std::string str_fptr_name(bool is_block, std::string name, std::string arguments) {
    std::string c = (is_block) ? "^" : "*";
    return "(" + std::string(c) + name + ")" + "(" + arguments + ")";
}
std::string str_sub(const std::string& file,
                    const unsigned long long start,
                    const unsigned long long end) {
    return file.substr(start, end - start);
}

std::string str_ifdef_def_else(const std::string ifdef,
                               const std::string def,
                               const std::string def_val) {
    return str_ifdef(ifdef) + "  " + str_def(def) + def_val + "\n" + str_else();
}
std::string
str_ifdef_def(const std::string ifdef, const std::string def, const std::string def_val) {
    return str_ifdef(ifdef) + "  " + str_def(def) + def_val + "\n" + str_endif();
}

std::string str_gen_concat_macros(int count) {
    const std::string concat1 = CONCAT_STR;
    std::string out = "";

    std::string base_inner = "#define " + concat1 + "0_(prefix, T0) prefix##_##T0\n";
    std::string base_outer =
        "#define " + concat1 + "0(prefix, T0) " + concat1 + "0_(prefix, T0)\n";
    out += base_inner + base_outer + "\n";

    for (int i = 1; i < count; ++i) {
        std::string part = concat1 + std::to_string(i);
        std::string part_prev = concat1 + std::to_string(i - 1);

        // build arg list: (prefix, T0, T1, ..., Ti)
        std::string args = "prefix, ";
        for (int j = 0; j <= i; ++j) {
            args += "T" + std::to_string(j);
            if (j != i)
                args += ", ";
        }
        std::string arg_part = "(" + args + ")";

        std::string tail_args = "prefix##_##T0, ";
        for (int j = 1; j <= i; ++j) {
            tail_args += "T" + std::to_string(j);
            if (j != i)
                tail_args += ", ";
        }

        std::string inner =
            "#define " + part + "_(" + args + ") " + part_prev + "_(" + tail_args + ")\n";

        std::string outer =
            "#define " + part + "(" + args + ") " + part + "_(" + args + ")\n";

        out += inner + outer + "\n";
    }
    return out;
}
