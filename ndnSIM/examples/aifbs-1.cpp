#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"

#include "ns3/ndnSIM-module.h"
#include "ns3/position-allocator.h"

using namespace std;
namespace ns3 {

NS_LOG_COMPONENT_DEFINE("ndn.WifiExample");

int main(int argc, char* argv[]) {
	// disable fragmentation
	Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue("2200"));
	Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("2200"));
	Config::SetDefault("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue("OfdmRate24Mbps"));

	CommandLine cmd;
	cmd.Parse(argc, argv);

	WifiHelper wifi = WifiHelper::Default();
	wifi.SetStandard(WIFI_PHY_STANDARD_80211a);
	wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue("OfdmRate24Mbps"));

	YansWifiChannelHelper wifiChannel;
	wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
	wifiChannel.AddPropagationLoss("ns3::ThreeLogDistancePropagationLossModel");
	wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel");

	YansWifiPhyHelper wifiPhyHelper = YansWifiPhyHelper::Default();
	wifiPhyHelper.SetChannel(wifiChannel.Create());
	wifiPhyHelper.Set("TxPowerStart", DoubleValue(5));
	wifiPhyHelper.Set("TxPowerEnd", DoubleValue(5));

	NqosWifiMacHelper wifiMacHelper = NqosWifiMacHelper::Default();
	wifiMacHelper.SetType("ns3::AdhocWifiMac");

//	Ptr<UniformRandomVariable> randomizer = CreateObject<UniformRandomVariable>();
//	randomizer->SetAttribute("Min", DoubleValue(10));
//	randomizer->SetAttribute("Max", DoubleValue(100));

	NodeContainer nodes;
	nodes.Create(3);

	// 1. Install Wifi
	NetDeviceContainer wifiNetDevices = wifi.Install(wifiPhyHelper, wifiMacHelper, nodes);
	wifiPhyHelper.EnablePcap("wave-simple-80211a", wifiNetDevices);

	MobilityHelper mobility;
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
	positionAlloc->Add(Vector(0.0, 0.0, 0.0));
	positionAlloc->Add(Vector(125.0, 40.0, 0.0));
	positionAlloc->Add(Vector(250, 0.0, 0.0));
	mobility.SetPositionAllocator(positionAlloc);
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

	// 2. Install Mobility model
	mobility.Install(nodes);

	// 3. Install NDN stack
	NS_LOG_INFO("Installing NDN stack");
	ndn::StackHelper ndnHelper;
	ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "1000");
	ndnHelper.SetDefaultRoutes(true);
	ndnHelper.Install(nodes);

	// Set BestRoute strategy
	ndn::StrategyChoiceHelper::Install(nodes, "/", "/localhost/nfd/strategy/best-route");
	// 4. Set up applications
	NS_LOG_INFO("Installing Applications");

	ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
	consumerHelper.SetPrefix("/test/");
	consumerHelper.SetAttribute("Frequency", DoubleValue(1.0)); // no. of interests per second
	consumerHelper.Install(nodes.Get(0));

//	ndn::AppHelper consumerHelper1("ns3::ndn::ConsumerCbr");
//	consumerHelper1.SetPrefix("/test/");
//	consumerHelper1.SetAttribute("Frequency", DoubleValue(1.0)); // no. of interests per second
//	consumerHelper1.Install(nodes.Get(1));

	ndn::AppHelper producerHelper("ns3::ndn::Producer");
	producerHelper.SetPrefix("/test/");
	producerHelper.SetAttribute("PayloadSize", StringValue("1200"));
	producerHelper.Install(nodes.Get(2));

	// 5. Running simulation
	Simulator::Stop(Seconds(20));

	ndn::AppDelayTracer::InstallAll("app-delays-trace.txt");
	ndn::CsTracer::InstallAll("cs-trace.txt", Seconds(1));
	ndn::L3RateTracer::InstallAll("rate-trace.txt", Seconds(1.0));
	L2RateTracer::InstallAll("drop-trace.txt", Seconds(0.5));

	Simulator::Run();
	Simulator::Destroy();

	return 0;
}

}

int main(int argc, char* argv[]) {
	return ns3::main(argc, argv);
}
