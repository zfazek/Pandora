#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "record.h"

class Util {

    public:
        ~Util();
        int readFileToMemory(const std::string&);
        std::vector<Record> parseCdr();

    private:
        int idx;
        int getId();
        int getLength();
        std::ifstream::pos_type size;
        unsigned char * memblock;
        std::vector<unsigned char> getContent(int);
};

#endif // UTIL_H
