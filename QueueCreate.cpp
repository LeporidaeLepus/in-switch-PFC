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

    QueueCreate::QueueCreate():QueueCreate(int nport, int index){
        NS_LOG_FUNCTION(this);
        this->pair = nport;
        this->currentIndex = index;
        cout<<"VOQ_OFF: "<<VOQ_OFF<<" VOQ_ON: "<<VOQ_ON<<endl;
        cout<<"VIQ_OFF: "<<VIQ_OFF<<" VIQ_ON: "<<VIQ_ON<<endl;

        ObjectFactory factory;
        factory.SetTypeId("ns3::DropTailQueue");
        factory.Set("Mode", EnumValue(Queue::QUEUE_MODE_PACKETS));
        factory.Set("MaxPackets", UintegerValue(DEFAULT_FIFO_N_SIZE));

        for(int p = 0; p < PER_PAIR; p++){
            for(int i = 0; i < (pair); i++){
                for(int j = 0; j < (pair); j++){
                    fifos[p][i][j] = GetPointer(factory.Create<Queue>());
                    uint32_t maxSize = DEFAULT_FIFO_N_SIZE;
                    fifos[p][i][j]->SetMaxPackets(maxSize);
                }
            }
        }
    }  

    QueueCreate::~QueueCreate(){
        NS_LOG_FUNCTION(this);
    }
}