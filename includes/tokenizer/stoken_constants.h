#ifndef STOKEN_CONSTANTS_H
#define STOKEN_CONSTANTS_H

//v Prof's code
// const int MAX_COLUMNS = 256;
// const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;
//For now
//v Constants for table 1
// const int MAX_ROWS = 5;
//v Constants for table 2
const int MAX_ROWS = 9;
const int MAX_COLUMNS = 128;

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ*";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?.,:;'`~!\"";
const char PAREN[] = "()";
const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

//token types:
const int STOKEN_NUMBER = 1;
const int STOKEN_ALPHA = 2;
const int STOKEN_SPACE = 3;
const int STOKEN_OPERATOR = 4;
const int STOKEN_PUNC = 5;
//TOKEN PAREN is 8 because 6 and 7 are being used to handle decimals
const int STOKEN_PAREN = 8;

const int STOKEN_UNKNOWN = -1;
const int STOKEN_END = -2;

#endif