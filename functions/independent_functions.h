#ifndef INDEPENDENT_FUNCTIONS_H_INCLUDED
#define INDEPENDENT_FUNCTIONS_H_INCLUDED

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>

namespace INDEPENDENT
{
    void cut_and_show(std::string s);

    template < class T >
    std::string to_string( T t )
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    int get_file_length(std::string filename);
    char get_validated_char();
    double get_validated_number();

}


#endif // INDEPENDENT_FUNCTIONS_H_INCLUDED
