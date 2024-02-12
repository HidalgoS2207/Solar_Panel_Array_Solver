#ifndef ZPIPE_H
#define ZPIPE_H
#define SET_BINARY_MODE(file)
#include <iostream>
#include <cstring>
#include <cassert>
#include "zlib.h"

static const long int CHUNK = { 16384 };

class Zpipe 
{
public:
    int def(FILE*, FILE*, int); // compress
    int inf(FILE*, FILE*); // decompress
    void zerr(int);
    void usage(char**);
};
#endif