#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h" // For NetAnim support

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TcpClientServerExample");

int main(int argc, char *argv[])
{
    // Enable logging for info-level messages
    LogComponentEnable("TcpClientServerExample", LOG_LEVEL_INFO);
    LogComponentEnable("PacketSink", LOG_LEVEL_INFO);
    LogComponentEnable("OnOffApplication", LOG_LEVEL_INFO);

    // Create two nodes: client (node 0) and server (node 1)
    NodeContainer nodes;
    // Page No. …………………….. Signature of the Faculty………………………...
    // Roll No: 160123733031 Exp. No: 10 Date: 15/10/2025
    nodes.Create(2);

    // Create point-to-point channel between the nodes
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    NetDeviceContainer devices = pointToPoint.Install(nodes);

    // Install Internet stack on the nodes
    InternetStackHelper stack;
    stack.Install(nodes);

    // Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    // Server listens on this port
    uint16_t port = 5566;

    // Setup TCP server (PacketSink) on node 1
    Address serverAddress(InetSocketAddress(interfaces.GetAddress(1), port));
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory",
                                      InetSocketAddress(Ipv4Address::GetAny(), port));
    ApplicationContainer serverApp = packetSinkHelper.Install(nodes.Get(1));
    serverApp.Start(Seconds(1.0));
    serverApp.Stop(Seconds(10.0));

    // Setup TCP client (OnOffApplication) on node 0
    OnOffHelper clientHelper("ns3::TcpSocketFactory", serverAddress);
    clientHelper.SetAttribute("DataRate", StringValue("2Mbps"));
    clientHelper.SetAttribute("PacketSize", UintegerValue(1024));
    ApplicationContainer clientApp = clientHelper.Install(nodes.Get(0));
    clientApp.Start(Seconds(2.0));
    clientApp.Stop(Seconds(9.0));

    // Enable PCAP tracing (optional)
    pointToPoint.EnablePcapAll("tcp_client_server");

    // NetAnim setup
    // Page No. …………………….. Signature of the Faculty………………………...
    // Roll No: 160123733031 Exp. No: 10 Date: 15/10/2025
    AnimationInterface anim("tcp_client_server.xml");
    anim.SetConstantPosition(nodes.Get(0), 10, 20); // Client node position
    anim.SetConstantPosition(nodes.Get(1), 50, 20); // Server node position

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
