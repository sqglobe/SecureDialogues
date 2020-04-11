#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QWidget>
#include "export-definition.h"

class PluginConnectionInfo;

class EXPORTED PluginWidget : public QWidget {
 public:
  using QWidget::QWidget;

 public:
  [[nodiscard]] virtual const char* fillConnectionInfo(
      PluginConnectionInfo* conninfo) noexcept = 0;
  virtual void setConnectionInfo(PluginConnectionInfo* connInfo) noexcept = 0;

 public:
  virtual void cleareWidget() noexcept = 0;
  virtual void makeActivated() noexcept = 0;
};

#endif  // PLUGINWIDGET_H
