#include <stdio.h>
#include <iostream>
#include "pgw.h"
#include "record.h"

using namespace std;

/* Constructor. Sets data to default values */
PGW::PGW(Record r) {
    record = r;
    open_time = "";
    node_id = "";
    imsi = "";
    imei = "";
    msisdn = "";
    rat = -1;
    lac = -1;
    ci_sai = -1;
    apn = "";
    duration = -1;
    cause = -1;
    cc = -1;
    service_id_str = "";
    uplink = -1;
    downlink = -1;
    ipaddr_ms = "";
    ipaddr_sgsn = "";
    ipaddr_ggsn = "";
    plmn = "";
}

/* Parses one record. Only the interested contents are in the switch 
 * expressions. The rest goes to default parts.
 */
void PGW::parseRecord(bool debug) {
    int contentLength, contentLength1, contentLength2;
    int length = record.getLength();
    int c, c1, c2;
    vector<unsigned char> content = record.getContent();
    idx = 0;
    while (idx < length) {
        c = content[idx++];
        switch (c) {
            case 0x83: // servedIMSI (3)
                contentLength = getLength(content);
                imsi = record.getImsi(content, idx, contentLength);
                if (debug) printf("%02X %02X %s\n", c, contentLength, imsi.c_str());
                idx += contentLength;
                break;
            case 0x87: // accessPointNameNI (7)
                contentLength = getLength(content);
                apn = record.getApn(content, idx, contentLength);
                if (debug) printf("%02X %02X %s\n", c, contentLength, apn.c_str());
                idx += contentLength;
                break;
            case 0x8D: // OpenTime (13)
                contentLength = getLength(content);
                open_time = record.getTime(content, idx);
                if (debug) printf("%02X %02X %s\n", c, contentLength, open_time.c_str());
                idx += contentLength;
                break;
            case 0x8E: // CallDuration (14)
                contentLength = getLength(content);
                duration = record.getInt(content, idx, contentLength);
                if (debug) printf("%02X %02X %d\n", c, contentLength, duration);
                idx += contentLength;
                break;
            case 0x8F: // CauseofClosing (15)
                contentLength = getLength(content);
                cause = record.getInt(content, idx, contentLength);
                if (debug) printf("%02X %02X %d\n", c, contentLength, cause);
                idx += contentLength;
                break;
            case 0x92: // NodeID (18)
                contentLength = getLength(content);
                node_id = record.getNodeId(content, idx, contentLength);
                if (debug) printf("%02X %02X %s\n", c, contentLength, node_id.c_str());
                idx += contentLength;
                break;
            case 0x96: //MSISDN (22)
                contentLength = getLength(content);
                msisdn = record.getMsisdn(content, idx, contentLength);
                if (debug) printf("%02X %02X %s\n", c, contentLength, msisdn.c_str());
                idx += contentLength;
                break;
            case 0x97: //ChargingCharacteristic (23)
                contentLength = getLength(content);
                cc = record.getChargingChar(content, idx, contentLength);
                if (debug) printf("%02X %02X %d\n", c, contentLength, cc);
                idx += contentLength;
                break;
            case 0x9B: //PLMN (27)
                contentLength = getLength(content);
                plmn = record.getPlmn(content, idx);
                if (debug) printf("%02X %02X %s\n", c, contentLength, plmn.c_str());
                idx += contentLength;
                break;
            case 0x9D: //IMEI (29)
                contentLength = getLength(content);
                imei = record.getImei(content, idx, contentLength);
                if (debug) printf("%02X %02X %s\n", c, contentLength, imei.c_str());
                idx += contentLength;
                break;
            case  0x9E: //RATType (30)
                contentLength = getLength(content);
                rat = record.getInt(content, idx, contentLength);
                if (debug) printf("%02X %02X %d\n", c, contentLength, rat);
                idx += contentLength;
                break;
            case 0x9F: // first byte of a two-byte-long id. Tag > 31
                c = content[idx++];
                switch (c) {
                    case 0x20: //UserLocationInformation (32)
                        contentLength = getLength(content);
                        c1 = content[idx++];
                        if ((c1 & 0x1) == 0x1) {
                            idx += 7;
                        }
                        if ((c1 & 0x2) == 0x2) {
                            idx += 7;
                        }
                        if ((c1 & 0x4) == 0x1) {
                            idx += 7;
                        }
                        if ((c1 & 0x8) == 0x8) { // TAC
                            lac = record.getInt(content, idx + 3, 2);
                            if (debug) printf("9F %02X %02X %d\n", c, contentLength, lac);
                            idx += 5;
                        }
                        if ((c1 & 0x10) == 0x10) { // ECGI
                            ci_sai = record.getInt(content, idx + 4, 3);
                            if (debug) printf("9F %02X %02X %d\n", c, contentLength, ci_sai);
                            idx += 7;
                        }
                        if (debug) printf("9F %02X %02X\n", c, contentLength);
                        break;
                    default:
                        contentLength = getLength(content);
                        idx += contentLength;
                        if (debug) printf("9F %02X %02X\n", c, contentLength);
                        break;
                }
                break;
            case 0xA4: // ggsnAddress (4)
                contentLength = getLength(content);
                c1 = content[idx++];
                switch(c1) {
                    case 0x80: // iPBinV4Address
                        contentLength1 = getLength(content);
                        ipaddr_ggsn = record.getIpAddress(content, idx);
                        if (debug) printf("%02X %02X %s\n", c1, contentLength1, ipaddr_ggsn.c_str());
                        idx += contentLength1;
                        break;
                    default:
                        contentLength1 = getLength(content);
                        idx += contentLength;
                        if (debug) printf("%02X %02X\n", c1, contentLength);
                        break;
                }
                break;
            case 0xA6: // sgsnAddress (6)
                contentLength = getLength(content);
                c1 = content[idx++];
                switch(c1) {
                    case 0x30:
                        contentLength1 = getLength(content);
                        c2 = content[idx++];
                        switch(c2) {
                            case 0x80: // iPBinV4Address
                                contentLength2 = getLength(content);
                                ipaddr_sgsn = record.getIpAddress(content, idx);
                                if (debug) printf("%02X %02X %s\n", c2, contentLength2, ipaddr_sgsn.c_str());
                                idx += contentLength2;
                                break;
                            default:
                                contentLength2 = getLength(content);
                                idx += contentLength1;
                                if (debug) printf("%02X %02X %02X\n", c1, c2, contentLength1);
                                break;
                        }
                        break;
                    default:
                        contentLength1 = getLength(content);
                        idx += contentLength;
                        if (debug) printf("9F %02X %02X\n", c1, contentLength);
                        break;
                }
                break;
            case 0xA9: // servedPDPAddress (9)
                contentLength = getLength(content);
                c1 = content[idx++];
                switch(c1) {
                    case 0xA0:
                        contentLength1 = getLength(content);
                        c2 = content[idx++];
                        switch(c2) {
                            case 0x80: // iPBinV4Address
                                contentLength2 = getLength(content);
                                ipaddr_ms = record.getIpAddress(content, idx);
                                if (debug) printf("%02X %02X %s\n", c2, contentLength2, ipaddr_ms.c_str());
                                idx += contentLength2;
                                break;
                            default:
                                contentLength2 = getLength(content);
                                idx += contentLength1;
                                if (debug) printf("%02X %02X %02X\n", c1, c2, contentLength1);
                                break;
                        }
                        break;
                    default:
                        contentLength1 = getLength(content);
                        idx += contentLength;
                        if (debug) printf("9F %02X %02X\n", c1, contentLength);
                        break;
                }
                break;
            case 0xBF: // first byte of a two-byte-long id. Tag > 31
                c = content[idx++];
                switch (c) {
                    case 0x22:
                        contentLength = getLength(content);
                        if (debug) printf("BF %02X %02X\n", c, contentLength);
                        c1 = content[idx++];
                        switch (c1) {
                            case 0x30:
                                contentLength1 = getLength(content);
                                while (contentLength1 > 0) {
                                    c2 = content[idx++];
                                    switch(c2) {
                                        case 0x8C: // Uplink
                                            contentLength2 = getLength(content);
                                            uplink = record.getInt(content, idx, contentLength2);
                                            if (debug) printf("%02X %02X %02X %d\n", c1, c2, contentLength2, uplink);
                                            idx += contentLength2;
                                            contentLength1 -= contentLength2;
                                            break;
                                        case 0x8D: // Downlink
                                            contentLength2 = getLength(content);
                                            downlink = record.getInt(content, idx, contentLength2);
                                            if (debug) printf("%02X %02X %02X %d\n", c1, c2, contentLength2, downlink);
                                            idx += contentLength2;
                                            contentLength1 -= contentLength2;
                                            break;
                                        default:
                                            contentLength2 = getLength(content);
                                            idx += contentLength2;
                                            contentLength1 -= contentLength2;
                                            if (debug) printf("%02X %02X %02X\n", c1, c2, contentLength2);
                                            break;
                                    }
                                }
                                break;
                            default:
                                contentLength1 = getLength(content);
                                idx += contentLength;
                                if (debug) printf("BF %02X %02X\n", c1, contentLength);
                                break;
                        }
                        break;
                    default:
                        contentLength1 = getLength(content);
                        idx += contentLength;
                        if (debug) printf("BF %02X %02X\n", c, contentLength);
                        break;
                }
                break;
            default:
                contentLength = getLength(content);
                idx += contentLength;
                if (debug) printf("%02X %02X\n", c, contentLength);
                break;
        }
    }
    if (debug) cout << "=== END ===" << endl;
}

/* Returns the record as output formatted string */
string PGW::getRecord() {
    char ret[512];
    sprintf(ret, "%s,%s,%s,%s,%s,%d,%d,%d,%s,%d,%d,%d,%s,%d,%d,%s,%s,%s,%s\n", 
            open_time.c_str(), node_id.c_str(), imsi.c_str(), imei.c_str(), 
            msisdn.c_str(), rat, lac, ci_sai, apn.c_str(), duration, cause, 
            cc, service_id_str.c_str(), uplink, downlink, ipaddr_ms.c_str(), 
            ipaddr_sgsn.c_str(), ipaddr_ggsn.c_str(), plmn.c_str());
    return string(ret);
}

/* Returns the length of a content */
int PGW::getLength(const vector<unsigned char> &content) {
    int length, length7bit;
    length = content[idx++];
    if ( (length & 128) == 128 ) {
        length7bit = (length & 127);
        if (length7bit == 1) {
            length = content[idx++];
        }
        if (length7bit == 2) {
            length = content[idx++];
            length = 256 * length + content[idx++];
        }
    }
    return length;
}
