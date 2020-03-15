#ifndef VKCONNECTIONWIDGET_H
#define VKCONNECTIONWIDGET_H

#include "export/pluginwidget.h"
namespace Ui {
class VkConnectionWidget;
}

class VkConnectionWidget : public PluginWidget {
  Q_OBJECT

 public:
  explicit VkConnectionWidget(QWidget* parent = nullptr);
  ~VkConnectionWidget() override;

 public:
  virtual void setConnectionInfo(PluginConnectionInfo*) noexcept override;
  virtual const char* fillConnectionInfo(
      PluginConnectionInfo* conninfo) noexcept override;

 public:
  virtual void cleareWidget() noexcept override;
  virtual void makeActivated() noexcept override;

 private:
  Ui::VkConnectionWidget* ui;
};

#endif  // VKCONNECTIONWIDGET_H
