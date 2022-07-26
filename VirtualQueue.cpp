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
        currentRound = 0;

        vqueues = new QueueCreate(nport);
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
        bool tag = vqueues.getVoqTag(srcPort, dstPort);
        if(tag == true){
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
}