#ifndef VKCONNECTIONWIDGET_H
#define VKCONNECTIONWIDGET_H

#include "export/pluginwidget.h"
namespace Ui {
class VkConnectionWidget;
}

class VkConnectionWidget final : public PluginWidget {
  Q_OBJECT

 public:
  explicit VkConnectionWidget(QWidget* parent = nullptr);
  ~VkConnectionWidget() override;

 public:
  void setConnectionInfo(PluginConnectionInfo*) noexcept override;
  const char* fillConnectionInfo(
      PluginConnectionInfo* conninfo) noexcept override;

 public:
  void cleareWidget() noexcept override;
  void makeActivated() noexcept override;

 private:
  Ui::VkConnectionWidget* ui;
};

#endif  // VKCONNECTIONWIDGET_H
