#ifndef MESSAGEHANDLERWRAPPER_H
#define MESSAGEHANDLERWRAPPER_H

#include <QObject>
#include <memory>

class MessageActionHandler;

/**
 * @brief Прокси-класс для MessageActionHandler
 * Используется, чтобы можно было использовать сигналы-слоты
 */
class MessageHandlerWrapper : public QObject {
  Q_OBJECT
 public:
  explicit MessageHandlerWrapper(std::shared_ptr<MessageActionHandler> handler,
                                 QObject* parent = nullptr);

 signals:

 public slots:
  void setActiveDialog(std::string idDialog);
  void cleareActiveDialog(std::string);
  void sendMessage(std::string messageText);

 private:
  std::shared_ptr<MessageActionHandler> mMessageHandler;
};

#endif  // MESSAGEHANDLERWRAPPER_H
