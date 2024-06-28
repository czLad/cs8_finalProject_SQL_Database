#ifndef ZAC_FILE_RECORD_CPP
#define ZAC_FILE_RECORD_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "file_record.h"
using namespace std;

FileRecord::FileRecord()
{
    // _record[0][0] = '\0';
    recno = -1;
    for(int i = 0; i < ROW; i++)
    {
        _record[i][0] = '\0';
    }
}
FileRecord::FileRecord(vector<string> s){
    for(int i = 0; i < ROW; i++)
    {
        _record[i][0] = '\0';
    }
    for(int i = 0; i < s.size(); i++)
    {
        strncpy(_record[i], s[i].c_str(), MAX);
    }
    
    //stub
}
long FileRecord::write(fstream& outs)              //returns the record number
{
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record
    // cout<<"went into write\n";
    //write to the end of the file.
    const bool debug = false;
    outs.seekg(0, outs.end);
    //start as 0 as in beginning to the end of the file
    // long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer
    //outs.write(&record[0], sizeof(record));
    long pos = outs.tellp(); 
    
    for(int i = 0; i < ROW; i++)
    {
        // pos = outs.tellp();   
        outs.write(_record[i], sizeof(_record)/ROW);
        // cout <<"_record[i]: \n";
        // print_array(_record[i], sizeof(_record)/ROW);
        
    }
    // outs.write(_record, sizeof(_record));
    //this v size arithmetic should still work
    if(debug)
    {
        cout<<"pos: "<<pos<<"\n";
        cout<<"sizeof(_record): "<<sizeof(_record)<<"\n";
        cout<<"pos/sizeof(_record): "<<pos/sizeof(_record)<<"\n";
    }
    return pos/sizeof(_record);  //record number
}
long FileRecord::read(fstream& ins, long recno)    //returns the number of bytes
{
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    //should be able to get even spacing with this
    
    const bool debug = false;
    if(debug)
        cout<<"recno to read: "<<recno<<"\n";
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);
    //Prof's code
    //ins.read(&_record[0], sizeof(_record))
    int total_gcount = 0;
    for(int i = 0; i < ROW; i++)
    {
        ins.read(_record[i], sizeof(_record)/ROW);
        //setting end of each record to null
        _record[i][ins.gcount()] = '\0'; 
        total_gcount += ins.gcount();
        // print_array(_record[i], sizeof(_record)/ROW);
    }
    if(debug)
    {
        cout<<"total_gcount: "<<total_gcount<<"\n";
    }
    return total_gcount;
}                                        //      read = MAX, or zero if
                                        //      read passed the end of file
ostream& operator<<(ostream& outs,
                           const FileRecord& r)
{
    for(int i = 0; i < r.ROW; i++)
        outs<<setw(20)<<r._record[i];
    outs<<"\n";
    return outs;
}

#endif // ZAC_FILE_RECORD_