#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QWidget>

class PluginConnectionInfo;
class PluginWidget : public QWidget {
 public:
  using QWidget::QWidget;

 public:
  virtual const char* getErrorMessage() const noexcept = 0;
  virtual PluginConnectionInfo* makeConnectionInfo() noexcept = 0;
  virtual void setConnectionInfo(PluginConnectionInfo* connInfo) noexcept = 0;

 public:
  virtual void cleareWidget() noexcept = 0;
  virtual void makeActivated() noexcept = 0;
};

#endif  // PLUGINWIDGET_H
