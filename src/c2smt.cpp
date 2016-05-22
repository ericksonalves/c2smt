#include <iostream>

#include "options_parser.h"

int main(int argc, const char** argv)
{
    options_parser options_parser(argc, argv);

    const unsigned int result = options_parser.parse();

    switch (result)
    {
        case error_file_unavailable : std::cout << "File is unavailable" << std::endl; break;
        case error_no_input_file : std::cout << "No input file set" << std::endl; break;
        case error_unknown_option : break;
        case error_z3_unavailable : std::cout << "Sorry, Z3 is unavailable" << std::endl; break;
        default : break;
    }

    return 0;
}
