#pragma once

#include <map>
#include <optional>
#include <set>
#include <string>

#include <clang-c/Index.h>

#include "macro_utils.hpp"
#include "ttype_regex.hpp"

struct type_bools {
    bool is_ptr = false;
    bool is_block = false;
    bool is_fptr = false;
    bool is_fproto = false;
    bool is_fnproto = false;

    bool is_typedef = false;

    bool is_carray = false;
    bool is_varray = false;
    bool is_iarray = false;

    bool is_builtin = false;
    bool is_complex = false;
    bool is_vector = false;
    bool is_auto = false;

    bool is_atomic = false;

    bool is_elaborated = false;
    bool is_record = false;
    bool is_struct = false;
    bool is_union = false;

    bool is_enum = false;
    bool is_unexposed = false;
};

void type_bools_analysis_rec(CXType ctype, type_bools& v) {
    std::optional<CXType> rctype;

    switch (ctype.kind) {
    case CXType_Unexposed:
        v.is_unexposed = true;
        break;
    case CXType_BlockPointer:
        v.is_fptr = false;
        v.is_block = true;
        break;
    case CXType_FunctionProto:
        if (v.is_ptr) {
            v.is_ptr = false;
            v.is_fptr = true;
        }
        v.is_fproto = true;
        break;
    case CXType_Pointer:
        v.is_ptr = true;
        rctype = clang_getPointeeType(ctype);
        break;
    case CXType_ConstantArray:
        v.is_carray = true;
        rctype = clang_getArrayElementType(ctype);
        break;
    case CXType_VariableArray:
        v.is_varray = true;
        rctype = clang_getArrayElementType(ctype);
        break;
    case CXType_IncompleteArray:
        v.is_iarray = true;
        rctype = clang_getArrayElementType(ctype);
        break;
    case CXType_Enum:
        v.is_enum = true;
        break;
    case CXType_Record:
        v.is_record = true;
        break;
    case CXType_Complex:
        v.is_complex = true;
        break;
    case CXType_FunctionNoProto:
        v.is_fnproto = true;
        break;
    case CXType_Vector:
        v.is_vector = true;
        break;
    case CXType_Elaborated:
        v.is_elaborated = true;
        rctype = clang_Type_getNamedType(ctype);
        break;
    case CXType_Typedef:
        v.is_typedef = true;
        rctype = clang_getCanonicalType(ctype);
        break;
    case CXType_Atomic:
        v.is_atomic = true;
        break;
    default:
        if (ctype.kind >= CXType_FirstBuiltin && ctype.kind <= CXType_LastBuiltin)
            v.is_builtin = true;
        break;
    }

    if (rctype.has_value())
        type_bools_analysis_rec(*rctype, v);
}

inline type_bools type_bools_analysis(CXType ctype) {
    type_bools vb;
    type_bools_analysis_rec(ctype, vb);
    return vb;
}

void type_bools_print(const type_bools& vb) {
    if (vb.is_complex)
        std::cerr << "------is_complex" << "\n";
    if (vb.is_ptr)
        std::cerr << "------is_ptr" << "\n";
    if (vb.is_fproto)
        std::cerr << "------is_fproto" << "\n";
    if (vb.is_block)
        std::cerr << "------is_block" << "\n";
    if (vb.is_fptr)
        std::cerr << "------is_fptr" << "\n";
    if (vb.is_fnproto)
        std::cerr << "------is_fnproto" << "\n";
    if (vb.is_typedef)
        std::cerr << "------is_typedef" << "\n";
    if (vb.is_carray)
        std::cerr << "------is_carray" << "\n";
    if (vb.is_varray)
        std::cerr << "------is_varray" << "\n";
    if (vb.is_iarray)
        std::cerr << "------is_iarray" << "\n";
    if (vb.is_vector)
        std::cerr << "------is_vector" << "\n";
    if (vb.is_auto)
        std::cerr << "------is_auto" << "\n";
    if (vb.is_elaborated)
        std::cerr << "------is_elaborated" << "\n";
    if (vb.is_builtin)
        std::cerr << "------is_builtin" << "\n";
    if (vb.is_record)
        std::cerr << "------is_record" << "\n";
    if (vb.is_struct)
        std::cerr << "------is_struct" << "\n";
    if (vb.is_union)
        std::cerr << "------is_union" << "\n";
    if (vb.is_enum)
        std::cerr << "------is_enum" << "\n";
    if (vb.is_atomic)
        std::cerr << "------is_atomic" << "\n";
}

struct source_range {
    unsigned int start = 0;
    unsigned int end = 0;

    source_range() {}

    source_range(CXCursor cursor) {
        const CXSourceRange srange = clang_getCursorExtent(cursor);
        const CXSourceLocation cstart = clang_getRangeStart(srange);
        const CXSourceLocation cend = clang_getRangeEnd(srange);

        clang_getFileLocation(cstart, NULL, NULL, NULL, &this->start);
        clang_getFileLocation(cend, NULL, NULL, NULL, &this->end);
    }
};

struct type_data {
    std::string type;
    template_type tt;
    type_bools bools;
    std::set<std::string> nested_template_types;
};

struct var_data {
    type_data var_type_data;
    std::string name;
};

struct record_data {
    type_data rec_type_data;
    std::vector<var_data> fields;

    source_range loc;

    const std::string name_() const {
        return rec_type_data.type;
    }

    const std::set<std::string> all_ttypes() const {
        std::set<std::string> ttypes;
        for (const auto& f : fields) {
            ttypes.insert(f.var_type_data.nested_template_types.begin(),
                          f.var_type_data.nested_template_types.end());
        }
        return ttypes;
    }

    const std::string str(const std::string& file) const {
        const std::string body = str_sub(file, loc.start, loc.end);
        return str_typedef(body, rec_type_data.type);
    }
};

struct function_data {
    type_data func_type_data;
    std::string name;

    std::vector<var_data> args;
    template_type tt;
    source_range loc;

    const std::string name_() {
        return name;
    }

    const std::string type_() {
        return func_type_data.type;
    }

    const std::set<std::string> all_ttypes() const {
        return func_type_data.nested_template_types;
    }
};

class state {
  public:
    std::map<std::string, record_data> records;
    std::vector<std::string> record_order;

    std::map<std::string, function_data> functions;
    std::vector<std::string> function_order;
};
