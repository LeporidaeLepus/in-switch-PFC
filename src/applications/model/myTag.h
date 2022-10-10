#ifndef _QUEUE_CREATE_H_
#define _QUEUE_CREATE_H_
#include "ns3/tag.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include <iostream>

using namespace std;

namespace ns3{
    class MyTag : public Tag
    {
    private:
        uint8_t m_simpleValue;
    public:
        static TypeId GetTypeId (void);

        MyTag();
        ~MyTag();
        
        virtual void Serialize (TagBuffer i) const;
        virtual void Deserialize (TagBuffer i);
        // these are our accessors to our tag structure
        void SetSimpleValue (uint8_t value);
        uint8_t GetSimpleValue (void) const;
    };
}

#endif //_QUEUE_CREATE_H_