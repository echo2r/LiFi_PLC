/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <fstream>
#include <string>
#include <ctype.h>
#include <iostream>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/aodv-module.h"
#include "ns3/dsdv-module.h"
#include "ns3/dsr-module.h"

#include "ns3/vlcChannelHelper.h"
#include "ns3/vlcDeviceHelper.h"
#include "ns3/netanim-module.h"

#include "ns3/plc.h"


// Default Network Topology
//
//  Lifi 192.168.0.0
//          AP
//  *       *
//  |       |    10.1.2.0
//  n6     n5 -------------- n0   n1   n2   n3   n4   
//            point-to-point  |    |    |    |    |
//                            =====================
//                                PLC 172.16.0.0

using namespace ns3;
using namespace vlc;

NS_LOG_COMPONENT_DEFINE ("LiFi-PLC");

static void SinkRx (Ptr<const Packet> p, const Address &ad)
{
  // A packet has been received at the receive sink
  std::cout << Simulator::Now().As (Time::S) << " " << *p << " received" <<std::endl;
}

int 
main (int argc, char *argv[])
{
  uint32_t cable_lens = 5;
  uint32_t stopTime = 14;
  double PhotoDetectorArea = (1.3e-5); 	
  double Band_factor_Noise_Signal = (10.0);
  bool verbose = true;
  bool tracing = true;
  bool useTraceCallback = true;
  
  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // The underlying restriction of 18 is due to the grid position
  // allocator's configuration; the grid layout will exceed the
  // bounding box if more than 18 nodes are provided.

  if (verbose)
    {
      LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
    }
  
  
  //configure PLC nodes
  PLC_SpectrumModelHelper smHelper;
  Ptr<const SpectrumModel> sm;
  sm = smHelper.GetSpectrumModel(0, 10e6, 100);

  Ptr<SpectrumValue> txPsd = Create<SpectrumValue> (sm);
  (*txPsd) = 1e-9;   //-50dBm/Hz
  Ptr<PLC_Cable> cable = CreateObject<PLC_AL3x95XLPE_Cable> (sm);
  /*AL3x95XLPE,NAYY150SE,NAYY50SE*/
  Ptr<PLC_ConstImpedance> shuntImp1 = Create<PLC_ConstImpedance> (sm,
  PLC_Value(40, 0)); 
  Ptr<PLC_ConstImpedance> shuntImp2 = Create<PLC_ConstImpedance> (sm,
  PLC_Value(250, 0)); 
  Ptr<PLC_ConstImpedance> shuntImp3 = Create<PLC_ConstImpedance> (sm,
  PLC_Value(250, 0)); 
  Ptr<PLC_ConstImpedance> shuntImp4 = Create<PLC_ConstImpedance> (sm,
  PLC_Value(250, 0));
  Ptr<PLC_ConstImpedance> shuntImp5 = Create<PLC_ConstImpedance> (sm,
  PLC_Value(40, 0));
  
  Ptr<PLC_Node> n1 = CreateObject<PLC_Node> ();
  Ptr<PLC_Node> n2 = CreateObject<PLC_Node> ();
  Ptr<PLC_Node> n3 = CreateObject<PLC_Node> ();
  Ptr<PLC_Node> n4 = CreateObject<PLC_Node> ();
  Ptr<PLC_Node> n5 = CreateObject<PLC_Node> ();

  n1->SetPosition(0,0,0);
  n2->SetPosition(3*cable_lens,0,0);
  n3->SetPosition(6*cable_lens,0,0);
  n4->SetPosition(9*cable_lens,0,0);
  n5->SetPosition(12*cable_lens,0,0);

  n1->SetName("Node1");
  n2->SetName("Node2");
  n3->SetName("Node3");
  n4->SetName("Node4");
  n5->SetName("Node5");

  PLC_NodeList nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);
  nodes.push_back(n5);

  CreateObject<PLC_Line> (cable, n1, n2);
  CreateObject<PLC_Line> (cable, n2, n3);
  CreateObject<PLC_Line> (cable, n3, n4);
  CreateObject<PLC_Line> (cable, n4, n5);

  PLC_ChannelHelper channelHelper(sm);
  channelHelper.Install(nodes);
  Ptr<PLC_Channel> channel = channelHelper.GetChannel();

  Ptr<PLC_Outlet> outlet1 = CreateObject<PLC_Outlet> (n1, shuntImp1);
  Ptr<PLC_Outlet> outlet2 = CreateObject<PLC_Outlet> (n2, shuntImp2);
  Ptr<PLC_Outlet> outlet3 = CreateObject<PLC_Outlet> (n3, shuntImp3);
  Ptr<PLC_Outlet> outlet4 = CreateObject<PLC_Outlet> (n4, shuntImp4);
  Ptr<PLC_Outlet> outlet5 = CreateObject<PLC_Outlet> (n5, shuntImp4);
  
  Ptr<SpectrumValue> noiseFloor= Create<SpectrumValue> (sm);
  (*noiseFloor) = 1e-12;
  
  PLC_NetDeviceHelper deviceHelper(sm, txPsd, nodes);
  deviceHelper.SetNoiseFloor(noiseFloor);
  deviceHelper.DefinePhyType(TypeId::LookupByName ("ns3::PLC_InformationRatePhy"));
  deviceHelper.DefineMacType(TypeId::LookupByName ("ns3::PLC_ArqMac"));
  deviceHelper.SetHeaderModulationAndCodingScheme(ModulationAndCodingScheme(BPSK_1_4,0));
           deviceHelper.SetPayloadModulationAndCodingScheme(ModulationAndCodingScheme(BPSK_1_2,0));
  deviceHelper.Setup();
 
  channel->InitTransmissionChannels();
  channel->CalcTransmissionChannels();
  
  PLC_Interference interference;
  interference.SetNoiseFloor(noiseFloor);
 
  NodeContainer PLCNodes;
  PLCNodes=deviceHelper.GetNS3Nodes();

  NetDeviceContainer PLCDevices;
  PLCDevices = deviceHelper.GetNetDevices();

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install(PLCNodes);
  
  //configure p2p nodes
  NodeContainer p2pNodes;
  p2pNodes.Add (PLCNodes.Get(0));
  p2pNodes.Create (1);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1000Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);
  
  //configure VLC nodes
  NodeContainer vlcnodes;
  vlcnodes.Add (p2pNodes.Get(1));
  vlcnodes.Create (1);
  
  vlc_DeviceHelper devHelperVPPM;
  devHelperVPPM.CreateTransmitter("THE_TRANSMITTER1");
  devHelperVPPM.SetTXSignal("THE_TRANSMITTER1", 1000, 0.5, 0, 9.25e-5, 0);
  devHelperVPPM.SetTrasmitterParameter("THE_TRANSMITTER1", "Bias", 0);
  devHelperVPPM.SetTrasmitterParameter("THE_TRANSMITTER1", "SemiAngle", 35);
  devHelperVPPM.SetTrasmitterParameter("THE_TRANSMITTER1", "Azimuth", 0);
  devHelperVPPM.SetTrasmitterParameter("THE_TRANSMITTER1", "Elevation",180.0);
  devHelperVPPM.SetTrasmitterPosition("THE_TRANSMITTER1", 0.0, 0.0, 52.0);
  devHelperVPPM.SetTrasmitterParameter("THE_TRANSMITTER1", "Gain", 70);
  devHelperVPPM.SetTrasmitterParameter("THE_TRANSMITTER1", "DataRateInMBPS",0.3);
  
  devHelperVPPM.CreateReceiver("THE_RECEIVER1");
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "FilterGain", 1);
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "RefractiveIndex", 1.5);
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "FOVAngle", 28.5);
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "ConcentrationGain", 0);
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "PhotoDetectorArea",1.3e-5);
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "RXGain", 0);
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "PhotoDetectorArea",PhotoDetectorArea);
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "Beta", 1); 
  devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "SetModulationScheme",VLC_ErrorModel::PAM);
  //devHelperVPPM.SetReceiverParameter("THE_RECEIVER1", "DutyCycle", 0.85); //Dutycycle is only valid for VPPM

  vlcChannelHelper chHelper;
  chHelper.CreateChannel("THE_CHANNEL1");
  chHelper.SetPropagationLoss("THE_CHANNEL1", "VlcPropagationLoss");
  chHelper.SetPropagationDelay("THE_CHANNEL1", 2);
  chHelper.AttachTransmitter("THE_CHANNEL1", "THE_TRANSMITTER1",&devHelperVPPM);
  chHelper.AttachReceiver("THE_CHANNEL1", "THE_RECEIVER1", &devHelperVPPM);
  chHelper.SetChannelParameter("THE_CHANNEL1", "TEMP", 295);
  chHelper.SetChannelParameter("THE_CHANNEL1", "BAND_FACTOR_NOISE_SIGNAL",Band_factor_Noise_Signal);
  chHelper.SetChannelWavelength("THE_CHANNEL1", 380, 780);
  chHelper.SetChannelParameter("THE_CHANNEL1", "ElectricNoiseBandWidth",3 * 1e5);
	
  NetDeviceContainer vlcdevices;
  vlcdevices = chHelper.Install(vlcnodes.Get(0), vlcnodes.Get(1),
			&devHelperVPPM, &chHelper, "THE_TRANSMITTER1", "THE_RECEIVER1",
			"THE_CHANNEL1");

  Ptr < ListPositionAllocator > m_listPosition = CreateObject<
		ListPositionAllocator>();
  m_listPosition->Add(Vector(250.0, 500.0, 0.0));
  m_listPosition->Add(Vector(500.0, 500.0, 0.0));
  m_listPosition->Add(Vector(750.0, 500.0, 0.0));

  mobility.SetPositionAllocator(m_listPosition);
  mobility.SetMobilityModel("ns3::VlcMobilityModel");

  mobility.Install(vlcnodes);
		
  InternetStackHelper stack;
  AodvHelper aodv;
  stack.SetRoutingHelper (aodv);
  
  stack.Install (PLCNodes);
  stack.Install (vlcnodes);

  Ipv4AddressHelper address;

  address.SetBase ("172.16.0.0", "255.255.255.0");
  Ipv4InterfaceContainer PLCInterfaces;
  PLCInterfaces = address.Assign (PLCDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("192.168.0.0", "255.255.255.0");
  Ipv4InterfaceContainer vlcInterfaces;
  vlcInterfaces = address.Assign (vlcdevices);
  
  //onoff appllication
  NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9; 

  Ptr<Node> appSource = NodeList::GetNode (4);
  Ptr<Node> appSink = NodeList::GetNode (6);
  Ipv4Address remoteAddr = appSink -> GetObject<Ipv4>() -> GetAddress (1,0).GetLocal(); 

  OnOffHelper onoff ("ns3::UdpSocketFactory",Address (InetSocketAddress(remoteAddr,port)));
  Config::SetDefault ("ns3::OnOffApplication::PacketSize", StringValue ("64"));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("512b/s"));

  ApplicationContainer apps = onoff.Install (appSource);
  apps.Start (Seconds (4));
  apps.Stop (Seconds (stopTime - 1));

  PacketSinkHelper sink ("ns3::UdpSocketFactory",
  InetSocketAddress (Ipv4Address::GetAny (), port));
  apps = sink.Install (appSink);
  apps.Start (Seconds (5));

  NS_LOG_INFO ("Configure Tracing.");
  if (useTraceCallback == true)
  {
  Config::ConnectWithoutContext ("/NodeList/6/ApplicationList/0/$ns3::PacketSink/Rx", 
                                 MakeCallback (&SinkRx));
  Packet::EnablePrinting ();
  }
  
  //Ascii Tracing
  AsciiTraceHelper ascii;
  stack.EnableAsciiIpv4All (ascii.CreateFileStream ("stack.tr"));
  stack.EnablePcapIpv4All("stackCap");

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (stopTime));
  Simulator::Run ();
  Simulator::Destroy ();
}
