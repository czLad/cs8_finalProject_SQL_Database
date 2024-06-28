#ifndef ZAC_UTILITIES_H
#define ZAC_UTILITIES_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include "../table/typedefs.h"


using namespace std;

bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);
//init functions
void init_write_to_file_txt(const string& _filename, const vector<string>& data_arr);
void init_create_file(fstream& f, const char filename[]);
//
//read functions
vectorstr read_from_file_txt(const string& _filename);
//
//write functions 
void write_to_file_bin(const string& _filename, const vector<string>& data_arr);
void write_to_file_txt(const string& _filename, const vector<string>& data_arr);
void write_to_file_txt_app(const string& _filename, const vector<string>& data_arr);
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