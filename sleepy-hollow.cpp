#include <iostream>
#include <cstring>
#include "sleepy-hollow.h"

using namespace std;

SleepyHollow::SleepyHollow()
{
}

SleepyHollow::~SleepyHollow()
{
}

char* SleepyHollow::load(char *url) const
{
    int size = 18;
    size += strlen(url);
    char *body = new char[size];
    sprintf(body, "Faking access to: %s", url);
    return body;
}
