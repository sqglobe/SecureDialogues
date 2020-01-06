#ifndef GMAILCONNECTIONWIDGET_H
#define GMAILCONNECTIONWIDGET_H

#include "export/pluginwidget.h"

namespace Ui {
class GmailConnectionWidget;
}
class GmailConnectionInfo;

class GmailConnectionWidget : public PluginWidget {
  Q_OBJECT

 public:
  explicit GmailConnectionWidget(QWidget* parent = nullptr);
  ~GmailConnectionWidget() override;

 public:
  virtual const char* getErrorMessage() const noexcept override;
  virtual PluginConnectionInfo* makeConnectionInfo() noexcept override;
  virtual void setConnectionInfo(
      PluginConnectionInfo* connInfo) noexcept override;

 public:
  virtual void cleareWidget() noexcept override;
  virtual void makeEnable() noexcept override;
  virtual void makeDisable() noexcept override;
  virtual void makeActivated() noexcept override;

 private:
  Ui::GmailConnectionWidget* ui;

 private:
  mutable GmailConnectionInfo* mConnInfo{nullptr};
};

#endif  // GMAILCONNECTIONWIDGET_H
