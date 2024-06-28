#ifndef STOKENIZER_H
#define STOKENIZER_H

#include "stoken_constants.h"
#include "string.h"
#include "stoken.h"
#include "state_machine_functions.h"

using namespace std;

class STokenizer
{
public:
    STokenizer()
    {
        //make buffer empty
        const bool debug = false;
        if(debug)
        {
            cout << "Stokenizer CTOR Fired.\n";
        }
        _buffer[0] = '\0';
        _pos = 0;
        init_table(_table);
        _buffer_null_hit_count = 0;
        make_table(_table);
        _subscript_token = false;
    }
    STokenizer(char str[])
    {
        const bool debug = false;
        if(debug)
        {
            cout << "Stokenizer(char str[]) CTOR Fired.\n";
        }
        //v Making sure buffer does not contain garbage
        memset(_buffer, 0, strlen(_buffer));
        strcpy(_buffer, str);
        init_table(_table);
        //v Not sure just testing
        make_table(_table);
        //v a CTOR bug was fixed here
        _pos = 0;
        _buffer_null_hit_count = 0;
        _subscript_token = false;
    }

    // bool done()            //true: there are no more tokens
    // {
    //     cout<<"_pos"<<_pos<<"\n";
    //     cout<<"strlen(_buffer)"<<strlen(_buffer)<<"\n";
    //     if(!_buffer[_pos])
    //     {
    //         _pos++;
    //     }
        
    //     return _pos == strlen(_buffer) + 1;
    // }
    //v old version
    bool done()            //true: there are no more tokens
    {
        //v Test for without memset
        // return _buffer_null_hit_count == 2;
        // return !(_buffer[_pos]);
        return _buffer_null_hit_count == 2 || strlen(_buffer) == 0;
    }
    bool more()            //true: there are more tokens
    {
        return !done();
    }
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    // friend STokenizer& operator >> (STokenizer& s, Token& t)
    // {
    //     string token_str = "";
    //     int token_type;
    //     int start_state = 0;
    //     // if(!(s.get_token(start_state, token_str)))
    //     // {
    //     //     
            // if(s._subscript_token)
            // {
                // if(s._buffer[s._pos+1] == '\0')
                // {
                //     s._pos++;
                // }  
                // else
                // {
                //     s._pos += 2;
                // }
                // }
            //     else
            // {
            //     s._pos++;
            //     token_type = TOKEN_UNKNOWN;
            //     t = Token(token_str, token_type);
            // }
    //     //     token_type = start_state;
    //     // }
    //     s.get_token(start_state, token_str);
    //     t = Token(token_str, start_state);
    //     //s._pos-1 is the position of last char of the token_str in the buffer
    //     // s._buffer[s._pos-1];

    //     return s;
    // }
    ///v old operator >>
    friend STokenizer& operator >> (STokenizer& s, SToken& t)
    {
        const bool debug = false;
        string token_str = "";
        int token_type;
        int start_state = 0;
        if(!(s.get_token(start_state, token_str)))
        {
            if(debug)
                cout << "Entered if(!(s.get_token(start_state, token_str)))\n";
            if(s._subscript_token)
            {
                //v To make sure done works properly
                s._pos += 2;
                if(s._buffer[s._pos] == '\0')
                    s._buffer_null_hit_count++;
                token_type = STOKEN_UNKNOWN;
                token_str = "";
                t = SToken(token_str, token_type);
                s._subscript_token = false;
            }
            else
            {
                s._pos++;
                token_type = STOKEN_UNKNOWN;
                t = SToken(token_str, token_type);
            }
        }
        else
        {
            char last_char_before_failing = s._buffer[s._pos - 1];
            // cout <<"s._pos: "<<s._pos<<"\n";
            // cout <<"strlen(s._buffer): "<< strlen(s._buffer) <<"\n";
            // cout <<"s._buffer[s._pos - 1]: "<<s._buffer[s._pos - 1]<<"\n";
            // cout <<"s._buffer[s._pos]: "<<s._buffer[s._pos]<<"\n";
            // cout <<"s._buffer[s._pos + 1]: "<<s._buffer[s._pos + 1]<<"\n";
            //Using start_state row to check token type
            // token_type = _table[0][last_char_before_failing];
            if(s._buffer_null_hit_count == 2)
               token_type = -1;
            else
                token_type = _table[0][last_char_before_failing];               
            // cout << "token_type: "<<token_type<<"\n";
            t = SToken(token_str, token_type);

        }

        return s;
    }

