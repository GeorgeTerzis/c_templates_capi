#pragma once
#include <optional>

#include "cclang_utils.hpp"
#include "state.hpp"
#include "ttype_regex.hpp"

#define child_visitor_(kind)                                                             \
    CXChildVisitResult visit_##kind##_children(CXCursor cursor,                          \
                                               CXCursor parent,                          \
                                               void* data_void)

#define decl_visit_branch(Kind, function)                                                \
    if (cursor.kind == Kind)                                                             \
    function
#define stmt_visit_branch(Kind, function)                                                \
    if (cursor.kind == Kind)                                                             \
    function

bool is_typedef(CXCursor cursor) {
    CXCursorKind kind = clang_getCursorKind(cursor);
    return kind == CXCursor_TypedefDecl;
}

CXType func_get_rtype(CXType ctype) {
    if (ctype.kind == CXType_FunctionProto) {
        return clang_getResultType(ctype);
    }
    return CXType{};
}

CXType ptr_type(CXType ctype, bool has_val = true) {
    if (!has_val)
        return (ctype);

    std::optional<CXType> rctype;
    if (ctype.kind == CXType_BlockPointer || ctype.kind == CXType_Pointer) {
        // block pointers always degrade to function pointers
        // and normal pointers will just give a type
        rctype = clang_getPointeeType(ctype);
    } else if (ctype.kind == CXType_FunctionProto) {
        // returns no matter what the result type of the function is
        return clang_getResultType(ctype);
    }

    has_val = rctype.has_value();
    return ptr_type(rctype.value_or(ctype), has_val);
}

// recursion will not happen for blocks and function pointers
// because if we do we get info about the return type not about the function it self
// but it will for pointers

bool clang_is_ptr_type(CXType c) {
    if (c.kind == CXType_Pointer)
        return true;
    else if (c.kind == CXType_BlockPointer)
        return true;
    return false;
}

bool is_func_def(CXCursor cursor) {
    CXCursor definition = clang_getCursorDefinition(cursor);
    return !clang_Cursor_isNull(definition);
}

bool is_func_decl(CXCursor cursor) {
    return clang_getCursorKind(cursor) == CXCursor_FunctionDecl &&
           clang_Cursor_isNull(clang_getCursorDefinition(cursor));
}

// sucks dick
// have to use a formater to fix the output from this one since we can not get the orginal
// text replace it with just looking through the file and getting what we want for now tho
// this will do
std::string GetSourceTextFromRange(CXTranslationUnit tu, CXSourceRange range) {
    CXToken* tokens;
    unsigned int numTokens;
    clang_tokenize(tu, range, &tokens, &numTokens);

    std::string sourceText;
    for (unsigned int i = 0; i < numTokens; ++i) {
        CXString tokenString = clang_getTokenSpelling(tu, tokens[i]);
        sourceText += std::string(clang_getCString(tokenString)) + " ";
        clang_disposeString(tokenString);
    }

    clang_disposeTokens(tu, tokens, numTokens);
    return sourceText;
}

// so does this by extension
std::string GetFunctionBody(CXCursor cursor) {
    CXSourceRange range = clang_getCursorExtent(cursor);
    CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);

    return GetSourceTextFromRange(tu, range);
}

typedef CXChildVisitResult (*child_visit_fptr)(CXCursor, CXCursor, CXClientData);

CXChildVisitResult visit_cursor(CXCursor cursor, CXCursor parent, CXClientData data_void);
CXChildVisitResult
visit_record_children(CXCursor cursor, CXCursor parent, void* data_void);
CXChildVisitResult
visit_func_body_children(CXCursor cursor, CXCursor parent, void* data_void);

class ast_visitor {
  public:
    state& st;

    ast_visitor(state& s) : st{s} {}

    std::vector<var_data> visit_func_params_types(CXType fctype) {
        // std::set<std::string> nested_types;
        std::vector<var_data> vvd;
        const CXType ftype =
            (clang_is_ptr_type(fctype)) ? clang_getPointeeType(fctype) : fctype;

        // arg info
        int arg_count = clang_getNumArgTypes(ftype);
        if (arg_count == -1)
            return vvd;

        for (int i = 0; i < arg_count; ++i) {
            CXType actype = clang_getArgType(ftype, i);
            type_data td = analyse_type(actype);
            var_data vd = {td, ""};

            // nested_types.insert(a_nested_types.begin(), a_nested_types.end());
            vvd.push_back(vd);
        }

        return vvd;
    }

