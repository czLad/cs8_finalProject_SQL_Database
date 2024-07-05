#ifndef ZAC_LOGICAL_CPP
#define ZAC_LOGICAL_CPP

#include "logical.h"

Logical::Logical()
{
    set_token_type(OPERATOR);
    set_operator_type(LOGICAL);
    //need to set_precedence of the two logical operators
    //by default logical empty CTOR has "or"'s precedence
    set_precedence(1);
    _val = "";
}
Logical::Logical(const string& val) : Operator()
{
    set_token_type(OPERATOR);
    set_operator_type(LOGICAL);
    if(val == "and")
        set_precedence(2);
    else if(val == "or")
        set_precedence(1);
    else
        assert(false && "Invalid Logical Operator");
    _val = val;
}
string Logical::get_val()
{
    const bool debug = false;
    if(debug)
        cout<<"Entered virtual get_val in logical\n";
    return _val;
}
vectorlong Logical::evaluate(Token* field_token, Token* condition_token, vector<mmap_sl> &record_indicies, map_sl &field_indicies) throw(Error_Code)
{
    const bool debug = false;
    if(debug)
        cout<<"Virtual evaluate() of logical class fired.\n";
    vectorlong field_token_vec = static_cast<ResultSet*>(field_token)->get_val_list();
    vectorlong condition_token_vec = static_cast<ResultSet*>(condition_token)->get_val_list();
    if(_val == "and")
    {
        return intersect(field_token_vec, condition_token_vec);
    }
    else if(_val == "or")
    {
        return union_vecs(field_token_vec, condition_token_vec);
    }
    else
    {
        //if not "and" or "or" then, throw that there's a syntax error at or near the operator relational token is holding
        assert(_val == "or" && "Unrecognized token\n");
    }

}
vectorlong Logical::intersect(vectorlong vector_1, vectorlong vector_2)
{
    vectorlong result_vec;
    long* vector_1_arr = &vector_1[0];
    long* vector_2_arr = &vector_2[0];
    //for intersect // want to pick only the elements that both vectors have in common
    merge_sort(vector_1_arr, vector_1.size());
    merge_sort(vector_2_arr, vector_2.size());
    set_intersection(vector_1.begin(), vector_1.end(), vector_2.begin(), vector_2.end(), back_inserter(result_vec));
    return result_vec;
}
//Better performing union_vecs
vectorlong Logical::union_vecs(vectorlong& vector_1, vectorlong& vector_2)
{
    //ofc set_union didn't work! both vectors need to be sorted for se_union to work
    vectorlong result_vec;
    long* vector_1_arr = &vector_1[0];
    long* vector_2_arr = &vector_2[0];
    merge_sort(vector_1_arr, vector_1.size());
    merge_sort(vector_2_arr, vector_2.size());
    set_union(vector_1.begin(), vector_1.end(), vector_2.begin(), vector_2.end(), back_inserter(result_vec));
    return result_vec;
}
void Logical::print_value()
{
    cout<<_val;
}
//v Working but poor performing union func
// vectorlong union_vecs(vectorlong vector_1, vectorlong vector_2)
// {
//     vectorlong result_vec;
//     vectorlong bigger_vec;
//     vectorlong smaller_vec;
//     if(vector_1.size() > vector_2.size())
//     {
//         result_vec = vector_2;
//         bigger_vec = vector_1;
//         smaller_vec = vector_2;
//     }
//     else
//     {
//         result_vec = vector_1;
//         bigger_vec = vector_2;
//         smaller_vec = vector_1;
//     }
//     //for union // want to pick everything only once
//     for(int i = 0; i < bigger_vec.size(); i++)
//     {
//         bool contains = true;
//         for(int j = 0; j < smaller_vec.size(); j++)
//         {
//             if(bigger_vec[i] != smaller_vec[j])
//                 contains = false;
//             else
//                 contains = true;
//         }
//         if(!contains)
//             result_vec.push_back(bigger_vec[i]);
//     }
//     return result_vec;
// }
//v Working but poor performing union func
// vectorlong Logical::intersect(vectorlong vector_1, vectorlong vector_2)
// {
//     vectorlong result_vec;
//     //for intersect // want to pick only the elements that both vectors have in common
//     for(int i = 0; i < vector_1.size(); i++)
//     {
//         for(int j = 0; j < vector_2.size(); j++)
//         {
//             if(vector_1[i]  == vector_2[j])
//                 result_vec.push_back(vector_2[j]);
//         }
//     }

//     return result_vec;
// }

#endif // ZAC_LOGICAL_