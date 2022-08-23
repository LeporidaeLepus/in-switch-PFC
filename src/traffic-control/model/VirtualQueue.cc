#include "ns3/VirtualQueue.h"
#include "ns3/QueueCreate.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unistd.h>     //only used in linux
#include "ns3/ipv4-header.h"
#include "ns3/ppp-header.h"
#include "ns3/ipv6-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ptr.h"
#include "ns3/queue.h"
#include "ns3/queue-disc.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-queue-disc-item.h"

using namespace std;

namespace ns3 {
    NS_LOG_COMPONENT_DEFINE("VXQ");
    NS_OBJECT_ENSURE_REGISTERED(VXQ);

    TypeId VXQ::GetTypeId(void){
        static TypeId tid = TypeId("ns3::VXQ")
            .SetParent<QueueDisc>()
            .SetGroupName("TrafficControl")
            .AddConstructor<VXQ>();

        return tid;
    }

    VXQ::VXQ(): VXQ(DEFAULT_PAIR){

    }

    VXQ::VXQ(int nport){
        NS_LOG_FUNCTION(this);
        this->pair = nport;
        this->nport = nport;
        currentRound = 0;
        currentPort = 0;
        currentVQueue = new int[nport]();
        port_flag = new bool[nport]();
        currDst = new int[this->nport]();

        vqueues = new QueueCreate(nport);

        roundInit(currentVQueue, nport);
        flagInit(port_flag, nport);
    }

    void VXQ::roundInit(int* Ptr, int n){
        for(int i=0; i<n; i++){
            Ptr[i] = 0;
        }
    }

    void VXQ::flagInit(bool* flagPtr, int n){
        for(int i=0; i<n; i++){
            flagPtr[i] = false;
        }
    }

    void VXQ::setCurrentRound(int cr){
        if(this->currentRound <= cr){
            this->currentRound =cr;
        }
    }

    int[] VXQ::getPktPort(Ptr<QueueDiscItem> item){

        //TODO: add function to get ports

        //for debug
        int srcPort = 0;
        int dstPort = 1;

        int[] port[2] = {srcPort, dstPort}
        
        return port;
    }

    void VXQ::removePktLabel(Ptr<QueueDiscItem> item){
        //TODO: remove the label used in this switch
    }

    bool VXQ::DoEnqueue(Ptr<QueueDiscItem> item){
        NS_LOG_FUNCTION(this);

        int srcPort = getPktPort(item)[0];
        int dstPort = getPktPort(item)[1];
        if(srcPort == dstPort){
            cout<<"Input port cannot be the same as Output port."<<endl;
            Drop(item);

            return false;
        }

        vqueues->checkVoqFlag(srcPort, dstPort);
        bool flag = vqueues->getVoqFlag(srcPort, dstPort);
        if(flag == true){
            cout<<"This stream ("<<srcPort<<","<<dstPort<<") has been paused."<<endl;
            Drop(item);

            return false;
        }

        QueueDiscItem* re = vqueues->voqEnqueue(item, srcPort, dstPort);

        if(re!=0){
            // TODO: add schedule

            cout<<"Enqueue to VOQ["<<srcPort<<","<<dstPort<<"]."<<endl;
            
            return true;
        }
        else{
            cout<<"Enqueue false. No package is enqueued."<<endl;

            return false;
        }
        
        InSwitchRoundRobin();

        return true;
    }

    Ptr<QueueDiscItem> VXQ::DoDequeue(){
        int count = 0;
        bool flag = getPortFlag(this->currentPort);
        while((flag == true) || vqueues->isSelectedPortEmpty(1,currentPort)){ 
            count++;
            //If all ports are paused or empty, return 0
            if(count == this->nport){   
                cout<<"No available port."<<endl;
                return 0;
            }

            this->currentPort = (this->currentPort+1)/this->nport;
            flag = getPortFlag(this->currentPort);
        }

        int crp = this->currentPort;
        int crvq = this->currentVQueue[crp];

        while((crvq == crp) || vqueues->isSelectedFifoEmpty(1,crp,crvq)){
            crvq = (crvq+1)/this->nport;
        }

        QueueDiscItem* re = vqueues->viqDequeue(crvq, crp);

        crvq = (crvq+1)/this->nport;
        this->currentVQueue[crp] = crvq;
        crp = (crp+1)/this->nport;
        this->currentPort = crp;

        InSwitchRoundRobin();

        return re;
    }

    bool VXQ::getPortFlag(int nPort){
        if(nPort>=this->nport || nPort<0){
            cout<<"Wrong port number"<<endl;
            return true;    //when flag==true, the port is paused and will be skipped.
        }
        return this->port_flag[nPort];
    }

    void VXQ::setPortFlag(int nPort, bool flag){
        if(nPort>=this->nport || nPort<0){
            cout<<"Wrong port number"<<endl;
            return;
        }

        this->port_flag[nPort] = flag;
    }

    void VXQ::InSwitchRoundRobin(){
        while(true){
            for(int src=0; src<this->nport; src++){
                int count = 0;
                int dst = currDst[src];

                dst=skipEqualDstPort(src,dst);
                vqueues->checkViqFlag(src,dst);
                while(vqueues->getViqFlag(src,dst)==true && count<(this->nport-1)){
                    dst = portAddOne(dst);
                    dst = skipEqualDstPort(src,dst);
                    count++;
                }
                if(count==this->nport-1){
                    currDst[src] = portAddOne(dst);
                    cout<<"SrcPort "<<src<<" is paused."<<endl;
                    continue;
                }  

                vqueues->InSwitchTransmission(src,dst);

                currDst[src] = portAddOne(dst);
            }
            usleep(INSWITCH_INTERVAL);
        }
    }

    bool VXQ::arePortsEqual(int src, int dst){
        if(src==dst) return true;

        return false;
    }

    int VXQ::skipEqualDstPort(int src, int dst){
        if(arePortsEqual(src,dst)) dst = portAddOne(dst);

        return dst;
    }

    int VXQ::portAddOne(int port){
        return (port+1)/this->nport;
    }



    Ptr<QueueDiscItem> VXQ::DoRemove(void) {
        return 0;
    }

    Ptr<const QueueDiscItem> VXQ::DoPeek(void) const {
        return 0;
    }

    void VXQ::InitializeParams(void)
    {
        NS_LOG_FUNCTION(this);
    }

    bool VXQ::CheckConfig(void)
    {
        NS_LOG_FUNCTION(this);
        if (GetNQueueDiscClasses() > 0){
            NS_LOG_ERROR("VXQ cannot have classes");
            return false;
        }

        if (GetNPacketFilters() != 0){
            NS_LOG_ERROR("VXQ needs no packet filter");
            return false;
        }

        if (GetNInternalQueues() > 0) {
            NS_LOG_ERROR("VXQ cannot have internal queues");
            return false;
        }

        return true;
    }
}