    type_data analyse_type(CXType ctype) {
        const std::string type = clang_get_type_str(ctype);
        const template_type tt = is_ttype(type);
        const type_bools bools = type_bools_analysis(ctype);
        std::set<std::string> nested_types;

        if (bools.is_builtin) {
            return type_data{type, tt, bools, nested_types};
        } else if (bools.is_ptr || bools.is_fptr || bools.is_block || bools.is_fproto) {
            CXType ret_ctype = ptr_type(ctype);
            const auto ret_nested_types = analyse_type(ret_ctype).nested_template_types;

            nested_types.insert(ret_nested_types.begin(), ret_nested_types.end());

            // do the arguments for the function ptrs and blocks here
            if (bools.is_fptr || bools.is_block || bools.is_fproto) {
                const auto fparms = visit_func_params_types(ctype);
                std::set<std::string> fparms_nested_types;
                for (const auto& fp : fparms) {
                    fparms_nested_types.insert(
                        fp.var_type_data.nested_template_types.begin(),
                        fp.var_type_data.nested_template_types.end());
                }
                nested_types.insert(fparms_nested_types.begin(),
                                    fparms_nested_types.end());
            }
        } else if (tt == TEMPLATE_INPUT) {
            nested_types.insert(type);
        } else if (tt == TEMPLATE_ENTITY) {
            if (st.records.find(type) != st.records.end()) {
                const auto& nt = st.records[type].rec_type_data.nested_template_types;
                nested_types.insert(nt.begin(), nt.end());
            } else {
                std::cerr << "(error)" << "Template enity: " << type
                          << " was not found on recrods" << "\n";
            }
        }

        return {type, tt, bools, nested_types};
    }

    child_visitor_(record_internal) {
        CXType cctype = clang_getCursorType(cursor);
        const std::string name = clang_get_name_str(cursor);

        var_data vd = {analyse_type(cctype), name};

        std::vector<var_data>* const parent_fields =
            ((std::pair<ast_visitor*, std::vector<var_data>*>*)data_void)->second;
        parent_fields->push_back(vd);

        return CXChildVisit_Continue;
    }

    void analyse_record(CXCursor cursor, const bool is_union) {
        const std::string cname = clang_get_name_str(cursor);
        if (st.records.find(cname) != st.records.end())
            return;

        std::string name = cname;

        template_type tt = is_ttype(cname);
        if (tt == NOT_TEMPLATE)
            return;
        type_bools bools = {.is_record = true,
                            .is_struct = (bool)(1 - is_union),
                            .is_union = is_union};

        std::vector<var_data> fields;

        // VERY IMPORTANT
        std::pair<ast_visitor*, std::vector<var_data>*> cdata = {this, &fields};
        clang_visitChildren(cursor, visit_record_children, &cdata);
        // VERY IMPORTANT

        std::set<std::string> nested_types;

        for (const auto& f : fields) {
            nested_types.insert(f.var_type_data.nested_template_types.begin(),
                                f.var_type_data.nested_template_types.end());
        }
        std::cerr << "Record: " << name << "\n";
        const auto range = source_range(cursor);

        const record_data rd = {{name, tt, bools, nested_types}, fields, range};
        st.records[name] = rd;
        st.record_order.push_back(name);
    }

    void analyse_struct(CXCursor cursor) {
        analyse_record(cursor, false);
    }
    void analyse_union(CXCursor cursor) {
        analyse_record(cursor, true);
    }

    void analyse_enum(CXCursor cursor) {
        std::cerr << __PRETTY_FUNCTION__ << " is to be filled" << std::endl;
    }

    std::set<std::string> analyse_func_body() {
        std::set<std::string> nested_types;

        return nested_types;
    }

    // functions
    child_visitor_(func_body_internal) {
        // putchar('\n');
        CXType ctype = clang_getCursorType(cursor);
        CXCursorKind cckind = clang_getCursorKind(cursor);
        type_data td;
        function_data fd;
        std::pair<ast_visitor*, std::set<std::string>*>* cdata =
            (std::pair<ast_visitor*, std::set<std::string>*>*)data_void;
        std::set<std::string>* nt = cdata->second;

        // std::cerr << clang_get_kind_str(cursor) << "";

        switch (cckind) {
        case CXCursor_CompoundStmt:
        case CXCursor_IfStmt:
        case CXCursor_SwitchStmt:
        case CXCursor_CaseStmt:
        case CXCursor_WhileStmt:
        case CXCursor_ForStmt:
        case CXCursor_DoStmt:
        case CXCursor_ReturnStmt:
        case CXCursor_DeclStmt:
        case CXCursor_InitListExpr:
        case CXCursor_UnexposedExpr:
        case CXCursor_CompoundLiteralExpr:
            clang_visitChildren(cursor, visit_func_body_children, data_void);
            break;

        // by using ParmDecl we can replace the function argument function
        case CXCursor_ParmDecl:
        case CXCursor_TypeRef:
        case CXCursor_MemberRefExpr:
        case CXCursor_DeclRefExpr:
        case CXCursor_VarDecl:
            // std::cerr << "----Type: " << clang_get_type_str(ctype);
            td = analyse_type(ctype);
            nt->insert(td.nested_template_types.begin(), td.nested_template_types.end());
            clang_visitChildren(cursor, visit_func_body_children, data_void);
            break;
        case CXCursor_CallExpr:
            fd = analyse_func_proto(cursor);
            nt->insert(fd.func_type_data.nested_template_types.begin(),
                       fd.func_type_data.nested_template_types.end());
            break;
        default:
            // std::cerr << "--NO";
            break;
        }

        return CXChildVisit_Continue;
    }

