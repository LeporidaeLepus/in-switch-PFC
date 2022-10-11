#include "ns3/tag.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/myDstTag.h"
#include "ns3/log.h"
#include <iostream>


using namespace std;

namespace ns3{
    NS_LOG_COMPONENT_DEFINE("MyDstTag");
    NS_OBJECT_ENSURE_REGISTERED(MyDstTag);

    TypeId MyDstTag::GetTypeId (void)
    {
        static TypeId tid = TypeId ("ns3::MyDstTag")
            .SetParent<Tag> ()
            .AddConstructor<MyDstTag> ()
            .AddAttribute ("SimpleValue",
                   "A simple value",
                    EmptyAttributeValue (),
                    MakeUintegerAccessor (&MyDstTag::GetSimpleValue),
                    MakeUintegerChecker<uint8_t> ())
        ;
        return tid;
    }

    TypeId MyDstTag::GetInstanceTypeId (void) const
    {
       return GetTypeId ();
    }

    MyDstTag::MyDstTag(){
        
    }

    MyDstTag::~MyDstTag(){
        // NS_LOG_FUNCTION(this);
    }

    uint32_t MyDstTag::GetSerializedSize (void) const
    {
        return 1;
    }

    void MyDstTag::Serialize (TagBuffer i) const
    {
        i.WriteU8 (m_simpleValue);
    }

    void MyDstTag::Deserialize (TagBuffer i)
    {
        m_simpleValue = i.ReadU8 ();
    }

    void MyDstTag::Print (std::ostream &os) const
    {
        os << "v=" << (uint32_t)m_simpleValue;
    }

    void MyDstTag::SetSimpleValue (uint8_t value)
    {
        m_simpleValue = value;
    }
    
    uint8_t MyDstTag::GetSimpleValue (void) const
    {
        return m_simpleValue;
    }
}