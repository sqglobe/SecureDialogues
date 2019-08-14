#include "basesettingsdialog.h"
#include <QSortFilterProxyModel>
#include <cassert>
#include "models/identifiedlistmodel.h"
#include "ui_basesettingsdialog.h"

BaseSettingsDialog::BaseSettingsDialog(
    std::shared_ptr<IdentifiedListModel> model,
    QWidget* dataWidget,
    QWidget* parent) :
    QDialog(parent),
    ui(new Ui::BaseSettingsDialog),
    mProxyModel(new QSortFilterProxyModel(this)), mModel(std::move(model))

{
  ui->setupUi(this);
  mProxyModel->setSourceModel(model.get());
  ui->elementsView->setModel(mProxyModel);
  ui->vidgetLayout->addWidget(dataWidget);
  dataWidget->setParent(this);

  connect(this, &BaseSettingsDialog::actionDisable, ui->cancelAction,
          &QPushButton::hide);
  connect(this, &BaseSettingsDialog::actionDisable, ui->saveAction,
          &QPushButton::hide);
  connect(this, &BaseSettingsDialog::actionDisable, ui->changeAction,
          &QPushButton::show);
  connect(this, &BaseSettingsDialog::actionDisable, ui->deleteAction,
          &QPushButton::show);

  connect(this, &BaseSettingsDialog::actionEnable, ui->cancelAction,
          &QPushButton::show);
  connect(this, &BaseSettingsDialog::actionEnable, ui->saveAction,
          &QPushButton::show);
  connect(this, &BaseSettingsDialog::actionEnable, ui->changeAction,
          &QPushButton::hide);
  connect(this, &BaseSettingsDialog::actionEnable, ui->deleteAction,
          &QPushButton::hide);

  emit actionEnable();
}

BaseSettingsDialog::~BaseSettingsDialog() {
  delete ui;
}

void BaseSettingsDialog::on_elementsView_doubleClicked(
    const QModelIndex& index) {
  auto sourceIndx = mProxyModel->mapToSource(index);
  if (sourceIndx.isValid() && (mSelectedId = mModel->getId(sourceIndx))) {
    moveToState(State::VIEW);
    emit actionDisable();
    emit actionViewAt(mSelectedId.value());
  }
}

void BaseSettingsDialog::on_elementsAdd_clicked() {
  mSelectedId.reset();
  moveToState(State::ADD);
  emit actionCleare();
  emit actionEnable();
}

void BaseSettingsDialog::moveToState(BaseSettingsDialog::State newState) {
  mCurrentState = newState;
}

void BaseSettingsDialog::on_changeAction_clicked() {
  assert(mSelectedId);
  moveToState(State::CHANGE);
  emit actionEnable();
}

void BaseSettingsDialog::on_deleteAction_clicked() {
  assert(mSelectedId);
  moveToState(State::ADD);
  emit actionRemoveAt(mSelectedId.value());
  emit actionCleare();
  emit actionEnable();
}

void BaseSettingsDialog::on_cancelAction_clicked() {
  if (State::ADD == mCurrentState) {
    emit actionCleare();
  } else if (State::CHANGE == mCurrentState) {
    moveToState(State::VIEW);
    assert(mSelectedId);
    emit actionViewAt(mSelectedId.value());
    emit actionDisable();
  }
}

void BaseSettingsDialog::on_saveAction_clicked() {
  if (State::ADD == mCurrentState) {
    emit actionAdd();
    emit actionCleare();
  } else if (State::CHANGE == mCurrentState) {
    moveToState(State::VIEW);
    assert(mSelectedId);
    emit actionUpdateAt(mSelectedId.value());
    emit actionDisable();
  }
}

void BaseSettingsDialog::on_elementsView_clicked(const QModelIndex& index) {
  on_elementsView_doubleClicked(index);
}

void BaseSettingsDialog::on_closeButton_clicked() {
  this->hide();
  this->on_cancelAction_clicked();
}
