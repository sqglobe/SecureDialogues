#ifndef FAKECONNECTIONINFOWIDGET_H
#define FAKECONNECTIONINFOWIDGET_H
#include "widgets/connectioninfowidget.h"

class FakeConnectionInfoWidget : public ConnectionInfoWidget {
  Ui::ConnectionInfoWidget* getUi();
};

Ui::ConnectionInfoWidget* FakeConnectionInfoWidget::getUi() {
  return ui;
}

#endif  // FAKECONNECTIONINFOWIDGET_H
