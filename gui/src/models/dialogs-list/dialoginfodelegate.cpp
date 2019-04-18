#include "dialoginfodelegate.h"
#include <QPainter>
#include "dialoginfo.h"

Q_DECLARE_METATYPE(DialogInfo);

QColor getColor(Dialog::Status status) {
  if (Dialog::Status::ACTIVE == status) {
    return QColor(Qt::green);
  } else if (Dialog::Status::CANCELED == status ||
             Dialog::Status::CLOSED == status) {
    return QColor(Qt::red);
  }
  return QColor(Qt::gray);
}

DialogInfoDelegate::DialogInfoDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {}

void DialogInfoDelegate::paint(QPainter* painter,
                               const QStyleOptionViewItem& option,
                               const QModelIndex& index) const {
  if (!index.data().canConvert<DialogInfo>()) {
    QStyledItemDelegate::paint(painter, option, index);
    return;
  }

  auto info = qvariant_cast<DialogInfo>(index.data());

  painter->fillRect(option.rect, getColor(info.status()));

  if (option.state & QStyle::State_Selected) {
    painter->fillRect(option.rect, option.palette.highlight());
  }

  auto boldFont = option.font;
  boldFont.setBold(true);

  QFontMetrics fm(boldFont);

  auto boldWidth = fm.width(QString::number(info.unreadMessages()));

  QRect mainText = option.rect;

  if (info.unreadMessages() > 0) {
    mainText.setWidth(mainText.width() - boldWidth - 2 * widthAligment);
    QRect unreadRect = option.rect;

    unreadRect.setLeft(unreadRect.left() + mainText.width());

    painter->fillRect(unreadRect, QColor(65, 105, 225));
    painter->save();
    painter->setPen(Qt::white);
    painter->setFont(boldFont);
    painter->drawText(unreadRect, Qt::AlignCenter,
                      QString::number(info.unreadMessages()));
    painter->restore();
  }

  painter->drawText(
      mainText, Qt::AlignCenter,
      QString("%1(%2)").arg(info.name().c_str(), info.address().c_str()));
}

QSize DialogInfoDelegate::sizeHint(const QStyleOptionViewItem& option,
                                   const QModelIndex& index) const {
  if (!index.data().canConvert<DialogInfo>()) {
    return QStyledItemDelegate::sizeHint(option, index);
  }

  auto info = qvariant_cast<DialogInfo>(index.data());

  if (info.unreadMessages() == 0) {
    auto mainTextWidth = option.fontMetrics.width(
        QString("%1(%2)").arg(info.name().c_str(), info.address().c_str()));
    return QSize(mainTextWidth + widthAligment * 2,
                 option.fontMetrics.height() + 2 * heightAligment);
  }

  auto boldFont = option.font;
  boldFont.setBold(true);

  QFontMetrics fm(boldFont);

  auto unreadWidth = fm.width(QString::number(info.unreadMessages()));
  auto mainTextWidth = option.fontMetrics.width(
      QString("%1(%2)").arg(info.name().c_str(), info.address().c_str()));

  return QSize(mainTextWidth + unreadWidth + widthAligment * 4,
               fm.height() + 2 * heightAligment);
}
