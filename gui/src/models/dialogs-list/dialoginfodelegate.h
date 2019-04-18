#ifndef DIALOGINFODELEGATE_H
#define DIALOGINFODELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class DialogInfoDelegate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  explicit DialogInfoDelegate(QObject* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;

  QSize sizeHint(const QStyleOptionViewItem& option,
                 const QModelIndex& index) const override;

 signals:

 public slots:

 private:
  static const int heightAligment = 3;
  static const int widthAligment = 5;
};

#endif  // DIALOGINFODELEGATE_H
