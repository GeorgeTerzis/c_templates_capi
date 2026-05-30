#include <iostream>
#include <string>

std::string str_gen_concat_macros(int count, std::string concat1) {
    std::string out = "";
    for (int i = 0; i < count; ++i) {
        std::string part1 = concat1 + std::to_string(i);
        std::string args = "";
        for (int j = 0; j <= i; ++j) {
            args += " T" + std::to_string(j);
            if (j != i)
                args += ",";
        }
        
        std::string arg_part = "(prefix, "+args+")";
        std::string macro_part = "";
        for (int j = 0; j <= i; ++j) {
            if(j == 0)
                macro_part += "##";
                        
            macro_part += "T"+std::to_string(j);
            
            if(j != i)
                macro_part += "##_";
        }
        
        std::string main_macro = "#define "+ part1 + arg_part+ " " + part1 + "_" + arg_part;
        std::string sec_macro  = "#define "+ part1 + "_" + arg_part+ " prefix##_" + macro_part;
        
        out += sec_macro+"\n";
        out += main_macro+"\n";
        out += "\n";
    }
    
    return out;
}

int main() {
    int count = 10;
    std::string concat1 = "CONCAT_";
    std::cout << gen_concat_macros(count, concat1);
    return 0;
}

