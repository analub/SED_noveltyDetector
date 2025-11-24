#include <svgts.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <chrono>
#include <ctime>
#include <string>
#include "pbg.cpp"
#include <rmqbroker.h>
#include <boost/shared_ptr.hpp>
#include <cstring>

using namespace std;
using json = nlohmann::json;

SampledValuesGateWayTShark::SampledValuesGateWayTShark(Broker* broker, string dst_mac, string src_mac):ProcessBusGateway(broker), timeseg(-1)
{

    this->broker->connect("process_bus_data", "sampled_values");

    this->processed_data = new SVData;
    this->raw_data = new char[PATH_MAX];

    cout <<"Aquisition module has been started ...."<< endl;
    
    string mac_filter = "'ether dst "; mac_filter += dst_mac; mac_filter += "'";
    //string mac_filter = "'ether src "; mac_filter += src_mac; mac_filter += "'";
    
    params["q"].push_back(" ");
    params["l"].push_back(" ");
    params["o"].push_back("sv.decode_data_as_phsmeas:TRUE");
    params["f"].push_back(mac_filter);
    params["b"].push_back("files:2");
    params["b"].push_back("filesize:100000");

    string tmpfile = "/tmp/cm_cap_";
    tmpfile+=dst_mac;
    tmpfile+=".tmp";

    params["w"].push_back(tmpfile);
    params["e"].push_back("sv.meas_value");
    params["e"].push_back("sv.smpCnt");
    params["e"].push_back("sv.svID");
    params["Tfields"].push_back(" ");

    FILE * out = popen("ls /sys/class/net/", "r");

    const int max_buffer = 1000;
    char buffer[max_buffer];

    while (fgets(buffer, sizeof buffer, out) != NULL) 
    {
        
        params["i"].push_back(strtok(buffer,"\n"));
    
    }

    this->stringStream << this->base_cmd;

    for (auto const &item : this->params)
    {


        for (int i = 0; i < (int)this->params[item.first].size(); i++)
        {
            
            this->stringStream << "-" << item.first << " " << params[item.first][i] << " ";
            
        }
    }
}

SampledValuesGateWayTShark::SampledValuesGateWayTShark(Broker* broker, string file_path):ProcessBusGateway(broker), timeseg(-1)
{   

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        cout << "[DEBUG] Current working directory: " << cwd << endl;
    else
        perror("getcwd() error");

    cout << "[DEBUG] FILE_PATH: " << file_path << endl;

    this->broker->connect("process_bus_data", "sampled_values");

    this->processed_data = new SVData;
    this->raw_data = new char[PATH_MAX];

    cout << "Acquisition module has been started ...." << endl;

    params["r"].push_back(file_path);
    params["Y"].push_back("sv");
    params["o"].push_back("sv.decode_data_as_phsmeas:TRUE");
    params["e"].push_back("sv.meas_value");
    params["e"].push_back("sv.smpCnt");
    params["e"].push_back("sv.svID");
    params["Tfields"].push_back(" ");
    params["q"].push_back(" ");
    params["l"].push_back(" ");

    this->stringStream << this->base_cmd;

    for (auto const &item : this->params)
    {
        for (int i = 0; i < (int)this->params[item.first].size(); i++)
        {
            this->stringStream << "-" << item.first << " " 
                               << params[item.first][i] << " ";
        }
    }

    cout << "Command: " << this->stringStream.str() << endl;

}

void SampledValuesGateWayTShark::run()
{

    this->fpa = popen(stringStream.str().c_str(), "r");

    while (true)
    {

        if(this->acquire())
        {

            if(this->process())
            {

                if(!this->publish())
                {

                    cout << "Error Publishing Message!" << endl;

                }                
            }
        }
    }
}

