#ifndef MESSAGECONTAINER_H
#define MESSAGECONTAINER_H

#include <map>
#include <memory>
#include <mutex>
#include <vector>
class MessageContainerEventHandler;
class UserMessage;

/**
 * @brief Контейнер сообщений
 *
 * Выполняет роль хранилища полученных и отправленных сообщений.
 * Так же является генератором событий о том, что пришло новое сообщение,
 * изменился активный диалог и пр.
 *
 */

class MessageContainer : public std::enable_shared_from_this<MessageContainer> {
 public:
  /**
   * @brief Класс-оболочка, которая упрощает изменение сообщений
   */
  class Wrapper {
    friend class MessageContainer;

   public:
    /**
     * @brief Конструктор класса
     * @param container ссылка на контейнер, в котором будут сохранены изменения
     * @param message указатель на сообщение, которое содержит прокси-класс
     * @param dialogid - идентификатор диалога, к которому принадлежит сообщение
     */
    Wrapper(std::shared_ptr<MessageContainer> container,
            std::shared_ptr<UserMessage> message,
            std::string dialogid);

   public:
    /**
     * @brief Сохраняет изменения, внесенные в оборачиваемый объект
     * @return true - при успехе, false - в противном случае.
     */
    bool save();

   public:
    /**
     * @brief Возвращает ссылку на сообщение для вызова методом оборачиваемого
     * класса.
     * @return ссылка на сообщение
     */
    std::shared_ptr<UserMessage> get();

   private:
    std::shared_ptr<UserMessage> mMessage;
    std::weak_ptr<MessageContainer> mContainer;
    std::string mDialogId;
  };

 public:
  /**
   * @brief Задает новый активный диалог.
   * Этот метод инициализирует событие о том, что изменился активный диалог,
   * а так же передает зарегистрированным MessageConteinerEventHandler все
   * сообщения из активного диалога
   * @param dialogId новый активный диалог
   */
  void setActiveDialog(const std::string& dialogId);

  /**
   * @brief Добавляет новое сообщение
   * @param dialogId id диалога, к которому принадлежит сообщение
   * @param message - текст сообщения
   * @param isIncome - показывает получено ли сообщение по сети (true - получено
   * по сети)
   */
  void addMessage(const std::string& dialogId,
                  const std::string& message,
                  bool isIncome);

  /**
   * @brief Добавляет новое сообщение
   * @param dialogId id диалога, к которому принадлежит сообщение
   * @param messsage - сообщение
   */
  void addMessage(const std::string& dialogId,
                  const std::shared_ptr<UserMessage>& messsage);

  /**
   * @brief Добавляет указанное сообщение к текущему активному диалогу и
   * возвращает враппер для добавленного сообщения
   * @param message - текст сообщения
   * @param isIncome - показывает получено ли сообщение по сети (true - получено
   * по сети)
   * @return враппер для добавленного сообщения
   */
  std::unique_ptr<Wrapper> addToActiveDialogWithWrapper(
      const std::string& message,
      bool isIncome);

  std::unique_ptr<Wrapper> addToActiveDialogWithWrapper(std::string&& message,
                                                        bool isIncome);

  /**
   * @brief Добавляет сообщение к активному диалогу
   * @param message - текст сообщения
   * @param isIncome - показывает получено ли сообщение по сети (true - получено
   * по сети)
   */
  void addMessageToActivedialog(const std::string& message, bool isIncome);

  /**
   * @brief Добавляет новый обработчик событий.
   * @param handler новый обработчик
   */
  void registerHandler(
      const std::shared_ptr<MessageContainerEventHandler>& handler);

  /**
   * @brief Удаляет все обработчики
   */
  void cleareHandlers();

  /**
   * @brief Выполняет действия, когда одно из сообщения изменилось
   * @param dialogId идентификатор диалога, в котором произошло изменение
   * диалога
   */
  void messageChanged(const std::string& dialogId,
                      const std::shared_ptr<UserMessage>&);

 public:
  /**
   * @brief Метод для получения текущего активного диалога
   * @return id диалога, который ранее был задан как активный
   */
  std::string getActiveDialog() const;

  /**
   * @brief Выдает каждое сообщение из диалога dialogId обработчику handler
   * @param dialogId id диалога, сообщения которого нужно передать handler
   * @param handler класс, который будет получать активные сообщения
   */
  void list(const std::string& dialogId,
            const std::shared_ptr<MessageContainerEventHandler>& handler) const;

  void removeDialog(const std::string& dialogId);

 private:
  using DialogMessages = std::vector<std::shared_ptr<UserMessage>>;

 private:
  void notifymAboutDialogIdChanged(const std::string& newDialogId) const;
  void notifyAboutNewMessage(
      const std::string& dialogId,
      const std::shared_ptr<const UserMessage>& message) const;
  void notifyPeekAllMessagesFromActive() const;

 private:
  std::map<std::string, DialogMessages> mMessages;
  std::string mActiveDialog;
  std::vector<std::shared_ptr<MessageContainerEventHandler>> mEventHandlers;
  mutable std::recursive_mutex mMutex;
};

#endif  // MESSAGECONTAINER_H
