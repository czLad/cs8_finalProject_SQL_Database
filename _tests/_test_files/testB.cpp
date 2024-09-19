#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include "random"
#include "../../includes/bplustree/bplustree.h"
#include "../../includes/bplustree/btree_array_funcs.h"
#include "../../includes/array_functions/array_functions.h"
#include "../../includes/bplustree/multimap.h"
#include "../../includes/sql/sql.h"
using namespace std;

bool test_stub(bool debug = false)
{
  if (debug){
    cout << "testB:: test-sub() entering test_sub" << endl;
  }
  return true;
}

bool test_hard_coded_BPlusTree(bool debug = false)
{
  if (debug){
    cout << "testB:: test-hard_coded_BPlusTree() entering test_hard_coded_BPlusTree" << endl;
  }
  BPlusTree<int> bpt1;
  bpt1.make_tree();
  cout<<"----------Just BTree--------\n";
  cout<<bpt1;
  BPlusTree<int>::Iterator it1 = bpt1.find(50);
  cout <<"*it1 :"<<*it1<<"\n";
  const int bpt1_inner_node_count = 8;
  int bpt1_inner_arr[bpt1_inner_node_count] = {50, 70, 150, 175, 250, 100 ,200, 201};
  cout<<"----------Finding all inner nodes and root---------\n";
  for(int i = 0; i < bpt1_inner_node_count; i++)
  {
    it1 = bpt1.find(bpt1_inner_arr[i]);
    if(!it1.is_null())
      cout <<"found *it :"<<*it1<<"\n";
    else
      cout <<bpt1_inner_arr[i]<<" not found iterator is null\n";
  }
  const int bpt1_insert_arr_size = 4;
  int bpt1_insert_arr[bpt1_insert_arr_size] = {40, 60, 260, 270};
  for(int i = 0; i < bpt1_insert_arr_size; i++)
  {
    cout<<"Inserting: "<<bpt1_insert_arr[i]<<"\n";
    bpt1.insert(bpt1_insert_arr[i]);
    cout<<bpt1;
    cout<<"-------------------------\n";
  }
  cout<<"-----------Finding the inner and inserted nodes---------\n";
  for(int i = 0; i < bpt1_insert_arr_size; i++)
  {
    it1 = bpt1.find(bpt1_insert_arr[i]);
    if(!it1.is_null())
      cout <<"found *it :"<<*it1<<"\n";
    else
      cout <<bpt1_insert_arr[i]<<" not found iterator is null\n";
  }
  for(int i = 0; i < bpt1_inner_node_count; i++)
  {
    it1 = bpt1.find(bpt1_inner_arr[i]);
    if(!it1.is_null())
      cout <<"found *it :"<<*it1<<"\n";
    else
      cout <<bpt1_inner_arr[i]<<" not found iterator is null\n";
  }
  int bpt1_arr_size = 11;
  // int elements_in_bpt1_arr[]
  BPlusTree<int>::Iterator it2 = bpt1.find_leftmost_leaf();
  cout <<"Leftmost leaf: ";
  it2.print_Iterator();
  cout<<"\n";
  cout<<boolalpha<<it2.is_null()<<"\n";

  vector<int> leave_data_vec;
  // it2.next();
  // cout<<"Printing next leaf node:\n";
  // it2.print_Iterator();
  // cout<<"\n";
  // it2.next();
  // cout<<"Printing next leaf node:\n";
  // it2.print_Iterator();
  // cout<<"\n";

  for(BPlusTree<int>::Iterator it2 = bpt1.find_leftmost_leaf();!it2.is_null(); it2.next())
  {
    it2.get_data(leave_data_vec);
  }
  BPlusTree<int> bpt2;
  cout<<"leave_data_vec_size: "<<leave_data_vec.size()<<"\n";
  for(int i = 0 ; i < leave_data_vec.size(); i++)
  {
    cout<<"leave_data_vec[i]: "<<leave_data_vec[i]<<"\n";
    bpt2.insert(leave_data_vec[i]);
  }
  cout<<bpt2;

  cout<<"bpt1.contains(leave_data_vec[3]): "<<boolalpha<<bpt1.contains(leave_data_vec[3])<<"\n";

  BPlusTree<int> bpt3;
  bpt3.copy_tree(bpt2);
  cout<<"------------Copying bpt2 into bpt3---------------\n";
  cout<<bpt3<<"\n";
  vector<int> leave_data_vec_1;
  for(BPlusTree<int>::Iterator it3 = bpt3.find_leftmost_leaf();!it3.is_null(); it3.next())
  {
    it3.get_data(leave_data_vec);
  }
  BPlusTree<int> bpt4;
  cout<<"leave_data_vec_size: "<<leave_data_vec.size()<<"\n";
  for(int i = 0 ; i < leave_data_vec.size(); i++)
  {
    cout<<"leave_data_vec[i]: "<<leave_data_vec[i]<<"\n";
    bpt4.insert(leave_data_vec[i]);
  }
  cout<<"--------------After inserting all leaves of bpt3 in bpt4---------------\n";
  cout<<bpt4;
  cout<<"-----------------Clearing tree----------------\n";
  bpt4.clear_tree();
  cout<<bpt4;
  cout<<"--------------Testing Iterator------------\n";
  for(BPlusTree<int>::Iterator it3 = bpt3.find_leftmost_leaf();!it3.is_null(); ++it3)
  {
    cout<<"*it3: "<<*it3<<"\n";
  }
  cout<<"---------------Testing begin and end---------------\n";
  for(BPlusTree<int>::Iterator it3 = bpt3.begin(); it3 != bpt3.end(); it3++)
  {
    cout<<"*it3: "<<*it3<<"\n";
  }
  cout<<"post order: "<<bpt3.post_order()<<"\n";
  cout<<"*bpt3.lower_bound(150): "<<*bpt3.lower_bound(150)<<"\n";
  cout<<"*bpt3.upper_bound(150): "<<*bpt3.upper_bound(150)<<"\n";

  cout<<"-----------Testing remove--------------\n";
  cout<<"--------Tree before remove------------\n";
  cout << bpt3 <<"\n";
  cout<<"Removing 270: \n";
  bpt3.remove(270);
  cout << bpt3 <<"\n";
  cout<<"Inserting 270: \n";
  bpt3.insert(270);
  cout << bpt3 <<"\n";
  cout<<"Removing 250: \n";
  bpt3.remove(250);
  cout << bpt3 <<"\n";
  cout<<"Inserting 250: \n";
  bpt3.insert(250);
  cout << bpt3 <<"\n";
  cout<<"Removing 260: \n";
  bpt3.remove(260);
  cout << bpt3 <<"\n";
  cout<<"Removing 235: \n";
  bpt3.remove(235);
  cout << bpt3 <<"\n";
  vector<int> leave_data_vec_3;
  for(BPlusTree<int>::Iterator it3 = bpt3.find_leftmost_leaf();!it3.is_null(); it3.next())
  {
    it3.get_data(leave_data_vec_3);
  }
  cout<<"leave_data_vec_size: "<<leave_data_vec_3<<"\n";
  cout<<"Removing 70: \n";
  bpt3.remove(70);
  cout << bpt3 <<"\n";
  vector<int> leave_data_vec_2;
  for(BPlusTree<int>::Iterator it3 = bpt3.find_leftmost_leaf();!it3.is_null(); it3.next())
  {
    it3.get_data(leave_data_vec_2);
  }
  cout<<"leave_data_vec_size: "<<leave_data_vec_2<<"\n";
  cout<<"test ---- table madness mmap----\n";
  int keys[] = {43, 20, 96, 100, 69};
  // string values[] = {"fourthree", "twozero", "ninesixty", "onethousand", "sixninezero"};
  string values[] = {"Joe", "Gomez", "Apple"};
  MMap<string, long> mmap;
  for (long i = 0; i < 1; i++){
    mmap[values[i]];
    cout<<mmap[values[i]]<<"\n";
  }
  cout<<mmap<<"\n";

  return true;
}

