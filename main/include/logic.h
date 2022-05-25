#ifndef LOGIC_H
#define LOGIC_H

#include "event.h"

class Logic
{
private:
    QueueIoToLogic& m_input;
    QueueLogicToIo& m_output;
public:
    Logic(QueueIoToLogic& input, QueueLogicToIo& output);
    ~Logic();
    void operator()();
};

Logic::Logic(QueueIoToLogic& input, QueueLogicToIo& output):
m_input(input),
m_output(output)
{
}

Logic::~Logic()
{
}

void Logic::operator()()
{
    
}


#endif