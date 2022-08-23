#include <iostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"
// #include "ns3/VirtualQueue.h"
// #include "ns3/QueueCreate.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//

using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
 
int
main (int argc, char *argv[]){
    CommandLine cmd;
    cmd.Parse (argc, argv);
  
    Time::SetResolution (Time::NS);
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
 
    NodeContainer nodes;
    nodes.Create (3);

    std::vector<NodeContainer> nodeAdjacencyList(2);
    nodeAdjacencyList[0]=NodeContainer(nodes.Get(0), nodes.Get(1));
    nodeAdjacencyList[1]=NodeContainer(nodes.Get(2), nodes.Get(1));
    
    // PointToPointHelper pointToPoint;
    std::vector<PointToPointHelper> pointToPoint(2);
    // pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    // pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    for (int i = 0; i < 2; i++){      
        pointToPoint[i].SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
        pointToPoint[i].SetChannelAttribute ("Delay", StringValue ("2ms"));
    } 
 
    // NetDeviceContainer devices;
    // devices = pointToPoint.Install (nodes);
    std::vector<NetDeviceContainer> devices(2);
    for (int i = 0; i < 2; i ++){
        devices[i] = pointToPoint[i].Install(nodeAdjacencyList[i]);
    }
 
    InternetStackHelper stack;
    stack.Install (nodes);
 
    Ipv4AddressHelper address;
    // address.SetBase ("10.1.1.0", "255.255.255.0");
 
    // Ipv4InterfaceContainer interfaces = address.Assign (devices);
    std::vector<Ipv4InterfaceContainer> interfaces(2);
    for (uint32_t i = 0; i < 2; i++)  
    {  
        std::ostringstream subset;  
        subset << "10.1." << i + 1 << ".0"; 
        // n0: 10.1.1.1    n1 NIC1: 10.1.1.2   n1 NIC2: 10.1.2.1   n2 NIC1: 10.1.2.2  ...
        address.SetBase(subset.str().c_str (), "255.255.255.0"); // sebnet & mask  
        interfaces[i] = address.Assign(devices[i]); // set ip addresses to NICs: 10.1.1.1, 10.1.1.2 ...  
    }  

    TrafficControlHelper tch;
    for (int i = 0; i < 2; i++){
        tch.Uninstall(devices[i]);
    }

    tch.SetRootQueueDisc ("ns3::VXQ");
    tch.Install(devices[0].Get(1));

    UdpEchoServerHelper echoServer (9);

    ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

    UdpEchoClientHelper echoClient (interfaces[0].GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (5));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));
    
    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}