#ifndef PGW_H
#define PGW_H

#include <string>
#include "record.h"

/* This class is used in case of PGW record */
class PGW {
    public:
        PGW(Record);
        void parseRecord(bool);
        std::string getRecord();

    private:
        Record record;
        int idx;
        int getLength(const std::vector<unsigned char> &);
        std::string open_time;
        std::string node_id;
        std::string imsi;
        std::string imei;
        std::string msisdn;
        int rat;
        int lac;
        int ci_sai;
        std::string apn;
        int duration;
        int cause;
        int cc;
        std::string service_id_str;
        int uplink;
        int downlink;
        std::string ipaddr_ms;
        std::string ipaddr_sgsn;
        std::string ipaddr_ggsn;
        std::string plmn;

};

#endif // PGW_H

