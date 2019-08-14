#ifndef BASESETTINGSDIALOG_H
#define BASESETTINGSDIALOG_H

#include <QDialog>
#include <memory>

class IdentifiedListModel;
class QSortFilterProxyModel;

class QModelIndex;
namespace Ui {
class BaseSettingsDialog;
}

/**
 * @brief Диалог для управления контактами/подключениями.
 * Содрежит QListView для отображения списка элементов,
 * а так же кнопки Сохранить/Закрыть и пр.
 *
 * Генерирует события, когда пользователь щелкает на QListView для отображения
 * элемента по выбранной позиции, так же события о сохранении выполненных
 * изменений и пр.
 *
 * Эти сигналы передаются виджету, который отображает элемент для обработки.
 *
 */
class BaseSettingsDialog : public QDialog {
  Q_OBJECT

 private:
  enum class State { ADD, CHANGE, VIEW };

 public:
  explicit BaseSettingsDialog(std::shared_ptr<IdentifiedListModel> model,
                              QWidget* dataWidget,
                              QWidget* parent = nullptr);
  ~BaseSettingsDialog() override;

 public:
  BaseSettingsDialog(const BaseSettingsDialog&) = delete;
  BaseSettingsDialog& operator=(const BaseSettingsDialog&) = delete;

 signals:

  /**
   * @brief Сигнал генерируется, когда пользователь выбирает элемент из списка
   * @param pos позиция элемента
   */
  void actionViewAt(std::string id);

  /**
   * @brief Команда на удаление элемента по указанной позиции
   * @param pos позиция элемента, по которой должен быть удален элемент
   */
  void actionRemoveAt(std::string id);

  /**
   * @brief Сигнал на обновление существующего элемента по указанной позиции
   * @param pos позиция элемента, который должен быть обновлен
   */
  void actionUpdateAt(std::string id);

  /**
   * @brief Команда на добавление нового элемента
   */
  void actionAdd();

  /**
   * @brief Команда на очистку полей виджета, который отображает элемент
   */
  void actionCleare();

  /**
   * @brief Команда информирует о том, что поля виджета, который отображает
   * элемент, должны быть доступны
   */
  void actionEnable();

  /**
   * @brief Команда информирует о том, что поля виджета, который отображает
   * элемент, должны быть не доступны
   */
  void actionDisable();

 private slots:
  void on_elementsView_doubleClicked(const QModelIndex& index);
  void on_elementsAdd_clicked();
  void on_changeAction_clicked();
  void on_deleteAction_clicked();
  void on_cancelAction_clicked();
  void on_saveAction_clicked();

  void on_elementsView_clicked(const QModelIndex& index);

  void on_closeButton_clicked();

 private:
  void moveToState(State newState);

 protected:
  Ui::BaseSettingsDialog* ui;

 private:
  std::optional<std::string> mSelectedId;
  State mCurrentState{State::ADD};
  QSortFilterProxyModel* mProxyModel;
  std::shared_ptr<IdentifiedListModel> mModel;
};

#endif  // BASESETTINGSDIALOG_H
