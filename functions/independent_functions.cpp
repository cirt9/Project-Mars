#include "independent_functions.h"

namespace INDEPENDENT
{
    void cut_and_show(std::string s)
    {
        if(s.find(" ") != std::string::npos || s.find(",") != std::string::npos)
        {
            std::vector<std::string> sentences;
            std::string temp;
            int len = s.length();
            int i=0;

            int j=73;
            while(len > 73)
            {
                if(s[j] == ' ' || s[j] == ',')
                {
                    sentences.push_back(s.substr(0,j));
                    s = s.substr(j+1, len);
                    len = s.length();
                    i++;
                    j=73;
                }
                else
                    j--;
            }

            sentences.push_back(s);
            i++;

            for(int j=0; j<i; j++)
                std::cout << sentences[j] << std::endl;
        }
        else if(s.size() < 60)
            std::cout << s << std::endl;
        else
            std::cout << "Blad w funkcji tnacej string." << std::endl;
    }

    int get_file_length(std::string filename)
    {
        std::ifstream fin;
        fin.open(filename, std::ifstream::binary);
        if(!fin.is_open())
            return -1;

        fin.seekg(0, fin.end);
        int length = fin.tellg();
        fin.seekg(0, fin.beg);
        fin.close();
        return length;
    }

    char get_validated_char()
    {
        std::string input;
        std::cin >> input;
        if(input.size() == 1)
            return input[0];
        else
            return '.';
    }

    double get_validated_number()
    {
        double number;
        std::string input = "";
        while(true)
        {
            getline(std::cin, input);
            std::stringstream stream(input);

            bool valid = true;
            for(size_t i=0; i<input.size(); i++)
            {
                if(isalpha(input[i]))
                {
                    valid = false;
                    break;
                }
            }

            if(stream >> number && valid)
                break;
            std::cout << "Podales zle dane! Sprobuj jeszcze raz." << std::endl;
         }
         return number;
    }

}
