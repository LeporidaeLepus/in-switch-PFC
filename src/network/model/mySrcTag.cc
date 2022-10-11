#include "ns3/tag.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/mySrcTag.h"
#include "ns3/log.h"
#include <iostream>


using namespace std;

namespace ns3{
    NS_LOG_COMPONENT_DEFINE("MySrcTag");
    NS_OBJECT_ENSURE_REGISTERED(MySrcTag);

    TypeId MySrcTag::GetTypeId (void)
    {
        static TypeId tid = TypeId ("ns3::MySrcTag")
            .SetParent<Tag> ()
            .AddConstructor<MySrcTag> ()
            .AddAttribute ("SimpleValue",
                   "A simple value",
                    EmptyAttributeValue (),
                    MakeUintegerAccessor (&MySrcTag::GetSimpleValue),
                    MakeUintegerChecker<uint8_t> ())
        ;
        return tid;
    }

    TypeId MySrcTag::GetInstanceTypeId (void) const
    {
       return GetTypeId ();
    }

    MySrcTag::MySrcTag(){
        
    }

    MySrcTag::~MySrcTag(){
        // NS_LOG_FUNCTION(this);
    }

    uint32_t MySrcTag::GetSerializedSize (void) const
    {
        return 1;
    }

    void MySrcTag::Serialize (TagBuffer i) const
    {
        i.WriteU8 (m_simpleValue);
    }

    void MySrcTag::Deserialize (TagBuffer i)
    {
        m_simpleValue = i.ReadU8 ();
    }

    void MySrcTag::Print (std::ostream &os) const
    {
        os << "v=" << (uint32_t)m_simpleValue;
    }

    void MySrcTag::SetSimpleValue (uint8_t value)
    {
        m_simpleValue = value;
    }
    
    uint8_t MySrcTag::GetSimpleValue (void) const
    {
        return m_simpleValue;
    }
}