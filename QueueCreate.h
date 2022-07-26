#ifndef _QUEUE_CREATE_H_
#define _QUEUE_CREATE_H_
#include <queue>
#include <map>
#include "ns3/queue.h"
#include "ns3/queue-disc.h"
#include "ns3/address.h"
#include "ns3/drop-tail-queue.h"

using namespace std;

namespace ns3{
    class QueueCreate: public Object{
    private:
        static const int DEFAULT_PAIR = 4;  //default number of queue pairs or ports
        static const int PER_PAIR = 2;  //number of queue groups per queue pair
        static const int DEFAULT_FIFO_N_SIZE = 20;
        static const int SPEEDUP_FACTOR = 1;

        int pair;   //number of queue pairs
        int currentIndex;   //current serve index

        // Queue* fifos[PER_PAIR][pair][pair];

        unit32_t m_limit;   //!< Maximum number of packets that can be stored 
        int VOQ_ON = 5;     //RESUME threshold of VOQ
        int VOQ_OFF = 10;   //PAUSE threshold of VOQ 
        int VIQ_ON = 5;     //RESUME threshold of VIQ
        int VIQ_OFF = 10;   //PAUSE threshold of VOQ 
        // bool VOQ_tag = false;   //if VOQ send PAUSE to upstream then VOQ_tag = true
        // bool VIQ_tag = false;   //if VIQ send PAUSE to VOQ then VIQ_tag = true 
        
        // int remainingQ = SPEEDUP_FACTOR; // TODO:check if need speedup factor   
        int previous_idx = 0; 

    public:
        static TypeId GetTypeId(void);

        QueueCreate();
        QueueCreate(int nport, int index);
        ~QueueCreate();

        QueueDiscItem* voqEnqueue(QueueDiscItem* item, int src, int dst); 
        QueueDiscItem* viqEnqueue(QueueDiscItem* item, int src, int dst);
        QueueDiscItem* voqDequeue(int src, int dst);
        QueueDiscItem* viqDequeue(int src, int dst); 

        void checkVoqTag(int src, int dst);
        void checkViqTag(int src, int dst);

        void InSwitchTransmission(int src, int dst);
        void RoundRobin();

        // int getCurrentIndex();
        // void setCurrentIndex(int index); // serving FIFO

        // void getAndIncrementIndex();
        // int getCurrentFifoSize();
        // int getFifoMaxNPackets();
        // int getCurrentFifoNPackets();
        int getFifoNPackets(int pair, int port, int queue);

        // bool isCurrentFifoEmpty();
        bool isSelectedFifoEmpty(int pair, int port, int queue);

        // void InitializeRR();
        // void TerminateRR();
        // bool ifLowerthanVoqOn(int size);
        // bool ifHigherthanVoqOff(int size);
        // bool ifLowerthanViqOn(int size);
        // bool ifHigherthanViqOff(int size);
        bool getVoqTag(int src, int dst);
        void setVoqTag(int src, int dst, bool tag);
        bool getViqTag(int src, int dst);
        void setViqTag(int src, int dst, bool tag);

        // void setPreviousIndex(int idx);
        // int getPreviousIndex();
        

    }
}


#endif //_QUEUE_CREATE_H_