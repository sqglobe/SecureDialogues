#ifndef CONTACTDISCOVERDIALOG_H
#define CONTACTDISCOVERDIALOG_H

#include <QDialog>
#include <memory>

class DiscoverContactAgent;
class ChannelsListModel;
class AbstractUserNotifier;

namespace Ui {
class ContactDiscoverDialog;
}

class ContactDiscoverDialog : public QDialog {
  Q_OBJECT

 public:
  explicit ContactDiscoverDialog(std::shared_ptr<ChannelsListModel> model,
                                 std::shared_ptr<DiscoverContactAgent> agent,
                                 std::shared_ptr<AbstractUserNotifier> notifier,
                                 QWidget* parent = nullptr);
  ~ContactDiscoverDialog() override;

 protected:
  void showEvent(QShowEvent* event) override;

 private slots:
  void on_sendContactBtn_clicked();

 private:
  Ui::ContactDiscoverDialog* ui;

 private:
  std::shared_ptr<ChannelsListModel> mModel;
  std::shared_ptr<DiscoverContactAgent> mAgent;
  std::shared_ptr<AbstractUserNotifier> mNotifier;
};

#endif  // CONTACTDISCOVERDIALOG_H
