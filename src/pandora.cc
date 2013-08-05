#include <iostream>
#include <string>
#include "util.h"
#include "pgw.h"

using namespace std;

void print_usage(char* argv[]) {
    printf("Usage: \n");
    printf("\t%s <ggsn_name> (zggsn1|zggsn2|zggsn11|zggsn9) filename\n", argv[0]);
    printf("or\n");
    printf("\t%s filename //only for testing if filename is in the same directory\n", argv[0]);
}

int main(int argc, char *argv[]) {
    bool debug = false;
    string inFile, outFile;
    ofstream ofp;
    /* Only for testing */
    if (argc == 2) {
        inFile = argv[1];
        outFile = inFile + ".loadfile";
        /* Normal way of usage */
    } else if (argc == 3) {
        inFile = string(argv[1]) + "/" + string(argv[2]);
        outFile = string(argv[1]) + "/loadfile/" + string(argv[2]) + ".loadfile";
    } else { /* Not enough parameters. Quitting */
        print_usage(argv);
        return 1;
    }
    /* Deletes output file */
    ofp.open(outFile.c_str());
    if (ofp.is_open()) {
        ofp.close();
    } else {
        cout << "Could not write output file! Aborting." << endl;
        return 1;
    }
    if (debug) cout << endl << "Start with " << inFile << endl;

    /* Loads file to memory and stores in records */
    Util *util = new Util();
    int ret = util->readFileToMemory(inFile);
    vector<Record> records = util->parseCdr();
    delete util;
    /* Exit in case of reading error */
    if (ret == 1) return 1;

    /* Iterates over records, parses one by one, writes to disk */
    if (debug) cout << endl;
    for (int i = 0; i < records.size(); i++) {
        int id = records[i].getId();
        if (debug) printf("%d. id: %X, ", i, id);
        if (debug) printf("length: %X\n", records[i].getLength());
        /* PGW record */
        if (id == 0xBF4F) {
            /* Creates a PGW object instance */
            PGW *cdr = new PGW(records[i]);
            /* Parses one record */
            cdr->parseRecord(debug);
            /* get the record as the output format */
            string result = cdr->getRecord();
            //            cout << result;
            delete cdr;
            /* Appends record to loadfile */
            ofp.open(outFile.c_str(), ios::app);
            if (ofp.is_open()) {
                ofp << result;
                ofp.close();
            } else {
                cout << "Could not write output file! Aborting." << endl;
                return 1;
            }

        }
    }
    return 0;
}
