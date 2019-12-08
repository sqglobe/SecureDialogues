#ifndef DESPATCHERRORSSINK_H
#define DESPATCHERRORSSINK_H

#include "containers/storages.h"
#include "interfaces/abstractdespatcherrorssink.h"

#include <memory>

class DespatchErrorsSink : public AbstractDespatchErrorsSink {
 public:
  DespatchErrorsSink(
      std::shared_ptr<DiscoveredContactStorage> discoveredStorage);

 public:
  virtual void error(
      DespatchError error,
      const DialogMessage& message,
      const std::string& channel_moniker) noexcept(false) override;

 private:
  std::shared_ptr<DiscoveredContactStorage> m_DiscoveredStorage;
};

#endif  // DESPATCHERRORSSINK_H
