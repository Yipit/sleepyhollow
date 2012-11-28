#ifndef __SLEEPYHOLLOW_H__
#define __SLEEPYHOLLOW_H__
#include <QApplication>
#include <QTextEdit>

class SleepyHollow
{
public:
    SleepyHollow();
    ~SleepyHollow();

    char *load(char *url) const;

/* private: */
/*     char *url; */
/*     char *body; */
};

#endif
