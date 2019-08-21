#ifndef EVENTQUEUEHOLDER_H
#define EVENTQUEUEHOLDER_H

#include <eventpp/eventqueue.h>
#include <memory>
#include <string>
#include "communication/channel.h"

class EventQueueHolder {
 public:
  EventQueueHolder();

  std::shared_ptr<Channel::EventQueue> channelEventQueue() const;

 private:
  std::shared_ptr<Channel::EventQueue> mChannelEventQueue;
};

#endif  // EVENTQUEUEHOLDER_H
