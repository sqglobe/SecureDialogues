#ifndef CONTACTDISCOVERDIALOG_H
#define CONTACTDISCOVERDIALOG_H

#include <QDialog>
#include <memory>
#include "ui_contactdiscoverdialog.h"
#include "utils/translatechangeeventhandler.h"

class DiscoverContactAgent;
class ChannelsListModel;

class ContactDiscoverDialog
    : public TranslateChangeEventHandler<QDialog, Ui::ContactDiscoverDialog> {
  Q_OBJECT

 public:
  explicit ContactDiscoverDialog(std::shared_ptr<ChannelsListModel> model,
                                 std::shared_ptr<DiscoverContactAgent> agent,
                                 QWidget* parent = nullptr);
  ~ContactDiscoverDialog() override;

 protected:
  void showEvent(QShowEvent* event) override;
  void changeEvent(QEvent* event) override;

 private slots:
  void on_sendContactBtn_clicked();

 private:
  Ui::ContactDiscoverDialog* ui;

 private:
  std::shared_ptr<ChannelsListModel> mModel;
  std::shared_ptr<DiscoverContactAgent> mAgent;
};

#endif  // CONTACTDISCOVERDIALOG_H
