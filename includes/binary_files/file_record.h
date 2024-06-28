#ifndef ZAC_FILE_RECORD_H
#define ZAC_FILE_RECORD_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <fstream>
#include "../array_functions/array_functions.h"
using namespace std;

class FileRecord{
public:
    //when you construct a FileRecord, it's either empty or it
    //  contains a word
    FileRecord();

    FileRecord(vector<string> s);

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                          //      read = MAX, or zero if
                                            //      read passed the end of file


    friend ostream& operator<<(ostream& outs,
                               const FileRecord& r);
    static const int MAX = 100;
    static const int ROW = 10;
    char _record[ROW][MAX+1];
private:
    // static const int MAX = 100;
    // static const int COL = 2;
    int recno;
    // char _record[MAX+1][COL];
};

#endif // ZAC_FILE_RECORD_