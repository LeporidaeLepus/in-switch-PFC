#include "ns3/QueueCreate.h"
#include <cmath>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "ns3/ipv6-header.h"
#include "ns3/queue.h"
#include "ns3/queue-disc.h"
#include "ns3/netanim-module.h"
#include "ns3/stats-module.h"

using namespace std;

namespace ns3{
    class VXQ: public QueueDisc{
    
    private:
        static const int DEFAULT_VOLUME = (4-1)*2*4;  //default number of VXQs
        static const int SPEEDUP_FACTOR = 1;    //default spped up

        int volume; // number of VXQs
        int currentRound;  // current Round

        int getPktPort(Ptr(QueueDiscItem) item);
        

    public:
        /**
        * \brief Get the type ID.
        * \return the object TypeId
        */
        static TypeId GetTypeId(void);

        /**
        * \brief VXQ constructor
        * Creates a queue with a default or customed number of packets
        */
        VXQ();
        explicit VXQ(int);
        virtual ~VXQ()

        bool DoEnqueue(Ptr<QueueDiscItem> item);
        Ptr<QueueDiscItem> DoDequeue(void);

        Ptr<const QueueDiscItem> DoPeek(void) const;
        bool CheckConfig(void);
        void InitializeParams(void);
        uint32_t m_limit;    //!< Maximum number of packets that can be stored

        Ptr<QueueDiscItem> DoRemove(void);
        void setCurrentRound(int);
        // int cal_theory_departure_round(Ipv4Header, int);
        // int cal_insert_level(int, int);
	    // int cal_index(int, int); // calculate the index according to the pkt's departureRound and the level

    }
}