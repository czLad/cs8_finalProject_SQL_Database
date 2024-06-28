#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include "../../includes/sql/sql.h"
using namespace std;

bool test_stub(bool debug = false)
{
  if (debug){
    cout << "testB:: test-sub() entering test_sub" << endl;
  }
  return true;
}

bool test_sql_interactive(bool debug = false)
{
  if (debug){
    cout << "testB:: test-sql_interactive() entering test_sql_interactive" << endl;
  }
  SQL sql;
  Table t;
  char s[300];
  // string s_str = s;
  string s_str;
  while(true)
  {
    cout<<">";
    getline(cin, s_str);
    // cout<<"s_str: "<<s_str<<"\n";
    strcpy(s, s_str.c_str());
    // cout<<"after strcpy s: "<<s;
    //if bored write a tolower for string
    if(s_str == "exit")
      break;
    Table temp = sql.command(s_str);
    // if(sql.error_state())
      // cout<<"sql is in error_state\n";
    // else
    if(!sql.error_state())
    {
      cout<<temp;
      if(!temp.get_tablenames_table())
        cout << "records selected from old table: "<<sql.select_recnos() << endl;
    }
  }
  cout<<"-------------Exited SQL---------------\n";
  return true;
}

TEST(TEST_STUB, TestStub) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(false));
}

TEST(TEST_SQL, TestSQLInteractive) {

  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_sql_interactive(false));
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

