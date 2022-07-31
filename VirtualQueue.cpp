#include "ns3/VirtualQueue.h"
#include "ns3/QueueCreate.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
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

    int VXQ::getPktPort(Ptr<QueueDiscItem> item){

        //TODO: add function to get ports
        
        return srcPort, dstPort;
    }

    void VXQ::removePktLabel(Ptr(QueueDisvItem) item){
        //TODO: remove the label used in this switch
    }

    bool VXQ::DoEnqueue(Ptr<queueDiscItem> item){
        NS_LOG_FUNCTION(this);

        int srcPort, dstPort = getPktPort(item);
        if(srcPort == dstPort){
            cout<<"Input port cannot be the same as Output port."<<endl;
            Drop(item)

            return false;
        }

        vqueues.checkVoqTag(srcPort, dstPort);
        bool flag = vqueues.getVoqFlag(srcPort, dstPort);
        if(flag == true){
            cout<<"This stream has been paused."<<endl;
            Drop(item);

            return false;
        }

        QueueDiscItem* re = vqueues.voqEnqueue(item, srcPort, dstPort);

        if(re!=0){
            // TODO: add schedule

            cout<<"Enqueue to VOQ["<<srcPort<<","<<dstPort<<"]."
            
            return true;
        }
    }

    Ptr<QueueDiscItem> VXQ::DoDequeue(){
        int count = 0;
        bool flag = getPortFlag(this->currentPort);
        while((flag == true) || vqueues.isSelectedPortEmpty(1,currentPort)){ 
            count++;
            //If all ports are paused or empty, return 0
            if(count == this->nport){   
                cout<<"No available port."<<endl;
                return 0;
            }

            this->curentPort = (this->currentPort+1)/this->nport;
            flag = getPortFlag(this->currentPort);
        }

        int crp = this->currentPort;
        int crvq = this->currentVQueue[crp];

        while((crvq == crp) || vqueues.isSelectedFifoEmpty(int 1, int crp, int crvq)){
            crvq = (crvq+1)/this->nport;
        }

        QueueDiscItem* re = vqueues.viqDequeue(crvq, crp);

        crvq = (crvq+1)/this->nport;
        this->currentVQueue[crp] = crvq;
        crp = (crp+1)/this->nport;
        this->currentPort = crp;

        return re;
    }
}