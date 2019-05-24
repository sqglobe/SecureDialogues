#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <eventpp/eventqueue.h>
#include <memory>
#include <thread>

template <typename Key,
          typename Prototype,
          typename Policies = eventpp::DefaultPolicies>
std::shared_ptr<eventpp::EventQueue<Key, Prototype, Policies>> make_queue();

template <typename Key, typename Prototype, typename Policies>
std::shared_ptr<eventpp::EventQueue<Key, Prototype, Policies>> make_queue() {
  using EventQueueType = eventpp::EventQueue<Key, Prototype, Policies>;
  auto res = std::make_shared<EventQueueType>();
  auto thread_func = [](const std::shared_ptr<EventQueueType>& sh_ptr) {
    auto queue_ptr = std::weak_ptr<EventQueueType>(sh_ptr);
    while (true) {
      if (auto ptr = queue_ptr.lock(); !ptr) {
        break;
      } else {
        ptr->wait();
        ptr->process();
      }
    }
  };

  std::thread th(thread_func, res);
  th.detach();
  return res;
}

#endif  // EVENTQUEUE_H
