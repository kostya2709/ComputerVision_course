#pragma once

#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>


class BigFloat
{
public:
    BigFloat( std::string init_val = std::string("0")) : value( init_val) {}

    BigFloat& operator+=( double val)
    {
        char cmd[100] = {};
        sprintf(cmd, "python3 src/utils/scripts/add.py %s %s > src/utils/scripts/tmp", value.c_str(), std::to_string( val).c_str());
        std::system( cmd);
        std::ifstream file ( "src/utils/scripts/tmp");
        value = std::string( std::istreambuf_iterator<char>( file),
                             (std::istreambuf_iterator<char>()));
        value.erase( remove( value.begin(), value.end(), '\n'), value.end());

        return *this;
    }
    
    BigFloat& operator/=( uint64_t val)
    {
        char cmd[100] = {};
        sprintf(cmd, "python3 src/utils/scripts/div.py %s %s > src/utils/scripts/tmp", value.c_str(), std::to_string( val).c_str());
        std::system( cmd);
        std::ifstream file ( "src/utils/scripts/tmp");
        value = std::string( std::istreambuf_iterator<char>( file),
                             (std::istreambuf_iterator<char>()));
        value.erase( remove( value.begin(), value.end(), '\n'), value.end());
        return *this;
    }

    double log10()
    {
        char cmd[100] = {};
        sprintf(cmd, "python3 src/utils/scripts/log10.py %s > src/utils/scripts/tmp", value.c_str());
        std::system( cmd);
        std::ifstream file ( "src/utils/scripts/tmp");
        std::string tmp_value( std::istreambuf_iterator<char>( file),
                               (std::istreambuf_iterator<char>()));
        tmp_value.erase( remove( value.begin(), value.end(), '\n'), value.end());
        return std::stod( tmp_value);
    }

private:

    std::string value;
};