#ifndef GMAILCONNECTIONWIDGET_H
#define GMAILCONNECTIONWIDGET_H

#include "export/pluginwidget.h"

namespace Ui {
class GmailConnectionWidget;
}
class GmailConnectionInfo;

class GmailConnectionWidget final : public PluginWidget {
  Q_OBJECT

 public:
  explicit GmailConnectionWidget(QWidget* parent = nullptr);
  ~GmailConnectionWidget() override;

 public:
  virtual void setConnectionInfo(
      PluginConnectionInfo* connInfo) noexcept override;
  virtual const char* fillConnectionInfo(
      PluginConnectionInfo* conninfo) noexcept override;

 public:
  virtual void cleareWidget() noexcept override;
  virtual void makeActivated() noexcept override;

 private:
  Ui::GmailConnectionWidget* ui;
};

#endif  // GMAILCONNECTIONWIDGET_H
