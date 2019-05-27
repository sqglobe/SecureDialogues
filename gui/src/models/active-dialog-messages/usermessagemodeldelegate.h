#ifndef USERMESSAGEMODELDELEGATE_H
#define USERMESSAGEMODELDELEGATE_H
#include <QStyledItemDelegate>

class UserMessageModelDelegate : public QStyledItemDelegate {
 public:
  UserMessageModelDelegate(QObject* parent = nullptr);

 public:
  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;

  QSize sizeHint(const QStyleOptionViewItem& option,
                 const QModelIndex& index) const override;
  QWidget* createEditor(QWidget* parent,
                        const QStyleOptionViewItem& option,
                        const QModelIndex& index) const override;
  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
};

#endif  // USERMESSAGEMODELDELEGATE_H
