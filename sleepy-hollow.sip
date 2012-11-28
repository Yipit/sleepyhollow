// Define the SIP wrapper to the SleepyHollow library.

%Module sleepy_hollow 0

class SleepyHollow
{

%TypeHeaderCode
#include "sleepy-hollow.h"
%End

public:
    SleepyHollow();
    char * load(char* url);
};
