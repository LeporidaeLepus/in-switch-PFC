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
#include "ns3/QueueCreate.h"

namespace ns3{
    NS_LOG_COMPONENT_DEFINE("QueueCreate");
    NS_OBJECT_ENSURE_REGISTERED(QueueCreate);

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

    QueueCreate::QueueCreate():QueueCreate(DEFAULT_PAIR){

    }

    QueueCreate::QueueCreate(int nport){
        NS_LOG_FUNCTION(this);
        this->pair = nport;
        // this->currentIndex = index;

        //create queue
        // Queue* fifos[PER_PAIR][pair][pair];

        //create VXQ flag
        VOQ_flag = new bool*[nport]();
        VIQ_flag = new bool*[nport]();

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
        cout<<"Initiate "<<PER_PAIR<<"*"<<pair<<"*"<<pair<<" queues."<<endl;

        //Initiate VXQ flag to false
        for(int i = 0; i < (pair); i++){
            VOQ_flag[i] = new bool[nport]();
            VIQ_flag[i] = new bool[nport]();
            for(int j = 0; j < (pair); j++){
                VOQ_flag[i][j] = false;   //if VOQ send PAUSE to upstream then VOQ_flag = true
                VIQ_flag[i][j] = false;   //if VIQ send PAUSE to VOQ then VIQ_flag = true
            }
        }
        cout<<"Initiate VOQ_flag and VIQ_flag."<<endl;
    }  

    QueueCreate::~QueueCreate(){
        NS_LOG_FUNCTION(this);
    }

    /* TODO: get src port and dst port */
    Ptr<QueueDiscItem> QueueCreate::voqEnqueue(Ptr<QueueDiscItem> item, int src, int dst){
        NS_LOG_FUNCTION(this);

        checkVoqFlag(src, dst);
        if((src!=dst) && (VIQ_flag[src][dst]!=true)){
            fifos[0][src][dst]->Enqueue(item);
            cout<<"VOQ["<<src<<","<<dst<<"] receives a packet."<<endl;
        }

        return item;
    }

    Ptr<QueueDiscItem> QueueCreate::viqEnqueue(Ptr<QueueDiscItem> item, int src, int dst){ //for in-swtich transmission
        NS_LOG_FUNCTION(this);

        fifos[1][dst][src]->Enqueue(item);
        cout<<"VIQ["<<dst<<","<<src<<"] receives a packet."<<endl;

        return item;
    }

    QueueDiscItem* QueueCreate::voqDequeue(int src, int dst){ //for in-switch transmission
        NS_LOG_FUNCTION(this);

        Ptr<QueueDiscItem> item = StaticCast<QueueDiscItem>(fifos[0][src][dst]->Dequeue());
        cout<<"VOQ["<<src<<","<<dst<<"] sends a packet out."<<endl;  //for debug

        return GetPointer(item);
    }

    QueueDiscItem* QueueCreate::viqDequeue(int src, int dst){
        NS_LOG_FUNCTION(this);

        Ptr<QueueDiscItem> item = StaticCast<QueueDiscItem>(fifos[1][dst][src]->Dequeue());
        cout<<"VIQ["<<dst<<","<<src<<"] sends a packet out."<<endl;  //for debugging

        return GetPointer(item);
    }

    void QueueCreate::checkVoqFlag(int src, int dst){
        int npkt = getFifoNPackets(0,src,dst);
        bool oflag = getVoqFlag(src,dst);
        
        if(oflag == false){  
            if(npkt < VOQ_OFF){ 
                /* No Action */
            }
            else if (npkt >= VOQ_OFF){      //When the num of pkts is larger than the high threshold
                setVoqFlag(src,dst,true);    //Set flag to true and send PAUSE to upstream
            }           
        }
        else if (oflag == true){
            if(npkt >= VOQ_ON){
                /* No Action */
            }
            else if(npkt < VOQ_ON){         //When the num of pkts is less than the low threshold
                setVoqFlag(src,dst,false);   //Set flag to false and send RESUME to upstream
            }
        }
    }

    void QueueCreate::checkViqFlag(int src, int dst){
        int npkt = getFifoNPackets(1,dst,src);
        bool iflag = getViqFlag(src,dst);

        if(iflag == false){
            if(npkt < VIQ_OFF){
                /* No Action */
            }
            else if (npkt >= VIQ_OFF){      //When the num of pkts is larger than the high threshold
                setViqFlag(src,dst,true);    //Set flag to true and send PAUSE to upstream
            }
        }
        else if (iflag == true){
            if(npkt >= VOQ_ON){
                /* No Action */
            }
            else if(npkt < VOQ_ON){         //When the num of pkts is less than the low threshold
                setViqFlag(src,dst,false);   //Set flag to false and send RESUME to upstream
            }
        }
    }

    void QueueCreate::InSwitchTransmission(int src, int dst){
        checkViqFlag(src,dst);

        if ((src!=dst) && VIQ_flag[dst][src]!=true){
            QueueDiscItem* item = voqDequeue(src,dst);
            // QueueDiscItem* item2 = viqEnqueue(item,src,dst);
            viqEnqueue(item,src,dst);
        }
        else if(VIQ_flag[dst][src] == true){
            cout<<"VOQ["<<src<<","<<dst<<"] has been paused, cannot send out packets now."<<endl;   //for debug
        }
        else if(src == dst){
            cout<<"Output port cannot be the same as input port." <<endl;  //for debug
        }  
    }

    void QueueCreate::RoundRobin(){
         /* TODO: Round Robin */
    }

    int QueueCreate::getFifoNPackets(int i, int port, int queue){
        return fifos[i][port][queue]->GetNPackets();
    }

    bool QueueCreate::isSelectedFifoEmpty(int i, int port, int queue){
        return fifos[i][port][queue]->IsEmpty();
    }

    bool QueueCreate::isSelectedPortEmpty(int pair, int port){
        for (int i=0; i<this->pair; i++){
            if(!isSelectedFifoEmpty(pair,port,i)){
                return false;
            }
        }
        return true;
    }

    bool QueueCreate::getVoqFlag(int src, int dst){
        return VOQ_flag[src][dst];
    }

    void QueueCreate::setVoqFlag(int src, int dst, bool flag){
        VOQ_flag[src][dst] = flag;
    }

    bool QueueCreate::getViqFlag(int src, int dst){
        return VIQ_flag[dst][src];
    }

    void QueueCreate::setViqFlag(int src, int dst, bool flag){
        VIQ_flag[dst][src] = flag;
    }
    
}