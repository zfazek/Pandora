#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "util.h"

class Record;

using namespace std;

/* Destructor frees memblock */
Util::~Util() {
    delete[] memblock;
}

/* Reads the whole CDR file to memory.
 * Stores in memblock.
 */
int Util::readFileToMemory(const string &pl_ifpName) {
    ifstream ifp;
    ifp.open(pl_ifpName.c_str(), ios::binary | ios::in | ios::ate);
    if (ifp.is_open()) {
        size = ifp.tellg();
        memblock = new unsigned char[size];
        ifp.seekg(0, ios::beg);
        ifp.read((char*)(&memblock[0]), size);
        ifp.close();
    } else {
        cout << "Could not open input file! Aborting." << endl;
        return 1;
    }
    return 0;
}

/* Parses the whole CDR found in memory.
 * Splits by records.
 * Returns as a vector.
 */
vector<Record> Util::parseCdr() {
    vector<Record> ret;
    Record record;
    idx = 0;
    /* It does not go to the end of the file because there is a 0x0A */
    while (idx < (int)size - 100) {
        int id = getId();
        record.setId(id);
        int length = getLength();
        record.setLength(length);
        record.setContent(getContent(length));
        ret.push_back(record);
        while (memblock[idx] == 0xFF) {idx++;}
    }
    return ret;
}

int Util::getId() {
    unsigned char c;
    c = memblock[idx++];
    if (c == 0xBF) {
        c = memblock[idx++];
        return 0xBF * 256 + c;
    } else {
        return c;
    }
}

int Util::getLength() {
    unsigned char c;
    c = memblock[idx++];
    if ( c == 0x81) {
        c = memblock[idx++];
        return c;
    }
    if ( c == 0x82) {
        c = memblock[idx++];
        return c * 256 + memblock[idx++];
    }
}

vector<unsigned char> Util::getContent(int length) {
    vector<unsigned char> ret;
    unsigned char c;
    for (int i = 0; i < length; i++) {
        c = memblock[idx++];
        ret.push_back(c);
    }
    return ret;
}
