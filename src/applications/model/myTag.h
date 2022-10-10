#ifndef _MY_TAG_H_
#define _MY_TAG_H_
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

        //override abstract methods in base class ‘Tag’
        virtual uint32_t GetSerializedSize (void) const;
        virtual void Serialize (TagBuffer i) const;
        virtual void Deserialize (TagBuffer i);
        virtual void Print (std::ostream &os) const;
        
        // these are our accessors to our tag structure
        void SetSimpleValue (uint8_t value);
        uint8_t GetSimpleValue (void) const;
    };
}

#endif //_MY_TAG_H_