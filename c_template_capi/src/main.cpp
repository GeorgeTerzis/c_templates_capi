#include "ast_visitor.hpp"
#include <filesystem>

#include "macro_utils.hpp"
#include "utils.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char* const filename = argv[1];
    const char* const args[] = {"-xc", "-std=c2x"};

    // Info print
    {
        std::filesystem::path cwd = std::filesystem::current_path();
        std::string cpp_args;
        for (uint64_t i = 1; i < argc; i++)
            cpp_args = std::string(argv[i]) + " ";

        // std::cerr << "Working directory: \"" << cwd.string() << "\"" << "\n----"
        //           << "arguments: \"" << cpp_args << "\"" << "\n\n";

        std::string clang_args;
        for (uint64_t i = 0; i < sizeof args / sizeof *args; i++)
            clang_args += std::string(args[i]) + " ";

        // std::cerr << "Analysing file: \"" << filename << "\"" << "\n----"
        //           << "clang arguments: \"" << clang_args << "\"" << "\n\n"
        //           << "\n";
    }

    // Init
    CXTranslationUnit translation_unit;
    CXIndex index;
    {
        index = clang_createIndex(1, 0);
        translation_unit = clang_parseTranslationUnit(
            index,
            filename,
            args,
            sizeof args / sizeof *args,
            nullptr,
            0,
            CXTranslationUnit_LimitSkipFunctionBodiesToPreamble);

        if (!translation_unit) {
            fprintf(stderr,
                    "(error) \"%s\"  Failed to parse the translation unit.\n",
                    filename);
            return 1;
        }
    }

    // Analysis
    state data;
    ast_visitor visitor = ast_visitor(data);
    visitor.run(translation_unit);

    std::string file = read_file_into_string(filename);

    // Analysis 2
    std::vector<const record_data*> tinputs;
    std::vector<const record_data*> rtent;
    std::vector<const record_data*> rtaux;

    std::vector<const function_data*> ftent;
    std::vector<const function_data*> ftaux;
    {
        for (const auto& ro : data.record_order) {
            auto& r = data.records[ro];
            if (r.rec_type_data.tt == TEMPLATE_INPUT) {
                tinputs.push_back(&r);
            } else if (r.rec_type_data.tt == TEMPLATE_ENTITY) {
                rtent.push_back(&r);
            } else if (r.rec_type_data.tt == TEMPLATE_AUXILIARY) {
                rtaux.push_back(&r);
            }
        }

        for (const auto& fo : data.function_order) {
            auto& f = data.functions[fo];
            if (f.tt == TEMPLATE_ENTITY) {
                ftent.push_back(&f);
            } else if (f.tt == TEMPLATE_AUXILIARY) {
                ftaux.push_back(&f);
            }
        }

        std::cerr << ('\n');
    }

    // std::cerr << "--------OUTPUT--------" << std::endl;

    // Output

    std::string output_file = "./out.h";
    std::string user_prefix = "default";
    std::string out_text;
    {
        std::string begin_comments =
            str_comment("Input file: " + std::string(filename)) +
            str_comment("Prefix: " + user_prefix) + "\n" +
            str_comment("This file was automagicaly generated") +
            str_comment("When defining the input types make sure that you use single "
                        "word and no special characters types") +
            str_comment("Which means you have to typedef types like pointers");

        // Template inputs
        std::string tinput_def;
        std::string tinput_udef;
        for (const auto& ti : tinputs) {
            tinput_def +=
                str_ifndef(ti->rec_type_data.type) +
                str_error("Template Input: " + ti->name_() + " needs to be defined") +
                str_endif();

            tinput_udef += str_udef(ti->name_());
        }
        const std::string macros = str_gen_concat_macros(10);

        // Template Aux
        std::vector<std::string> aux_def;
        std::vector<std::string> aux_rec;
        std::vector<std::string> aux_func_prot;
        std::vector<std::string> aux_func_impl;
        std::vector<std::string> aux_udef;
        for (const auto& ta : rtaux) {
            const std::string def_name =
                std::string(ta->name_()).erase(ta->name_().find_last_of("_ta") - 2) +
                "_NAME";
            const std::string def =
                str_ifdef_def_else(def_name, ta->name_(), def_name) +
                str_def(ta->name_()) +
                std::string(ta->name_()).erase(ta->name_().find_last_of("_ta") - 2) +
                "\n" + str_endif();

            const std::string rec = ta->str(file) + "\n";
            const std::string udef = str_udef(ta->name_());

            aux_def.push_back(def);
            aux_rec.push_back(rec);
            aux_udef.push_back(udef);
        }

        for (const auto& ta : ftaux) {
            const std::string def_name =
                std::string(ta->name).erase(ta->name.find_last_of("_ta") - 2);
            const std::string def = str_def(ta->name) + def_name + "\n";
            const std::string body = str_sub(file, ta->loc.start, ta->loc.end);
            const std::string proto = body.substr(0, body.find("{")) + ";";
            const std::string udef = str_udef(def_name);
            aux_def.push_back(def);
            aux_udef.push_back(udef);
            aux_func_prot.push_back(proto);
            aux_func_impl.push_back(body);
        }

        // Template Ent
        std::vector<std::string> ent_def;
        std::vector<std::string> ent_rec;

        std::vector<std::string> ent_func_def;
        std::vector<std::string> ent_func_prot;
        std::vector<std::string> ent_func_impl;
        std::vector<std::string> ent_udef;
        for (const auto& te : rtent) {
            const std::set<std::string> ttypes = te->all_ttypes();
            const std::string def_name =
                std::string(te->name_()).erase(te->name_().find_last_of("_te") - 2);
            const std::string def =
                str_ifdef_def_else(def_name, te->name_(), def_name) + "  " +
                str_def(te->name_()) +
                str_concat_macro(def_name, {ttypes.begin(), ttypes.end()}) + "\n" +
                str_endif();

            const std::string udef = str_udef(te->name_());
            const std::string rec = te->str(file) + "\n";

            ent_def.push_back(def);
            ent_rec.push_back(rec);
            ent_udef.push_back(udef);
        }

        for (const auto& ta : ftent) {
            const std::set<std::string> ttypes = ta->all_ttypes();
            const std::string body = str_sub(file, ta->loc.start, ta->loc.end);

            const std::string def =
                str_def(ta->name) +
                str_concat_macro(ta->name, {ttypes.begin(), ttypes.end()});
            const std::string proto = body.substr(0, body.find("{")) + ";";

            const std::string udef = str_udef(ta->name);
            ent_func_def.push_back(def);
            ent_func_prot.push_back(proto);
            ent_func_impl.push_back(body);
            ent_udef.push_back(udef);
        }

        std::string aux_def_str;
        std::string aux_func_prot_str;
        std::string aux_func_impl_str;
        std::string aux_section;
        if (aux_def.size() > 0 && aux_rec.size() > 0 && aux_func_prot.size() > 0 &&
            aux_func_impl.size() > 0) {
            aux_section += str_ifndef(user_prefix + "_TEMPLATE_AUXILIARY") +
                           str_def(user_prefix + "_TEMPLATE_AUXILIARY") + "\n";
            for (const auto& a : aux_def) {
                aux_def_str += a + "\n";
            }

            for (const auto& a : aux_rec) {
                aux_def_str += a + "\n";
            }

            for (const auto& a : aux_func_prot) {
                aux_func_prot_str += a + "\n" + "\n";
            }

            if (aux_func_impl.size() > 0) {
                aux_func_impl_str +=
                    str_ifdef(user_prefix + "_TEMPLATE_AUXILIARY_FUNC_IMPL") + "\n";
                for (const auto& a : aux_func_impl) {
                    aux_func_impl_str += a + "\n";
                }
                aux_func_impl_str += str_endif();
                aux_udef.push_back(
                    str_udef(user_prefix + "_TEMPLATE_AUXILIARY_FUNC_IMPL"));
            }
            aux_section +=
                aux_def_str + aux_func_prot_str + "\n" + aux_func_impl_str + str_endif();
        }

        std::string ent_def_str;
        for (const auto& e : ent_def) {
            ent_def_str += e + "\n";
        }
        for (const auto& e : ent_func_def) {
            ent_def_str += e + "\n";
        }

        std::string ent_rec_str;
        for (const auto& e : ent_rec) {
            ent_rec_str += e + "\n";
        }

        std::string ent_func_proto_str;
        for (const auto& e : ent_func_prot) {
            ent_func_proto_str += e + "\n";
        }

        std::string ent_func_impl_str;
        if (ent_func_impl.size() > 0) {
            ent_func_impl_str +=
                str_ifdef(user_prefix + "_TEMPLATE_ENTITY_FUNC_IMPL") + "\n";
            for (const auto& e : ent_func_impl) {
                ent_func_impl_str += e + "\n\n";
            }
            ent_func_impl_str += str_endif();
            ent_udef.push_back(str_udef(user_prefix + "_TEMPLATE_ENTITY_FUNC_IMPL"));
        }

        std::string ent_section =
            ent_def_str + "\n" + ent_rec_str + ent_func_proto_str + ent_func_impl_str;

        std::string udef;
        for (const auto& u : aux_udef) {
            udef += u + "\n";
        }
        for (const auto& u : ent_udef) {
            udef += u + "\n";
        }

        out_text += "" + begin_comments + "\n" + tinput_def + "\n" + macros + "\n\n" +
                    aux_section + "\n" + ent_section + "\n" + udef + "\n" + tinput_udef;
    }
    std::cout << out_text << std::endl;

    // Clean up
    clang_disposeTranslationUnit(translation_unit);
    clang_disposeIndex(index);

    return 0;
}