bool test_operate(bool debug = false)
{

  if (debug)
  {
    const int capacity = 200;
    int arr[capacity];
    int size = 0;

    // for random seeding
    // srand(time(NULL));

    // 1. make entry array
    while (size != capacity)
    {
      int random = rand() % 500;
      // cout << "Random is " << random << endl;
      if (search(arr, size, random) == -1)
      {
        arr[size] = random;
        size++;
      }
    }

    // 2. make index array
    int indices[capacity];
    for (int i = 0; i < capacity; i++)
    {
      indices[i] = i;
    }

    // 3. fill the btree with array
    BPlusTree<int> bt;
    for (int i = 0; i < size; i++)
    {
      bt.insert(arr[i]);
    }
    bt.print_tree();

    // 4. shuffle the index array
    for (int i = 0; i < size * size; i++)
    {
      int first = rand() % 20;
      int second = rand() % 20;
      std::swap(indices[first], indices[second]);
    }

    // 5. insertion and removal order
    cout << "Insertion order: ";
    print_array(arr, size);
    cout << "Removal order: ";
    cout << "  {";
    for (int i = 0; i < size; i++)
    {
      cout << arr[indices[i]] << " ";
    }
    cout << "}\n\n";

    // 6. remove from the tree
    for (int i = 0; i < size; i++)
    {

      bt.remove(arr[i]);

      cout << "-----------------------Removed[" << arr[i] << "]-----------------------\n";
      cout << "round: " << i << endl;
      bt.print_tree();
    }

    // cout << "SIZE IS " << size << endl;

    // 7. check if the tree is empty
    if (!bt.empty())
    {
      cout << "Array is supposed to be empty\n";
      return false;
    }
  }
  // for (int i = 0; i < 5; i++){
  //   cout << "mmap[" << keys[i] << "]: " << mmap[keys[i]] << endl;
  // }


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

TEST(TEST_BPLUS, TestBPlus) {

  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_hard_coded_BPlusTree(false));
}

// TEST(TEST_OPERATE, TestOperate) {

//   //EXPECT_EQ(0, <your individual test functions are called here>);

//   EXPECT_EQ(1, test_operate(true));
// }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

