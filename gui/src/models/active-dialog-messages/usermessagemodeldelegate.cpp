#include "usermessagemodeldelegate.h"
#include <QLabel>
#include <QPainter>
#include "containers/usermessage.h"

#include <memory>

Q_DECLARE_METATYPE(std::shared_ptr<const UserMessage>);

const auto TEXT_PADDING{10};
const auto SYSTEM_MESSAGE_PADDING{20};

QColor getColor(UserMessage::Status status) {
  switch (status) {
    case UserMessage::Status::Deliveried:
      return QColor(Qt::black);
    case UserMessage::Status::NotDeliveried:
      return QColor(Qt::red);
    case UserMessage::Status::WaitDelivery:
      return QColor(Qt::gray);
    case UserMessage::Status::Error:
      return QColor(Qt::darkRed);
  }
  return QColor();
}

QColor getBackground(UserMessage::Type type) {
  switch (type) {
    case UserMessage::Type::In:
      return QColor(135, 206, 235);
    case UserMessage::Type::Out:
      return QColor(152, 251, 152);
    case UserMessage::Type::System:
      return QColor(255, 182, 193);
  }
}

auto getOptions(const std::shared_ptr<const UserMessage>& message) {
  switch (message->type()) {
    case UserMessage::Type::In:
      return Qt::AlignLeft | Qt::AlignVCenter;
    case UserMessage::Type::Out:
      return Qt::AlignRight | Qt::AlignVCenter;
    case UserMessage::Type::System:
      return Qt::AlignCenter | Qt::AlignVCenter;
  }
}

auto getTextRect(const std::shared_ptr<const UserMessage>& message,
                 const QRect& paintRect) {
  auto newRect = paintRect;

  newRect.setTop(newRect.top() - TEXT_PADDING);
  newRect.setBottom(newRect.bottom() + TEXT_PADDING);
  if (message->type() == UserMessage::Type::System) {
    newRect.setLeft(newRect.left() - SYSTEM_MESSAGE_PADDING);
    newRect.setRight(newRect.right() + SYSTEM_MESSAGE_PADDING);
  } else if (message->type() == UserMessage::Type::In) {
    newRect.setRight(newRect.right() + 2 * TEXT_PADDING);
  } else if (message->type() == UserMessage::Type::Out) {
    newRect.setLeft(newRect.left() - 2 * TEXT_PADDING);
  }
  return newRect;
}

UserMessageModelDelegate::UserMessageModelDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {}

void UserMessageModelDelegate::paint(QPainter* painter,
                                     const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const {
  if (!index.data().canConvert<std::shared_ptr<const UserMessage>>()) {
    QStyledItemDelegate::paint(painter, option, index);
    return;
  }

  const auto& message =
      qvariant_cast<std::shared_ptr<const UserMessage>>(index.data());

  auto textFlags = static_cast<int>(getOptions(message));
  auto messageText = QString(message->content().c_str());
  auto newRect = getTextRect(
      message, painter->boundingRect(option.rect, textFlags, messageText));

  painter->fillRect(newRect, getBackground(message->type()));

  if (option.state & QStyle::State_Selected) {
    painter->fillRect(newRect, option.palette.highlight());
  }

  auto textPaintRect = newRect;
  textPaintRect.setLeft(textPaintRect.left() + TEXT_PADDING);
  textPaintRect.setRight(textPaintRect.right() - TEXT_PADDING);
  painter->setPen(getColor(message->status()));
  painter->drawText(textPaintRect, textFlags, messageText);
}

QSize UserMessageModelDelegate::sizeHint(const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const {
  if (!index.data().canConvert<std::shared_ptr<const UserMessage>>()) {
    return QStyledItemDelegate::sizeHint(option, index);
  }

  const auto& message =
      qvariant_cast<std::shared_ptr<const UserMessage>>(index.data());
  const auto messageText = QString(message->content().c_str());

  auto mainTextWidth = option.fontMetrics.width(messageText);

  if (message->type() == UserMessage::Type::System) {
    mainTextWidth += 2 * SYSTEM_MESSAGE_PADDING;
  }

  auto maintTextHeight =
      option.fontMetrics.height() * (messageText.count('\n') + 1);
  return QSize(mainTextWidth + TEXT_PADDING * 2,
               maintTextHeight + 2 * TEXT_PADDING);
}

QWidget* UserMessageModelDelegate::createEditor(
    QWidget* parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const {
  if (index.data().canConvert<std::shared_ptr<const UserMessage>>()) {
    auto editor = new QLabel(parent);
    editor->setFocusPolicy(Qt::StrongFocus);
    editor->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                    Qt::TextSelectableByKeyboard);
    auto pallet = editor->palette();
    pallet.setBrush(editor->backgroundRole(), option.palette.highlight());
    editor->setPalette(pallet);
    editor->setAutoFillBackground(true);
    editor->setIndent(TEXT_PADDING);
    return editor;
  }
  return QStyledItemDelegate::createEditor(parent, option, index);
}

void UserMessageModelDelegate::setEditorData(QWidget* editor,
                                             const QModelIndex& index) const {
  if (index.data().canConvert<std::shared_ptr<const UserMessage>>()) {
    auto message =
        qvariant_cast<std::shared_ptr<const UserMessage>>(index.data());
    auto label = qobject_cast<QLabel*>(editor);
    label->setText(message->content().c_str());
    label->setAlignment(getOptions(message));

  } else {
    return QStyledItemDelegate::setEditorData(editor, index);
  }
}