bool SampledValuesGateWayTShark::acquire()
{
    if (fpa == NULL)
    {
        printf("Error calling popen.\n");
        return false;
    }

    if (fgets(this->raw_data, PATH_MAX, this->fpa) == NULL)
    {
        pclose(this->fpa);
        this->fpa = popen(this->stringStream.str().c_str(), "r");

        if (this->fpa == NULL)
        {
            printf("Error restarting tshark for replay.\n");
            return false;
        }

        fgets(this->raw_data, PATH_MAX, this->fpa);
    }

    return true;
}

bool SampledValuesGateWayTShark::process()
{
    
    string testestr(this->raw_data);

    char seps[] = " ,\t\n";
    int iCol = 0, iLine = 0;
    char *token = strtok(this->raw_data, seps);

    try
    {
        while (token)
        {

            if (iLine == 9)
            {
                
                this->processed_data->sv_id = token;
                break;

            }

            if (iLine < NSAMPLES)
            {
                
                if (iCol < 4) // Currents
                { 
                
                    this->processed_data->Phsmeas[iLine][iCol] = atof(token) * 1.0E-3; //para normalizar 10^16

                }
                else // Voltages
                { 
                    
                    this->processed_data->Phsmeas[iLine][iCol] = atof(token) * 1.0E-2;  
                
                }
            }
            else // Store sample count values
            { 
                
                this->processed_data->SmpCnt[iCol] = atoi(token);

                if (iCol == 0)
                {
                    
                    if (this->timeseg < 0)
                    {

                        auto time = std::chrono::system_clock::now();
                        auto tmsmp = std::chrono::time_point_cast<std::chrono::microseconds>(time);

                        long microsecNTP = (tmsmp.time_since_epoch().count())%1000000;
                        long microsecMU = this->processed_data->SmpCnt[iCol]*TS*1000000;

                        this->timeseg = (microsecNTP > microsecMU) ? ((tmsmp.time_since_epoch().count())/1000000) : (((tmsmp.time_since_epoch().count())/1000000) - 1);

                        if (this->processed_data->SmpCnt[iCol] == 0)
                        {
                            
                            this->processed_data->timestamp = this->timeseg*1000000;
                        
                        }
                    }

                    if(this->processed_data->SmpCnt[iCol] != 0 || this->timeseg >= 0)
                    {

                        auto time = std::chrono::system_clock::now();
                        auto tmsmp = std::chrono::time_point_cast<std::chrono::microseconds>(time);

                        this->timeseg = tmsmp.time_since_epoch().count();

                        this->processed_data->timestamp = this->timeseg;

                    } 
                }
            }
        
            token = strtok(0, seps);
            iCol++;

            if (iCol > 7) // Reset column index and increment line number
            { 
                
                iCol = 0;
                iLine++;

            }
        } 
    }
    catch (exception &e)
    {
        
        cout << e.what() << endl;
    
    }

    if (this->syncing)
    {

        if ((this->processed_data->timestamp / 1000000) % 60 == 1 && this->processed_data->SmpCnt[0] == 0)
        {

            this->syncing = false;
            cout << "Acquisition module synchronized." << endl;
            
            return true;

        }
        else
        {

            return false;

        }
    }

    return true;

}

bool SampledValuesGateWayTShark::publish()
{

    try
    {

        static int i = 0;

        static json message;
        json j;
        map<int, vector<float>> samples;

        for (int s = 0; s < NSAMPLES; s++)
        {
            
            for (int ch = 0; ch < NCH; ch++)
            {
                
                samples[this->processed_data->SmpCnt[s]].push_back(this->processed_data->Phsmeas[s][ch]);
            
            }
        }

        j["timestamp"] = this->processed_data->timestamp;
        j["status"] = this->processed_data->status;
        j["samples"] = samples;
        j["sv_id"] = this->processed_data->sv_id;

        if (i < BATCHSIZE)
        {
            
            message[i] = j;
            i++;

        }
        else
        {

            this->broker->publish(message.dump());
                
            i = 0;
            message[i] = j;
            i++;

        }

        return true;
        
    }
    catch (exception &e)
    {
        
        cout << "Broker Connection Error: " << e.what() << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        return false;

    }
}