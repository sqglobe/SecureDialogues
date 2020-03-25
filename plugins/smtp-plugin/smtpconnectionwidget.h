#ifndef SMTPCONNECTIONWIDGET_H
#define SMTPCONNECTIONWIDGET_H

#include "export/pluginwidget.h"

namespace Ui {
class SmtpConnectionWidget;
}

class SmtpConnectionWidget : public PluginWidget {
  Q_OBJECT

 public:
  explicit SmtpConnectionWidget(QWidget* parent = nullptr);
  ~SmtpConnectionWidget() override;

 public:
  void setConnectionInfo(PluginConnectionInfo* connInfo) noexcept override;
  const char* fillConnectionInfo(
      PluginConnectionInfo* conninfo) noexcept override;

 public:
  void cleareWidget() noexcept override;
  void makeActivated() noexcept override;

 private:
  Ui::SmtpConnectionWidget* ui;
};

#endif  // SMTPCONNECTIONWIDGET_H
