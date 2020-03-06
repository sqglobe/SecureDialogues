#ifndef PLAGINFACADE_H
#define PLAGINFACADE_H

class PluginWidget;
class PluginConnectionInfo;
class PluginConnectionSerializer;
class PluginMessageCommunicator;
class RecievedMessagesIterator;
class PluginAddressValidator;

class PluginFacade {
 public:
  virtual ~PluginFacade() = default;

 public:
  virtual const char* getId() const noexcept = 0;
  virtual const char* getPluginName() const noexcept = 0;

 public:
  virtual PluginWidget* getWidget() const noexcept = 0;
  virtual const PluginConnectionSerializer* getSerializer() const noexcept = 0;
  virtual PluginMessageCommunicator* makeCommunicator() noexcept = 0;
  virtual PluginConnectionInfo* makeEmptyConn() noexcept = 0;
  virtual const PluginAddressValidator* getAddressValidator() const
      noexcept = 0;

 public:
  virtual void release(PluginMessageCommunicator* comm) noexcept = 0;
  virtual void release(PluginConnectionInfo* connInfo) noexcept = 0;
  virtual void release(RecievedMessagesIterator* iter) noexcept = 0;
};

#endif  // PLAGINFACADE_H
