#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include <string>

/* This class contains the generic helper functions to get the given data from
 * the CDR file
 */
class Record {

    public:
        void setId(int);
        int getId();
        void setLength(int);
        int getLength();
        void setContent(std::vector<unsigned char>);
        std::vector<unsigned char> getContent();
        std::string getTime(const std::vector<unsigned char> &, int);
        std::string getNodeId(const std::vector<unsigned char> &, int, int);
        std::string getImsi(const std::vector<unsigned char> &, int, int);
        std::string getImei(const std::vector<unsigned char> &, int, int);
        std::string getMsisdn(const std::vector<unsigned char> &, int, int);
        int getInt(const std::vector<unsigned char> &, int, int);
        std::string getApn(const std::vector<unsigned char> &, int, int);
        int getChargingChar(const std::vector<unsigned char> &, int, int);
        std::string getPlmn(const std::vector<unsigned char> &, int);
        std::string getIpAddress(const std::vector<unsigned char> &, int);

    private:
        int id;
        int length;
        std::vector<unsigned char> content;
};

#endif // RECORD_H
