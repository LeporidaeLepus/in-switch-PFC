#include <queue>
#include "ns3/object-factory.h"
#include "ns3/Level_flex.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/pfifo-fast-queue-disc.h"
#include "ns3/queue.h"
#include "ns3/queue-disc.h"
// #include "ns3/Replace_string.h"
#include "ns3/simulator.h"

namespace ns3{
    NS_LOG_COMPONENT_DEFINE("CreateQueue");
    NS_OBJECT_ENSURE_REGISTERED(CreateQueue);

    TypeId QueueCreate::GetTypeId(void)
    {
      static TypeId tid = TypeId("ns3::QueueCreate")
            .SetParent<Object>()
            .SetGroupName("TrafficControl")
            .AddConstructor<QueueCreate>()
    	    .AddAttribute ("pair",
                   "num of ports",
                   IntegerValue (4),
                   MakeIntegerAccessor (&QueueCreate::pair),
                   MakeIntegerChecker<int> ())
            ;
        return tid;
    }

    QueueCreate::QueueCreate():QueueCreate(DEFAULT_PAIR,0){

    }

    QueueCreate::QueueCreate(int nport, int index){
        NS_LOG_FUNCTION(this);
        this->pair = nport;
        this->currentIndex = index;

        //create queue
        Queue* fifos[PER_PAIR][pair][pair];

        //create VXQ tag
        bool VOQ_tag[pair][pair];
        bool VIQ_tag[pair][pair];

        cout<<"VOQ_OFF: "<<VOQ_OFF<<" VOQ_ON: "<<VOQ_ON<<endl;
        cout<<"VIQ_OFF: "<<VIQ_OFF<<" VIQ_ON: "<<VIQ_ON<<endl;

        ObjectFactory factory;
        factory.SetTypeId("ns3::DropTailQueue");
        factory.Set("Mode", EnumValue(Queue::QUEUE_MODE_PACKETS));
        factory.Set("MaxPackets", UintegerValue(DEFAULT_FIFO_N_SIZE));

        //initiate queue
        for(int p = 0; p < PER_PAIR; p++){
            for(int i = 0; i < (pair); i++){
                for(int j = 0; j < (pair); j++){
                    fifos[p][i][j] = GetPointer(factory.Create<Queue>());
                    uint32_t maxSize = DEFAULT_FIFO_N_SIZE;
                    fifos[p][i][j]->SetMaxPackets(maxSize);
                }
            }
        }
        cout<<"Initiate "<<PER_PAIR<<"*"<<pair<<"*"<<pair<<" queues."<<endl

        //Initiate VXQ tag to false
        for(int i = 0; i < (pair); i++){
            for(int j = 0; j < (pair); j++){
                VOQ_tag[i][j] = false;   //if VOQ send PAUSE to upstream then VOQ_tag = true
                VIQ_tag[i][j] = false;   //if VIQ send PAUSE to VOQ then VIQ_tag = true
            }
        }
        cout<<"Initiate VOQ_tag and VIQ_tag."<<endl
    }  

    QueueCreate::~QueueCreate(){
        NS_LOG_FUNCTION(this);
    }

    /* TODO: get src port and dst port */
    QueueDiscItem* QueueCreate::voqEnqueue(QueueDiscItem* item, int src, int dst){
        NS_LOG_FUNCTION(this);

        checkVoqThreshold(src, dst);
        if((src!=dst) && (VIQ_tag[src][dst]!=true)){
            fifos[0][src][dst]->Enqueue(Ptr<QueueDiscItem>(item));
        }

        return item;
    }

    QueueDiscItem* QueueCreate::viqEnqueue(QueueDiscItem* item, int src, int dst){
        NS_LOG_FUNCTION(this);

        checkViqThreshold(src, dst);
        if((src!=dst) && (VIQ_tag[dst][src]!=true)){
            fifos[1][dst][src]->Enqueue(Ptr<QueueDiscItem>(item));
        }

        return item;
    }

    QueueDiscItem* QueueCreate::voqDequeue(int src, int dst){ //for in-switch transmission
        NS_LOG_FUNCTION(this);

        checkViqThreshold(src, dst);
        if (VIQ_tag[dst][src]!=trur){
            Ptr<QueueDiscItem> item = StaticCast<QueueDiscItem>(fifos[0][src][dst]->Dequeue());
            cout<<"VOQ["<<src<<","<<dst<<"] send a packet out."<<endl;  //for debug
        }
        else{
            Ptr<QueueDiscItem> item = NULL;
            cout<<"VOQ["<<src<<","<<dst<<"] has been paused, cannot send out packets now."<<endl;   //for debug
        }

        return GetPointer(item);
    }

    QueueDiscItem* QueueCreate::viqDequeue(int src, int dst){
        NS_LOG_FUNCTION(this);

        Ptr<QueueDiscItem> item = StaticCast<QueueDiscItem>(fifos[1][dst][src]->Dequeue());
        cout<<"VIQ["<<dst<<","<<src<<"] send a packet out."<<endl;  //for debugging

        return GetPointer(item);
    }

    void QueueCreate::checkVoqTag(int src, int dst){
        int npkt = getFifoNPackets(0,src,dst);
        bool otag = getVoqTag(src,dst);
        
        if(otag == false){  
            if(npkt < VOQ_OFF){ 
                /* No Action */
            }
            else if (npkt >= VOQ_OFF){      //When the num of pkts is larger than the high threshold
                setVoqTag(src,dst,true);    //Set tag to true and send PAUSE to upstream
            }           
        }
        else if (otag == true){
            if(npkt >= VOQ_ON){
                /* No Action */
            }
            else if(npkt < VOQ_ON){         //When the num of pkts is less than the low threshold
                setVoqTag(src,dst,false);   //Set tag to false and send RESUME to upstream
            }
        }
    }

    void QueueCreate::checkViqTag(int src, int dst){
        int npkt = getFifoNPackets(1,dst,src);
        bool itag = getViqTag(src,dst);

        if(itag == false){
            if(npkt < VIQ_OFF){
                /* No Action */
            }
            else if (npkt >= VIQ_OFF){      //When the num of pkts is larger than the high threshold
                setVoqTag(src,dst,true);    //Set tag to true and send PAUSE to upstream
            }
        }
        else if (itag == true){
            if(npkt >= VOQ_ON){
                /* No Action */
            }
            else if(npkt < VOQ_ON){         //When the num of pkts is less than the low threshold
                setViqTag(src,dst,false);   //Set tag to false and send RESUME to upstream
            }
        }

        void QueueCreate::InSwitchTransmisson(int src, int dst){
            QueueDiscItem* item = voqDequeue(src,dst);
            QueueDiscItem* item2 = viqEnqueue(item,src,dst);
        }

        void QueueCreate::RoundRobin(){
            /* TODO: Round Robin */
        }

        int QueueCreate::getFifoNPackets(int pair, int port, int queue){
            return fifos[pair][port][queue]->GetNPackets();
        }

        bool QueueCreate::isSelectedFifoEmpty(int pair, int port, int queue){
            return fifos[pair][port][queue]->IsEmpty();
        }

        bool QueueCreate::getVoqTag(int src, int dst){
            return VOQ_tag[src][dst];
        }

        void QueueCreate::setVoqTag(int src, int dst, bool tag){
            VOQ_tag[src][dst] = tag;
        }

        bool QueueCreate::getViqTag(int src, int dst){
            return VIQ_tag[dst][src];
        }

        void QueueCreate::setViqTag(int src, int dst, bool tag){
            VIQ_tag[dst][src] = tag;
        }
    }
}