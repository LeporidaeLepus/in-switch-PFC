#include "ns3/tag.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/myTag.h"
#include <iostream>

namespace ns3{
    NS_LOG_COMPONENT_DEFINE("myTag");
    NS_OBJECT_ENSURE_REGISTERED(myTag);

    TypeId MyTag::GetTypeId (void)
    {
        static TypeId tid = TypeId ("ns3::MyTag")
            .SetParent<Tag> ()
            .AddConstructor<MyTag> ()
            .AddAttribute ("SimpleValue",
                   "A simple value",
                    EmptyAttributeValue (),
                    MakeUintegerAccessor (&MyTag::GetSimpleValue),
                    MakeUintegerChecker<uint8_t> ())
        ;
   return tid;
    }

    MyTag::MyTag(){
        
    }

    void MyTag::Serialize (TagBuffer i) const
    {
        i.WriteU8 (m_simpleValue);
    }

    void MyTag::Deserialize (TagBuffer i)
    {
        m_simpleValue = i.ReadU8 ();
    }

    void MyTag::SetSimpleValue (uint8_t value)
    {
        m_simpleValue = value;
    }
    
    uint8_t MyTag::GetSimpleValue (void) const
    {
        return m_simpleValue;
    }
}