    //set a new string as the input string
    void set_string(char str[])
    {
        // cout <<"_buffer before strcpy without memset: "<<_buffer<<"\n";
        memset(_buffer, 0, strlen(_buffer));
        //^ emptying the _buffer
        strcpy(_buffer, str);
        // cout <<"_buffer after strcpy without memset: "<<_buffer<<"\n";
        // cout << "----- New Block ---------------------"<<sizeof(_buffer)<<"\n";
        _pos = 0;
        _buffer_null_hit_count = 0;
    }

    //Zac
    //for debug
    int get_pos(){return _pos;}
    void print_buffer()
    {
        cout<< "String in buffer: ";
        for(int i = 0; _buffer[i] != '\0'; i++)
            cout << _buffer[i];
        cout<< "NULL";
    }
    //for testing stage private functions
    //to delete later
    bool get_token_test(int& start_state, string& token)
    {
        return get_token(start_state, token);
    }
    

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    // void make_table(int _table[][MAX_COLUMNS])
    // {
    //     const bool debug = true;
    //     if(debug)
    //     {
    //         cout << "---Before Touching Table------\n";
    //         print_table(_table);
    //     }
    //     //v Hard coded, made table
    //     mark_fail(_table, 0);
    //     mark_success(_table, 1);
    //     mark_fail(_table, 2);
    //     mark_success(_table, 3);

