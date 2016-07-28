// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fastrtps/rtps/network/NetworkFactory.h>
#include <fastrtps/transport/UDPv4Transport.h>
#include <fastrtps/transport/UDPv6Transport.h>
#include <fastrtps/transport/test_UDPv4Transport.h>
#include <utility>
using namespace std;

namespace eprosima{
namespace fastrtps{
namespace rtps{

vector<SenderResource> NetworkFactory::BuildSenderResources(const Locator_t& local)
{
   vector<SenderResource> newSenderResources; 

   for(auto& transport : mRegisteredTransports)
   {
      if ( transport->IsLocatorSupported(local) &&
          !transport->IsOutputChannelOpen(local) )
      {
         SenderResource newSenderResource(*transport, local);
         if (newSenderResource.mValid)
            newSenderResources.push_back(move(newSenderResource));
      }
   }
   return newSenderResources;
}

vector<SenderResource> NetworkFactory::BuildSenderResourcesForRemoteLocator(const Locator_t& remote)
{
   vector<SenderResource> newSenderResources; 

   for(auto& transport : mRegisteredTransports)
   {
      Locator_t local = transport->RemoteToMainLocal(remote);
      if ( transport->IsLocatorSupported(local) &&
          !transport->IsOutputChannelOpen(local) )
      {
         SenderResource newSenderResource(*transport, local);
         if (newSenderResource.mValid)
            newSenderResources.push_back(move(newSenderResource));
      }
   }
   return newSenderResources;
}

vector<ReceiverResource> NetworkFactory::BuildReceiverResources(const Locator_t& local)
{
   vector<ReceiverResource> newReceiverResources; 

   for(auto& transport : mRegisteredTransports)
   {
      if ( transport->IsLocatorSupported(local) &&
          !transport->IsInputChannelOpen(local) )
      {
         ReceiverResource newReceiverResource(*transport, local);
         if (newReceiverResource.mValid)
            newReceiverResources.push_back(move(newReceiverResource));
      }
   }
   return newReceiverResources;
}

void NetworkFactory::RegisterTransport(const TransportDescriptorInterface* descriptor)
{
   if (auto concrete = dynamic_cast<const UDPv4TransportDescriptor*> (descriptor))
      mRegisteredTransports.emplace_back(new UDPv4Transport(*concrete));
   if (auto concrete = dynamic_cast<const UDPv6TransportDescriptor*> (descriptor))
      mRegisteredTransports.emplace_back(new UDPv6Transport(*concrete));
   if (auto concrete = dynamic_cast<const test_UDPv4TransportDescriptor*> (descriptor))
      mRegisteredTransports.emplace_back(new test_UDPv4Transport(*concrete));
}

} // namespace rtps
} // namespace fastrtps
} // namespace eprosima