#ifndef DUMMYCONNECTIONWIDGET_H
#define DUMMYCONNECTIONWIDGET_H

#include "export/pluginwidget.h"

class DummyConnectionWidget final : public PluginWidget {
 public:
  void cleareWidget() noexcept override;
  void makeActivated() noexcept override;
  void setConnectionInfo(PluginConnectionInfo* connInfo) noexcept override;
  const char* fillConnectionInfo(
      PluginConnectionInfo* conninfo) noexcept override;
};

#endif  // DUMMYCONNECTIONWIDGET_H
