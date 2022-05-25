#ifndef APP_H
#define APP_H
#include <thread>

#include "event.h"
#include "io.h"
#include "logic.h"


class App
{
private:
    QueueIoToLogic m_ioToLogic;
    QueueLogicToIo m_LogicToIo;
    Io m_io;
    Logic m_logic;
public:
    App();
    ~App();
    void operator()();
};

App::App():
m_io(m_LogicToIo, m_ioToLogic),
m_logic(m_ioToLogic, m_LogicToIo)
{
}

App::~App()
{
}

void App::operator()()
{
    //start everything
    m_io();
}

#endif