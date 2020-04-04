#ifndef PLAGINFACADE_H
#define PLAGINFACADE_H
#include "languages.h"

#include <QtGlobal>

class PluginWidget;
class PluginConnectionInfo;
class PluginConnectionSerializer;
class PluginMessageCommunicator;
class RecievedMessagesIterator;
class PluginAddressValidator;

class Q_DECL_EXPORT PluginFacade {
 public:
  virtual ~PluginFacade() = default;

 public:
  [[nodiscard]] virtual const char* getId() const noexcept = 0;
  [[nodiscard]] virtual const char* getPluginName() const noexcept = 0;
  [[nodiscard]] virtual const char* getTranslationFileName(Language lang) const
      noexcept = 0;
  [[nodiscard]] virtual const char* getGettextDomain() const noexcept = 0;

 public:
  [[nodiscard]] virtual PluginWidget* getWidget() const noexcept = 0;
  [[nodiscard]] virtual const PluginConnectionSerializer* getSerializer() const
      noexcept = 0;
  [[nodiscard]] virtual PluginMessageCommunicator*
  makeCommunicator() noexcept = 0;
  [[nodiscard]] virtual PluginConnectionInfo* makeEmptyConn() noexcept = 0;
  [[nodiscard]] virtual const PluginAddressValidator* getAddressValidator()
      const noexcept = 0;

 public:
  virtual void release(PluginMessageCommunicator* comm) noexcept = 0;
  virtual void release(PluginConnectionInfo* connInfo) noexcept = 0;
  virtual void release(RecievedMessagesIterator* iter) noexcept = 0;
};

#endif  // PLAGINFACADE_H
