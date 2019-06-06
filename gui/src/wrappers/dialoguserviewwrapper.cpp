#include "dialoguserviewwrapper.h"

#include <QListView>
#include "models/dialogs-list/dialogsortmodel.h"
#include "models/dialogs-list/dialogusermodel.h"

DialogUserViewWrapper::DialogUserViewWrapper(
    QListView* view,
    std::shared_ptr<DialogUserModel> model,
    QObject* parent) :
    QObject(parent),
    mView(view), mProxy(new DialogSortModel), mModel(std::move(model)) {
  mProxy->setDynamicSortFilter(true);
  mProxy->setSourceModel(mModel.get());
  mView->setModel(mProxy.get());
  mProxy->sort(0, Qt::DescendingOrder);
  connect(mView, &QListView::activated, this,
          &DialogUserViewWrapper::requestToActivateItem);
  connect(mView, &QListView::clicked, this,
          &DialogUserViewWrapper::requestToActivateItem);
}

void DialogUserViewWrapper::requestToShowMenu(QPoint pos) {
  auto index = mView->indexAt(pos);
  auto sIndex = mProxy->mapToSource(index);
  if (sIndex.isValid()) {
    auto [status, dialogId] = mModel->getDialogInfo(sIndex);
    emit showMenuForDialog(mView->viewport()->mapToGlobal(pos), status,
                           dialogId);
  }
}

void DialogUserViewWrapper::requestToActivateItem(const QModelIndex& index) {
  auto sIndex = mProxy->mapToSource(index);
  mModel->rowWasSelected(sIndex);
}
