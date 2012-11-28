// Define the SIP wrapper to the SleepyHollow library.

%Module sleepy_hollow 0

class SleepyHollow
{

%TypeHeaderCode
#include "sleepy-hollow.h"
#include <QApplication>
%End

public:
    SleepyHollow();
    char * load(char* url);
};
