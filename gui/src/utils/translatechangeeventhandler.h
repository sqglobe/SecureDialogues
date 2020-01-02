#ifndef TRANSLATECHANGEEVENTHANDLER_H
#define TRANSLATECHANGEEVENTHANDLER_H

#include <QEvent>
#include <QWidget>
#include <cassert>

template <typename W, typename UI>
class TranslateChangeEventHandler : public W {
 public:
  TranslateChangeEventHandler(QWidget* parent = nullptr) : W(parent) {}

 public:
  void setUI(UI* ui) {
    assert(ui != nullptr);
    mUI = ui;
  }

 protected:
  void changeEvent(QEvent* event) override {
    if (event->type() == QEvent::LanguageChange) {
      mUI->retranslateUi(this);  // переведём окно заново
    } else {
      W::changeEvent(event);
    }
  }

 private:
  UI* mUI;
};
#endif  // TRANSLATECHANGEEVENTHANDLER_H
