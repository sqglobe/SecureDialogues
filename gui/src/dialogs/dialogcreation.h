#ifndef DIALOGCREATION_H
#define DIALOGCREATION_H

#include <QDialog>
#include <memory>
#include "containers/storages.h"
#include "primitives/contact.h"
#include "ui_dialogcreation.h"
#include "utils/translatechangeeventhandler.h"

class ContactModel;
class QSortFilterProxyModel;

/**
 * @brief Отображает пользователю список контактов, чтобы он мог выбрать один из
 * них для создания диалога
 */
class DialogCreation
    : public TranslateChangeEventHandler<QDialog, Ui::DialogCreation> {
  Q_OBJECT

 public:
  explicit DialogCreation(std::shared_ptr<ContactModel> model,
                          std::shared_ptr<ContactStorage> storage,
                          QWidget* parent = nullptr);
  ~DialogCreation() override;

 public:
  DialogCreation(const DialogCreation&) = delete;
  DialogCreation& operator=(const DialogCreation&) = delete;

 private slots:
  void on_buttonBox_accepted();
  void on_buttonBox_rejected();

 signals:
  void createNewDialog(Contact contact);

 private:
  Ui::DialogCreation* ui;
  std::shared_ptr<ContactModel> mModel;
  QSortFilterProxyModel* mProxy;
  std::shared_ptr<ContactStorage> mStorage;
};

#endif  // DIALOGCREATION_H
