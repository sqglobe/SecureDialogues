#ifndef DIALOGWIDGETGASKET_H
#define DIALOGWIDGETGASKET_H

#include <memory>
#include "interfaces/abstractuserask.h"
#include "interfaces/abstractusernotifier.h"

/**
 * Прокладка между контейнером и виджетом, который отображает конкретный элемент
 * в диалоге управления.
 */
template <typename Container, typename Widget>
class DialogWidgetGasket {
 public:
  DialogWidgetGasket(const std::shared_ptr<Container>& container,
                     Widget* widget,
                     const std::shared_ptr<AbstractUserAsk>& userAsk,
                     const std::shared_ptr<AbstractUserNotifier>& notifier);

 public:
  /**
   * @brief Получает элемент из контейнера по указанной позиции и передает его в
   * виджет для отображения
   * @param pos позиция элемента в контейнере
   */
  void viewAt(int pos);

  /**
   * @brief Получает из виджета элемент и обновляет его в контейнере
   */
  void update();

  /**
   * @brief Удаляет элемент из контейнера по указанной позиции
   * @param pos позиция элемента в контейнере для удаления
   */
  void removeAt(int pos);

  /**
   * @brief Получает из виджета элемент и сохраняет его в контейнере как новый
   */
  void add();

 private:
  std::shared_ptr<Container> mContainer;
  Widget* mWidget;
  std::shared_ptr<AbstractUserAsk> mUserAsk;
  std::shared_ptr<AbstractUserNotifier> mUserNotifier;
};

template <typename Container, typename Widget>
DialogWidgetGasket<Container, Widget>::DialogWidgetGasket(
    const std::shared_ptr<Container>& container,
    Widget* widget,
    const std::shared_ptr<AbstractUserAsk>& userAsk,
    const std::shared_ptr<AbstractUserNotifier>& notifier) :
    mContainer(container),
    mWidget(widget), mUserAsk(userAsk), mUserNotifier(notifier) {}

template <typename Container, typename Widget>
void DialogWidgetGasket<Container, Widget>::viewAt(int pos) {
  try {
    auto element = mContainer->at(pos);
    mWidget->setElement(element);
  } catch (std::exception& ex) {
    mUserNotifier->notify(AbstractUserNotifier::Severity::ERROR, ex.what());
  }
}

template <typename Container, typename Widget>
void DialogWidgetGasket<Container, Widget>::update() {
  try {
    auto element = mWidget->getElement();
    mContainer->update(element);
  } catch (const std::exception& ex) {
    mUserNotifier->notify(AbstractUserNotifier::Severity::ERROR, ex.what());
  }
}

template <typename Container, typename Widget>
void DialogWidgetGasket<Container, Widget>::removeAt(int pos) {
  if (mUserAsk->ask("Вы действительно хотите удалить текущий элемент?")) {
    mContainer->remove(pos);
  }
}

template <typename Container, typename Widget>
void DialogWidgetGasket<Container, Widget>::add() {
  try {
    auto element = mWidget->getElement();
    mContainer->add(element);
  } catch (const std::exception& ex) {
    std::string err = ex.what();
    mUserNotifier->notify(AbstractUserNotifier::Severity::ERROR, ex.what());
  }
}

#endif  // DIALOGWIDGETGASKET_H
