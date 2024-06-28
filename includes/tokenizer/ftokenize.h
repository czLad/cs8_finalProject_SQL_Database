#ifndef FTOKENIZER_H
#define FTOKENIZER_H

#include "stoken_constants.h"
#include "string.h"
#include "stoken.h"
#include "state_machine_functions.h"
#include "stokenize.h"
#include "fstream"

using namespace std;

class FTokenizer
{
public:
    const int MAX_BLOCK = MAX_BUFFER;
    FTokenizer(char* fname)
    {
        _f.open(fname);
        if(_f.fail())
        {
            cout <<"_f ifstream error opening "<<fname<<"\n";
            exit(1);
        }
        _pos = 0;
        _blockPos = 0;
        _done_count = 0;
        if(get_new_block())
            _more_blocks = true;
        else
            _more_blocks = false;
    }
    SToken next_token()
    {
        return SToken();
    }
    bool more()        //returns the current value of _more
    {
        return _more;
    }
    int pos()          //returns the value of _pos
    {
        return _pos;
    }
    int block_pos()     //returns the value of _blockPos
    {
        return _blockPos;
    }
    friend FTokenizer& operator >> (FTokenizer& f, SToken& t)
    {
        if(f._more_blocks)
        {
            if(f._stk.done())
            {
                // cout <<"done f._blockPos" <<f._blockPos << "\n";
                if(f.get_new_block())
                {
                    f._done_count = 0;
                    f._blockPos = 0;
                    f._more_blocks = true;
                    f._stk >> t;
                    f._blockPos++;
                    f._pos++;
                }
                else
                {
                    f._more_blocks = false;
                }
            }
            else
            {
                // cout <<"f._blockPos" <<f._blockPos << "\n";
                f._stk >> t;
                f._blockPos++;
                f._pos++;
            }
        }
        else
        {
            if(f._stk.more())
            {
                f._stk >> t;
                f._blockPos++;
                f._pos++;
            }
            else
            {
                f._more = false;
                f._f.close();
            }
        }
        return f;
    }
private:
    bool get_new_block() //gets the new block from the file
    {
        const bool debug = true;
        char tempblock[MAX_BLOCK];
        // tempblock[0] = '\0';
        _f.read(tempblock, MAX_BLOCK);
        tempblock[_f.gcount()] = '\0';
        if(debug)
        {
            cout << "-------tempblock-----" << "\n";
            cout << tempblock << "\n";
        }
        if(strlen(tempblock) > 0)
        {
            cout << "----- New Block ---------------------"<<_f.gcount()<<"\n";
            _stk.set_string(tempblock);
            return true;
        }
        else
        {
            return false;
        }

    }
    std::ifstream _f;   //file being tokenized
    STokenizer _stk;     //The STokenizer object to tokenize current block
    int _pos;           //Current position in the file
    int _blockPos;      //Current position in the current block
    bool _more;         //false if last token of the last block
    bool _more_blocks;
                        //  has been processed and now we are at
                        //  the end of the last block.
    int _done_count;
};


#endif