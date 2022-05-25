#if !defined(EVENT_H)
#define EVENT_H

#include <deque>
#include <queue>
#include <cstdint>


struct EventIo
{
    //id for events 
    //0-4btn
    //5 posbtn
    //6-10 pottards
    uint8_t id;
    union
    {
        bool btnState;
        uint8_t ptardVal;
    };
};

struct EventLogic
{
    /* data */
};

using QueueIoToLogic = std::queue<EventIo>;

using QueueLogicToIo = std::queue<EventLogic>;

#endif // EVENT_H
