#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace NS3;

NS_LOG_COMPONENT_DEFINE ("MyTopo");

const int nSERVER = 3;
const int nCLIENT = 1;
const int nROUTER = 1;

int
mian (int argc, char *argv[])
{
    bool verbose = true;
    
    CommandLine cmd;
    cmd.Parse (argc, argv);
    Time::SetResolution (Time::NS);

    // if (verbose)
    // {
    //     LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    //     LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    // }


}



static void
RxDrop(Ptr<const Packet> packet) {
    /* 
     * Need to copy packet since headers need to be removed
     * to be inspected. Alternatively, remove the headers,
     * and add them back.
     */
    Ptr<Packet> copy = packet->Copy();

    // Headers must be removed in the order they're present.
    PppHeader pppHeader;
    copy->RemoveHeader(pppHeader);
    Ipv4Header ipHeader;
    copy->RemoveHeader(ipHeader);

    std::cout << "Source IP: ";
    ipHeader.GetSource().Print(std::cout);
    std::cout << std::endl;

    std::cout << "Destination IP: ";
    ipHeader.GetDestination().Print(std::cout);
    std::cout << std::endl;
}