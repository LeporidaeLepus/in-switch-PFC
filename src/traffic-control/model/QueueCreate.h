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

        int pair = DEFAULT_PAIR;   //number of queue pairs
        int currentIndex;   //current serve index

        // Queue* fifos[PER_PAIR][pair][pair];

        uint32_t m_limit;   //!< Maximum number of packets that can be stored 
        int VOQ_ON = 5;     //RESUME threshold of VOQ
        int VOQ_OFF = 10;   //PAUSE threshold of VOQ 
        int VIQ_ON = 5;     //RESUME threshold of VIQ
        int VIQ_OFF = 10;   //PAUSE threshold of VOQ 
        bool** VOQ_flag = NULL;   //if VOQ send PAUSE to upstream then VOQ_flag = true
        bool** VIQ_flag = NULL;   //if VIQ send PAUSE to VOQ then VIQ_flag = true 
        
        // int remainingQ = SPEEDUP_FACTOR; // TODO:check if need speedup factor   
        int previous_idx = 0; 

    public:
        static TypeId GetTypeId(void);

        QueueCreate();
        QueueCreate(int nport);
        ~QueueCreate();

        QueueDiscItem* voqEnqueue(QueueDiscItem* item, int src, int dst); 
        QueueDiscItem* viqEnqueue(QueueDiscItem* item, int src, int dst);
        QueueDiscItem* voqDequeue(int src, int dst);
        QueueDiscItem* viqDequeue(int src, int dst); 

        void checkVoqFlag(int src, int dst);
        void checkViqFlag(int src, int dst);

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
        // bool isAllFifoEmpty();
        bool isSelectedPortEmpty(int pair, int port);

        // void InitializeRR();
        // void TerminateRR();
        // bool ifLowerthanVoqOn(int size);
        // bool ifHigherthanVoqOff(int size);
        // bool ifLowerthanViqOn(int size);
        // bool ifHigherthanViqOff(int size);
        bool getVoqFlag(int src, int dst);
        void setVoqFlag(int src, int dst, bool flag);
        bool getViqFlag(int src, int dst);
        void setViqFlag(int src, int dst, bool flag);

        // void setPreviousIndex(int idx);
        // int getPreviousIndex();
        

    };
}


#endif //_QUEUE_CREATE_H_