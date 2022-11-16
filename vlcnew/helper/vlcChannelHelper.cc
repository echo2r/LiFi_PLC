/*
 * vlcChannelHelper.cc
 *
 *  Created on: Apr 10, 2016
 *      Author: Adel
 */

#include "vlcChannelHelper.h"

namespace vlc {

vlcChannelHelper::vlcChannelHelper() {
  m_queueFactory.SetTypeId ("ns3::DropTailQueue<Packet>");
  m_deviceFactory.SetTypeId ("ns3::PointToPointNetDevice");
  m_channelFactory.SetTypeId ("ns3::PointToPointChannel");
}

void 
vlcChannelHelper::SetQueue (std::string type,
                              std::string n1, const AttributeValue &v1,
                              std::string n2, const AttributeValue &v2,
                              std::string n3, const AttributeValue &v3,
                              std::string n4, const AttributeValue &v4)
{
  QueueBase::AppendItemTypeIfNotPresent (type, "Packet");

  m_queueFactory.SetTypeId (type);
  m_queueFactory.Set (n1, v1);
  m_queueFactory.Set (n2, v2);
  m_queueFactory.Set (n3, v3);
  m_queueFactory.Set (n4, v4);
}


void vlcChannelHelper::CreateChannel(std::string channelName){
	//NS_LOG_FUNCTION(this);
	this->m_channel[channelName] = CreateObject<vlc_channel>();
}

void vlcChannelHelper::SetChannelParameter(std::string chName,std::string paramName, double value){
	if(paramName=="TEMP"){
		this->m_channel[chName]->SetTemperature(value);
	}
	else if(paramName=="ElectricNoiseBandWidth"){
		this->m_channel[chName]->SetElectricNoiseBandWidth(value);
	}
}

void vlcChannelHelper::SetPropagationLoss(std::string channelName, std::string propagationLossType){
	//NS_LOG_FUNCTION(this);
	if(propagationLossType=="VLCPropagationLoss"){
		this->m_channel[channelName]->SetPropagationLossModel(CreateObject<VLCPropagationLossModel>());
	}
}

void vlcChannelHelper::SetPropagationDelay(std::string channelName, double value){
	//NS_LOG_FUNCTION(this);
	this->m_channel[channelName]->SetPropagationDelay(value);

}

double vlcChannelHelper::GetChannelSNR(std::string chName){
	ns3::Ptr<vlc_NetDeviceRX> rx = DynamicCast<vlc_NetDeviceRX>(m_channel[chName]->GetDevice(1));
	this->m_channel[chName]->DoCalcPropagationLossForSignal(0);
	this->m_channel[chName]->CalculateNoiseVar();
	double snr =  this->m_channel[chName]->GetSNR();
	rx->SetSNRForErrorModel(snr);
	return snr;
}

void vlcChannelHelper::SetChannelWavelength(std::string channelName, int lower, int upper){
	this->m_channel[channelName]->SetWavelength(lower,upper);
}

/*void vlcChannelHelper::AttachTransmitter(std::string chName,std::string TXDevName, ns3::Ptr<vlc_DeviceHelper> devHelper){
	//NS_LOG_FUNCTION(this);
	this->m_channel[chName]->Attach(devHelper->GetTransmitter(TXDevName));
	this->m_channel[chName]->SetPorpagationLossParametersFromTX();
}

void vlcChannelHelper::AttachReceiver(std::string chName,std::string RXDevName, ns3::Ptr<vlc_DeviceHelper> devHelper){
	//NS_LOG_FUNCTION(this);
	this->m_channel[chName]->Attach(devHelper->GetReceiver(RXDevName));
	this->m_channel[chName]->SetPorpagationLossParametersFromRX();
}*/

void vlcChannelHelper::AttachTransmitter(std::string chName,std::string TXDevName, ns3::Ptr<vlc_DeviceHelper> devHelper) {
	//NS_LOG_FUNCTION(this);
	//this->m_channel[chName]->Attach(devHelper->GetTransmitter(TXDevName));
	/*ns3::Ptr < VlcPropagationLossModel > loss = ns3::DynamicCast
			< VlcPropagationLossModel
			> (m_channel[chName]->GetPropagationLossModel());
	loss->SetTxPowerMAX(devHelper->GetTransmitter(TXDevName)->GetTXPowerMax());
	loss->SetTXGain(devHelper->GetTransmitter(TXDevName)->GetTXGain());

	//this->m_channel[chName]->SetPropagationLossParametersFromTX(devHelper->GetTransmitter(TXDevName));*/
}

void vlcChannelHelper::AttachReceiver(std::string chName,std::string RXDevName, ns3::Ptr<vlc_DeviceHelper> devHelper) {
	//NS_LOG_FUNCTION(this);
	//this->m_channel[chName]->Attach(devHelper->GetReceiver(RXDevName));

	/*ns3::Ptr < VlcPropagationLossModel > loss = ns3::DynamicCast
			< VlcPropagationLossModel
			> (m_channel[chName]->GetPropagationLossModel());

	loss->SetFilterGain(devHelper->GetReceiver(RXDevName)->GetFilterGain());

	loss->SetConcentratorGain(
			devHelper->GetReceiver(RXDevName)->GetConcentrationGain());

	loss->SetRXGain(devHelper->GetReceiver(RXDevName)->GetRXGain());

	loss->SetArea(devHelper->GetReceiver(RXDevName)->GetPhotoDetectorArea());*/

	//this->m_channel[chName]->SetPropagationLossParametersFromRX(devHelper->GetReceiver(RXDevName));
}

ns3::Ptr<vlc_channel> vlcChannelHelper::GetChannel(std::string chName){
	//NS_LOG_FUNCTION(this);
	return m_channel[chName];
}

ns3::Ptr< vlc_NetDevice > vlcChannelHelper::GetDevice(std::string chName,uint32_t idx){
	//NS_LOG_FUNCTION(this);
	ns3::Ptr<vlc_NetDevice> ans = DynamicCast<vlc_NetDevice>(m_channel[chName]->GetDevice(idx));

	return ans;
}

NetDeviceContainer vlcChannelHelper::Install(std::string chName,Ptr<Node> a, Ptr<Node> b){

	//NS_LOG_FUNCTION(this);

	NetDeviceContainer container;
	Ptr<vlc_channel> ch = this->m_channel[chName];

	Ptr<vlc_NetDevice> devTX =  DynamicCast<vlc_NetDevice>(ch->GetDevice(0) );
	Ptr<vlc_NetDevice> devRX =  DynamicCast<vlc_NetDevice>(ch->GetDevice(1) );


	a->AddDevice(devTX);
	//Ptr < Queue > queueA = m_queueFactory.Create<Queue>();
	//Ptr < DropTailQueue > queueA = CreateObject<DropTailQueue>();
	//devTX->SetQueue(queueA);

	b->AddDevice(devRX);
	//Ptr < Queue > queueB = m_queueFactory.Create<Queue>();
	//devRX->SetQueue(queueB);

	/*bool useNormalChannel = true;
	ch = 0;

	if (MpiInterface::IsEnabled()) {
		uint32_t n1SystemId = a->GetSystemId();
		uint32_t n2SystemId = b->GetSystemId();
		uint32_t currSystemId = MpiInterface::GetSystemId();

		if (n1SystemId != currSystemId || n2SystemId != currSystemId) {
			useNormalChannel = false;
		}
	}*/


	container.Add(devTX);
	container.Add(devRX);

	return container;

}
/*
NetDeviceContainer vlcChannelHelper::Install(std::string chName, Ptr<Node> a,
		Ptr<Node> b) {
	NetDeviceContainer container;
	Ptr < vlc_channel > ch = this->m_channel[chName];

	Ptr < vlc_NetDeviceTX > devTX = DynamicCast < vlc_NetDeviceTX
			> (ch->GetDevice(0));
	Ptr < vlc_NetDeviceRX > devRX = DynamicCast < vlc_NetDeviceRX
			> (ch->GetDevice(1));

	devTX->SetAddress(Mac48Address::Allocate());
	a->AddDevice(devTX);
	//Ptr < Queue > queueA = m_queueFactory.Create<Queue>();
	//devTX->SetQueue(queueA);

	devRX->SetAddress(Mac48Address::Allocate());
	b->AddDevice(devRX);
	//Ptr < Queue > queueB = m_queueFactory.Create<Queue>();
	//devRX->SetQueue(queueB);

	devTX->AttachChannel(ch);
	devRX->AttachChannel(ch);

	container.Add(devTX);
	container.Add(devRX);

	return container;
}*/

NetDeviceContainer vlcChannelHelper::Install(Ptr<Node> a, Ptr<Node> b,
		Ptr<vlc_DeviceHelper> devHelper, Ptr<vlcChannelHelper> chHelper,
		std::string txName, std::string rxName, std::string chName) {

	NetDeviceContainer container;
	devHelper->GetTransmitter(txName)->SetAddress(Mac48Address::Allocate());

	a->AddDevice(devHelper->GetTransmitter(txName));
	Ptr<Queue<Packet> > queueA = m_queueFactory.Create<Queue<Packet> > ();
	devHelper->GetTransmitter(txName)->SetQueue(queueA);
	devHelper->GetReceiver(rxName)->SetAddress(Mac48Address::Allocate());

	b->AddDevice(devHelper->GetReceiver(rxName));
	Ptr<Queue<Packet> > queueB = m_queueFactory.Create<Queue<Packet> > ();
	devHelper->GetReceiver(rxName)->SetQueue(queueB);

	bool useNormalChannel = true;

	Ptr < vlc_channel > channel = 0;
	if (useNormalChannel) {
		channel = chHelper->GetChannel(chName);
		m_subChannel = CreateObject<PointToPointChannel>();
	}

	devHelper->GetTransmitter(txName)->AttachChannel(channel);
	devHelper->GetTransmitter(txName)->Attach(m_subChannel);
	channel->Attach(devHelper->GetTransmitter(txName));

	devHelper->GetReceiver(rxName)->AttachChannel(channel);
	devHelper->GetReceiver(rxName)->Attach(m_subChannel);
	channel->Attach(devHelper->GetReceiver(rxName));

	container.Add(devHelper->GetTransmitter(txName));
	container.Add(devHelper->GetReceiver(rxName));

	return container;
}

vlcChannelHelper::~vlcChannelHelper() {

}

} /* namespace vlc */
