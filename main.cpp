#include <iostream>
#include <iomanip>
#include "includes/sql/sql.h"


using namespace std;

int main(int argv, char** argc) {
    SQL sql;
    Table t;
    char s[300];
    string s_str;
    while(true)
    {
      cout<<">";
      getline(cin, s_str);
      strcpy(s, s_str.c_str());
      if(s_str == "exit")
        break;
      Table temp = sql.command(s_str);
      if(!sql.error_state())
      {
        cout<<temp;
        if(!temp.get_tablenames_table())
          cout << "records selected from old table: "<<sql.select_recnos() << endl;
      }
    }
    cout<<"-------------Exited SQL---------------\n\n\n";
    return 0;
}
