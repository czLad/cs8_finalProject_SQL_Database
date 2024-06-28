#ifndef ZAC_UTILITIES_CPP
#define ZAC_UTILITIES_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "utilities.h"
using namespace std;

bool file_exists(const char filename[])
{
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*)
{
    const bool debug = false;
    //opening a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        //create the file
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }
}
void open_fileW(fstream& f, const char filename[])
{
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }
}
//init functions
void init_write_to_file_txt(const string& _filename, const vector<string>& data_arr)
{
    // if(!file_exists(_filename.c_str()))
    // {
        ofstream fout;
        fout.open(_filename, ios::trunc);
        if(fout.fail())
        {
            cout <<"Fileout Error opening "<<_filename<<"\n";
            exit(1);
        }
        for(int i = 0; i < data_arr.size(); i++)
            fout << data_arr[i] << "\n";
        fout.close();
    // }
}
void init_create_file(fstream& f, const char filename[])
{
    open_fileW(f, filename);
    f.close();
}
//
//read functions
vectorstr read_from_file_txt(const string& _filename)
{
    const bool debug = false;
    vectorstr field_name_v;
    ifstream fin;
    fin.open(_filename);
    if(fin.fail())
    {
        cout <<"Fileout Error opening "<<_filename<<"\n";
        exit(1);
    }

    string read_line;
    while(fin >> read_line)
    {
        if(debug)
        {
            cout <<"read_line: "<<read_line<<"\n";
        }
        field_name_v.push_back(read_line);
    }
    fin.close();
    if(debug)
        cout<<field_name_v<<"\n";
    return field_name_v;
}
//
//write functions 
void write_to_file_bin(const string& _filename, const vector<string>& data_arr)
{
    ofstream fout;
    fout.open(_filename, fstream::app);
    if(fout.fail())
    {
        cout <<"Fileout Error opening "<<_filename<<"\n";
        exit(1);
    }
    for(int i = 0; i < data_arr.size(); i++)
        fout << data_arr[i] << "\n";
    fout.close();
}
void write_to_file_txt(const string& _filename, const vector<string>& data_arr)
{
    ofstream fout;
    fout.open(_filename, fstream::trunc);
    if(fout.fail())
    {
        cout <<"Fileout Error opening "<<_filename<<"\n";
        exit(1);
    }
    for(int i = 0; i < data_arr.size(); i++)
        fout << data_arr[i] << "\n";
    fout.close();
}
void write_to_file_txt_app(const string& _filename, const vector<string>& data_arr)
{
    ofstream fout;
    fout.open(_filename, fstream::app);
    if(fout.fail())
    {
        cout <<"Fileout Error opening "<<_filename<<"\n";
        exit(1);
    }
    for(int i = 0; i < data_arr.size(); i++)
        fout << data_arr[i] << "\n";
    fout.close();
}
//
// void confuse_me()
// {
//     string list[10] = {"zero",
//                        "one",
//                        "two",
//                        "three",
//                        "four",
//                        "five",
//                        "six",
//                        "seven",
//                        "eight",
//                        "nine"};

//     cout << "[ ]Show the list of input records:" << endl;
//     cout << "------------------------------" << endl;
//     for (int i = 0; i < 10; i++)
//     {
//         cout<<setw(6)<<list[i];
//     }
//     cout << endl<<endl;

//     cout << "[ ]Write / INSERT records into the file:" << endl;
//     cout << "--------------------------------------------" << endl;


//     fstream f;
//     //reset the file:
//     open_fileW(f, "record_list.bin");
//     for (int i= 0; i<10; i++){
//         FileRecord r(list[i]);      //filling the envelop
//         long recno = r.write(f);
//         cout<<"["<<setw(5)<<list[i] <<"]"<< " was written into file as record: "<<setw(2)<<recno<<endl;
//     }
//     f.close();
//     cout << endl
//           << endl;

//     cout << "[ ]Read / RETRIEVE / SELECT records from the file: " << endl;
//     cout << "------------------------------------------------------" << endl;

//     FileRecord r;
//     int selected_records[] = {4,6,8,9};
//     // open the file for reading and writing.
//     open_fileRW(f, "record_list.bin" );
//     for (int i = 0; i < 4; i++){
//         r.read(f, i);
//         cout << "reading record " << i << ": " << r << endl;
//     }

//     cout << "\n\n[ ]Reading passed the end of file: " << endl;
//     cout << "---------------------------------------------" << endl;

//     long bytes = r.read(f, 19);
//     cout<<"  number of bytes read: "<<bytes<<endl;
// }


#endif // ZAC_UTILITIES_