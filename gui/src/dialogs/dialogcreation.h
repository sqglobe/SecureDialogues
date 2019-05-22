#ifndef DIALOGCREATION_H
#define DIALOGCREATION_H

#include <QDialog>
#include <memory>

namespace Ui {
class DialogCreation;
}

class ContactModel;
class QSortFilterProxyModel;
class Contact;

/**
 * @brief Отображает пользователю список контактов, чтобы он мог выбрать один из
 * них для создания диалога
 */
class DialogCreation : public QDialog {
  Q_OBJECT

 public:
  explicit DialogCreation(const std::shared_ptr<ContactModel>& model,
                          QWidget* parent = 0);
  ~DialogCreation();

 private slots:
  void on_buttonBox_accepted();
  void on_buttonBox_rejected();

 signals:
  void createNewDialog(std::shared_ptr<const Contact> contact);

 private:
  Ui::DialogCreation* ui;
  std::shared_ptr<ContactModel> mModel;
  QSortFilterProxyModel* mProxy;
};

#endif  // DIALOGCREATION_H
