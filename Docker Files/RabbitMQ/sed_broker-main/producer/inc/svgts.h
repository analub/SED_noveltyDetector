#ifndef SVGTS_H
#define SVGTS_H

#include <pbg.h>
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <datatypes.h>
#include <parameters.h>
#include <sstream>

using namespace std;

class SampledValuesGateWayTShark: public ProcessBusGateway
{

    protected:

        bool syncing = true;
        bool infiniteloop = false;

        long timeseg;
        long producer_enter_timestamp;
        
        long long timeparser;
        long long timedataSender;
        long long lastBatchTime;
        long long accumulatedTime = 0;
        
        string base_cmd = "tshark ";
        ostringstream stringStream;

        map<string, vector<string>> params;

        FILE *fpa;
        char* raw_data;
        SVData* processed_data;

    public:
    
        SampledValuesGateWayTShark(Broker*, string, string);
        SampledValuesGateWayTShark(Broker*, string);
        void run();
        bool acquire();
        bool process();
        bool publish();

};

#endif