#ifndef PBG_H
#define PBG_H

#include <broker.h>

class ProcessBusGateway
{
    protected:

        Broker* broker;

    public:
    
        ProcessBusGateway(Broker* broker);
        virtual void run() = 0;
        virtual bool acquire() = 0;
        virtual bool process() = 0;
        virtual bool publish() = 0;

};

#endif