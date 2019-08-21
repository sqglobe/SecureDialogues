#include "eventqueueholder.h"
#include "utils/eventqueue.h"

EventQueueHolder::EventQueueHolder() :
    mChannelEventQueue(make_queue<Channel::ChannelStatus,
                                  void(Channel::ChannelStatus,
                                       const std::string&,
                                       const std::string&)>()) {}

std::shared_ptr<Channel::EventQueue> EventQueueHolder::channelEventQueue()
    const {
  return mChannelEventQueue;
}
