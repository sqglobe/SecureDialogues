#ifndef DIALOGACTIONHANDLERWRAPPER_H
#define DIALOGACTIONHANDLERWRAPPER_H

#include <QObject>
#include <memory>
class Contact;
class DialogActionHandler;
class AbstractUserNotifier;

/**
 * @brief Класс-оболочка для взаимодействия с DialogActionHandler
 * Нужен для применения системы сигналов-слотов.
 */
class DialogActionHandlerWrapper : public QObject {
  Q_OBJECT
 public:
  explicit DialogActionHandlerWrapper(
      std::shared_ptr<DialogActionHandler> handler,
      std::shared_ptr<AbstractUserNotifier> notifier,
      QObject* parent = nullptr);

 signals:

 public slots:
  void createDialogAction(std::shared_ptr<const Contact> contact);
  void closeDialogAction(std::string dialogId);
  void abortDialogAction(std::string dialogId);
  void removeDialogAction(std::string dialogId);

 private:
  std::shared_ptr<DialogActionHandler> mHandler;
  std::shared_ptr<AbstractUserNotifier> mNotifier;
};

#endif  // DIALOGACTIONHANDLERWRAPPER_H
