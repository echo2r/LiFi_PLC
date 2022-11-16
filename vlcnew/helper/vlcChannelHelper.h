/*
 * vlcChannelHelper.h
 *
 *  Created on: Apr 10, 2016
 *      Author: adel
 */

#ifndef VLCCHANNELHELPER_H_
#define VLCCHANNELHELPER_H_

#include <map>
#include <iostream>
#include <string>
#include "ns3/core-module.h"
#include "ns3/vlcchannel.h"
#include "ns3/vlcDeviceHelper.h"
#include "ns3/net-device-container.h"
#include "ns3/queue.h"
#include "ns3/vlcNetDevice.h"
#include "ns3/vlcNetDeviceRX.h"
#include "ns3/vlcNetDeviceTX.h"
#include "ns3/object-factory.h"

#include <string>

#include "ns3/object-factory.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"

#include "ns3/trace-helper.h"


namespace vlc {

class vlcChannelHelper : public Object {
public:
	vlcChannelHelper();

void SetQueue (std::string type,
                 std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                 std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                 std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                 std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue ());


	void CreateChannel(std::string channelName);

	void SetChannelWavelength(std::string channelName, int lower, int upper);

	void SetPropagationLoss(std::string channelName, std::string propagationLossType);

	void SetPropagationDelay(std::string channelName, double value);

	void AttachTransmitter(std::string chName,std::string TXDevName, ns3::Ptr<vlc_DeviceHelper> devHelper);

	void AttachReceiver(std::string chName,std::string RXDevName, ns3::Ptr<vlc_DeviceHelper> devHelper);

	double GetChannelSNR(std::string chName);

	void SetChannelParameter(std::string chName,std::string paramName, double value);

	ns3::Ptr<vlc_channel> GetChannel(std::string chName);

	ns3::Ptr< vlc_NetDevice > GetDevice(std::string chName,uint32_t idx);

	ns3::NetDeviceContainer Install(std::string chName,Ptr<Node> a, Ptr<Node> b);
	/*ns3::NetDeviceContainer Install(std::string chName, Ptr<Node> a,
			Ptr<Node> b);

	NetDeviceContainer Install(Ptr<Node> a, Ptr<Node> b, Ptr<vlc_NetDeviceTX> tx,
			Ptr<vlc_NetDeviceRX> rx);*/

	NetDeviceContainer Install(Ptr<Node> a, Ptr<Node> b,
			Ptr<vlc_DeviceHelper> devHelper, Ptr<vlcChannelHelper> chHelper,
			std::string txName, std::string rxName, std::string chName);
			
	virtual ~vlcChannelHelper();

private:

	std::map< std::string, ns3::Ptr<vlc_channel> > m_channel;
        ObjectFactory m_deviceFactory; 
	ObjectFactory m_queueFactory;
	ObjectFactory m_channelFactory;
	Ptr<PointToPointChannel> m_subChannel;

};

} /* namespace vlc */

#endif /* VLCCHANNELHELPER_H_ */
