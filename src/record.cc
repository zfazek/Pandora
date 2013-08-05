#include <iostream>
#include <stdio.h>
#include <vector>
#include "record.h"

using namespace std;

void Record::setId(int newId) {
    id = newId;
}

int Record::getId() {
    return id;
}

void Record::setLength(int newLength) {
    length = newLength;
}

int Record::getLength() {
    return length;
}

void Record::setContent(vector<unsigned char> newContent) {
    content = newContent;
}

vector<unsigned char> Record::getContent() {
    return content;
}

string Record::getTime(const vector<unsigned char> &content, int idx) {
    int year, month, day, hour, minute, second;
    char ret[21];
    year = content[idx++];
    month = content[idx++];
    day = content[idx++];
    hour = content[idx++];
    minute = content[idx++];
    second = content[idx++];
    sprintf(ret, "20%02X.%02X.%02X. %02X:%02X:%02X", 
            year, month, day, hour, minute, second);
    return ret;
}

string Record::getNodeId(const vector<unsigned char> &content, int idx, 
        int length) {
    int ii;
    string ret = "";
    for (ii=0; ii < length; ii++) {
        ret += content[idx++];
    }
    return ret;
}


string Record::getImsi(const vector<unsigned char> &content, int idx, 
        int length) {
    int a, b;
    unsigned char c;
    string ret = "";
    for (int i = 0; i < length; i++) {
        c = content[idx++];
        a = c % 16;
        b = c / 16;
        if (b < 10) {
            ret += a + '0';
            ret += b + '0';
        } else {
            ret += a + '0';
        }
    }
    return ret;
}

string Record::getImei(const vector<unsigned char> &content, int idx, 
        int length) {
    // The same calculating method as Imsi
    return getImsi(content, idx, length);
}

string Record::getMsisdn(const vector<unsigned char> &content, int idx, 
        int length) {
    // The same calculating method as Imsi
    string ret = getImsi(content, idx, length);
    // Delete the leading '00'
    ret.erase(0, 2);
    return ret;
}

int Record::getInt(const vector<unsigned char> &content, int idx, 
        int length) {
    int ii, hatvany, szorzat, result;
    unsigned char c;
    result=0;
    for (ii=0; ii < length; ii++) {
        c=content[idx++];
        szorzat=1;
        for (hatvany=0; hatvany < length-ii-1; hatvany++) szorzat=szorzat*256;
        result=result+c*szorzat;
    }
    return result;
}

string Record::getApn(const vector<unsigned char> &content, int idx, 
        int length) {
    unsigned char c;
    string ret = "";
    for (int i = 0; i < length; i++) {
        c = content[idx++];
        if (c < 32) c = '.';
        ret += c;
    }
    return ret;
}

int Record::getChargingChar(const vector<unsigned char> &content, int idx, 
        int length) {
    int caca = 0;
    for (int i = 0; i < length; i++) {
        caca += content[idx++];
    }
    return caca;
}

string Record::getPlmn(const vector<unsigned char> &content, int idx) {
    int p1, p2, p3;
    string ret = "";
    p1 = content[idx++];
    p2 = content[idx++];
    p3 = content[idx++];
    ret += p1%16+'0';
    ret += p1/16+'0';
    ret += p2%16+'0';
    ret += p2/16+'0';
    ret += p3%16+'0';
    ret += p3/16+'0';
    if (ret[3] == 15+'0') ret[3]='0';
    return ret;
}

string Record::getIpAddress(const vector<unsigned char> &content, int idx) {
    int ip1, ip2, ip3, ip4;
    char ret[16];
    ip1 = content[idx++];
    ip2 = content[idx++];
    ip3 = content[idx++];
    ip4 = content[idx++];
    sprintf(ret, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
    return ret;
}