    //     mark_cell(0 ,_table, 97, 1);
    //     mark_cell(0 ,_table, 98, 2);
    //     mark_cell(1 ,_table, 97, 1);
    //     mark_cell(1 ,_table, 98, 2);
    //     mark_cell(2 ,_table, 97, 3);
    //     mark_cell(3 ,_table, 97, 3);
    //     if(debug)
    //     {
    //         cout << "---After Making Table------\n";
    //         print_table(_table);
    //     }
    // }
    //^ stage zero stokenizer table
    void make_table(int _table[][MAX_COLUMNS])
    {
        const bool debug = false;
        if(debug)
        {
            cout << "---Before Touching Table------\n";
            print_table(_table);
        }
        
        mark_fail(_table, 0);
        mark_success(_table, 1); //1 2 3 4 5 represents token types
        mark_success(_table, 2);
        mark_success(_table, 3);
        mark_success(_table, 4);
        mark_success(_table, 5);
        mark_fail(_table, 6);
        mark_success(_table, 7);
        mark_success(_table, 8);

        //v Marking initial states 

        mark_cells(0, _table, DIGITS, STOKEN_NUMBER);
        mark_cells(0, _table, ALFA, STOKEN_ALPHA);
        mark_cells(0, _table, SPACES, STOKEN_SPACE);
        mark_cells(0, _table, OPERATORS, STOKEN_OPERATOR);
        mark_cells(0, _table, PUNC, STOKEN_PUNC);
        mark_cells(0, _table, PAREN, STOKEN_PAREN);

        //v Marking states for tokens

        mark_cells(STOKEN_NUMBER, _table, DIGITS, STOKEN_NUMBER);
        mark_cells(STOKEN_ALPHA, _table, ALFA, STOKEN_ALPHA);
        mark_cells(STOKEN_SPACE, _table, SPACES, STOKEN_SPACE);
        mark_cells(STOKEN_OPERATOR, _table, OPERATORS, STOKEN_OPERATOR);
        mark_cells(STOKEN_PUNC, _table, PUNC, STOKEN_PUNC);
        // v final project final bug
        // mark_cells(STOKEN_PAREN, _table, PAREN, STOKEN_PAREN);

        //last line for decimals
        mark_cells(6, _table, DIGITS, 7); //state 7 for doubles
        //if a second dot comes after doubles
        mark_cells(7, _table, DIGITS, 7); //keep in success state

        //if get a period in state 3 put to state 6
        mark_cell(STOKEN_NUMBER, _table, '.', 6);
        //if get another dot in state 6 then punc state '3'

        if(debug)
        {
            cout << "---After Making Table------\n";
            print_table(_table);
        }
    }
    //^ Stage 2 ascii encoded adjacency matrix
    //extract the longest string that match
    //     one of the acceptable token types
    //To implement tonight
    //Precond: start_state < MAX_ROWS
    bool get_token(int& start_state, string& token)
    {
        const bool debug = false;
        if(_buffer[_pos] < 0)
        {
            _subscript_token = true;
            return false;
        }
        if(_table[start_state][_buffer[_pos]] == -1)
            return false;
        if(_buffer[_pos] > MAX_COLUMNS-1)
            return false;
        
        bool _fail_state = false;
        bool token_found = false;
        bool unsucess_state = false;
        int token_start_pos = _pos;

        for(int i = token_start_pos; _buffer[i] != '\0' && !_fail_state; i++)
        {
            if(_buffer[i] < 0)
                _subscript_token = true;
            //v mistake
            // _current_state = _table[_current_state][_buffer[i]];
            if(!_subscript_token)
            {
                int _current_state = _table[start_state][_buffer[i]];
                if(debug)
                    cout <<"current_state: "<<_current_state<<"\n";
                //If I'm at this # state that's not -1, am I a sucessful state?
                if(_current_state == -1)
                {
                    _fail_state = true;
                }
                else
                {
                    //v if success state
                    if(_table[_current_state][0] == 1)
                    {
                        token_found = true;
                        unsucess_state = false;
                        start_state = _current_state;
                        _pos = i+1;
                    }
                    else
                    {
                        start_state = _current_state;
                        unsucess_state = true;
                    }
                }
            }
            else
            {
                //To check if there was known token before subscript or not
                //If not, return statement at the top should catch it
                if(token_found)
                {
                    _fail_state = true;
                    if(unsucess_state)
                        _subscript_token = false;
                    //_subscript_token false because don't want to increment two if in unsucess state
                }
                else
                {
                    _fail_state = true;
                    _subscript_token = false;
                    //_subscript_token false because don't want to increment two if token before is unknown state
                }
            }
        }
        if(_buffer[_pos] == '\0')
            _buffer_null_hit_count++;
        if(_buffer_null_hit_count == 2 && !_fail_state)
        {
            token_found = true;
        }
        //_pos is a direct 0 based index 
        for(int j = token_start_pos; j < _pos; j++)
        {
                token += _buffer[j];
        }

        return token_found;
    }
    /////
    // bool get_token(int& start_state, string& token)
    // {
    //     const bool debug = false;
    //     if(_table[start_state][_buffer[_pos]] == -1)
    //         return false;
    //     bool _fail_state = false;
    //     bool token_found = false;
    //     int i;
    //     int token_start_pos = _pos;

    //     for(int i = token_start_pos; _buffer[i] != '\0' && !_fail_state; i++)
    //     {
    //         // _current_state = _table[_current_state][_buffer[i]];
    //         int _current_state = _table[start_state][_buffer[i]];
    //         if(debug)
    //             cout <<"current_state: "<<_current_state<<"\n";
    //         //If I'm at this # state that's not -1, am I a sucessful state?
    //         if(_current_state == -1)
    //         {
    //             _fail_state = true;
    //         }
    //         else
    //         {
    //             // token_found = true;
    //             //v if success state
    //             if(_table[_current_state][0] == 1)
    //             {
    //                 token_found = true;
    //                 start_state = _current_state;
    //                 _pos = i+1;
    //             }
    //         }
    //     }
        
    //     for(int j = token_start_pos; j < _pos; j++)
    //     {
    //             token += _buffer[j];
    //     }

    //     return token_found;
    // }

    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
    unsigned int _buffer_null_hit_count;
    bool _subscript_token;

    // int _table[MAX_ROWS][MAX_COLUMNS];
    //Side note: a print buffer would be useful for debugging
};


#endif