    function_data analyse_func(CXCursor cursor,
                               CXType fctype,
                               std::string fname,
                               std::string ftype,
                               bool parse_body) {
        std::string name = fname;

        if (st.functions.find(name) != st.functions.end())
            return st.functions[name];

        type_data td = analyse_type(fctype);
        template_type tt = is_ttype(fname);

        if (tt == NOT_TEMPLATE)
            return function_data{};

        CXCursor body_cursor = clang_getCursorDefinition(cursor);

        std::vector<var_data> args = visit_func_params_types(fctype);

        if (parse_body) {
            std::set<std::string> body_nt;
            if (!clang_Cursor_isNull(body_cursor)) {
                std::pair<ast_visitor*, std::set<std::string>*> cdata = {this, &body_nt};
                clang_visitChildren(body_cursor, visit_func_body_children, &cdata);
            }
            td.nested_template_types.insert(body_nt.begin(), body_nt.end());
        }

        for (const auto& arg : args) {
            td.nested_template_types.insert(
                arg.var_type_data.nested_template_types.begin(),
                arg.var_type_data.nested_template_types.end());
        }

        const source_range range = source_range(cursor);
        return {td, name, args, tt, range};
    }

    function_data analyse_func(CXCursor cursor) {
        const CXType fctype = clang_getCursorType(cursor);
        const std::string fname = clang_get_name_str(cursor);
        const std::string ftype = clang_get_type_str(fctype);
        return analyse_func(cursor, fctype, fname, ftype, true);
    }
    function_data analyse_func_proto(CXCursor cursor) {
        const CXType fctype = clang_getCursorType(cursor);
        const std::string fname = clang_get_name_str(cursor);
        const std::string ftype = clang_get_type_str(fctype);
        return analyse_func(cursor, fctype, fname, ftype, false);
    }

    void visit_function(CXCursor cursor) {
        function_data fd = analyse_func(cursor);

        if (fd.tt == NOT_TEMPLATE)
            return;
        // std::cerr << "Function: " << fd.name << std::endl;

        st.functions[fd.name] = fd;
        st.function_order.push_back(fd.name);
    }

    CXChildVisitResult
    visit_cursor_internal(CXCursor cursor, CXCursor parent, CXClientData data_void) {
        decl_visit_branch(CXCursor_StructDecl, analyse_struct(cursor));
        else decl_visit_branch(CXCursor_UnionDecl, analyse_union(cursor));
        else decl_visit_branch(CXCursor_FunctionDecl, visit_function(cursor));

        return CXChildVisit_Recurse;
    }

    void run(CXTranslationUnit tu) {
        CXCursor cursor = clang_getTranslationUnitCursor(tu);
        std::pair<ast_visitor*, void*> cdata = {this, NULL};
        clang_visitChildren(cursor, visit_cursor, &cdata);
    }
};

CXChildVisitResult visit_cursor(CXCursor cursor, CXCursor parent, void* data_void) {
    auto cdata = (std::pair<ast_visitor*, void*>*)data_void;
    ast_visitor* v = cdata->first;
    return v->visit_cursor_internal(cursor, parent, cdata);
}

CXChildVisitResult
visit_record_children(CXCursor cursor, CXCursor parent, void* data_void) {
    std::pair<ast_visitor*, std::vector<var_data>*>* cdata =
        (std::pair<ast_visitor*, std::vector<var_data>*>*)(data_void);
    ast_visitor* v = cdata->first;
    return v->visit_record_internal_children(cursor, parent, cdata);
}
CXChildVisitResult
visit_func_body_children(CXCursor cursor, CXCursor parent, void* data_void) {
    std::pair<ast_visitor*, std::set<std::string>*>* cdata =
        (std::pair<ast_visitor*, std::set<std::string>*>*)data_void;
    ast_visitor* v = cdata->first;
    return v->visit_func_body_internal_children(cursor, parent, cdata);
}
