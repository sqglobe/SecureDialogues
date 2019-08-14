#ifndef GUIINITIALIZER_H
#define GUIINITIALIZER_H

#include <memory>
#include "communication/channel.h"

class MainWindow;
class CoreInitializer;
class AbstractUserNotifier;
class AbstractUserAsk;
class BaseSettingsDialog;
class DialogCreation;
class DialogUserViewWrapper;
class QThread;

class GuiInitializer {
 public:
  GuiInitializer(MainWindow* parent,
                 const std::shared_ptr<CoreInitializer>& coreInit,
                 const std::shared_ptr<AbstractUserAsk>& userAsk,
                 const std::shared_ptr<AbstractUserNotifier>& userNotifier,
                 const std::shared_ptr<Channel::EventQueue>& eventQueue);

 private:
  void initMessageWrapper(
      const std::shared_ptr<CoreInitializer>& coreInit,
      MainWindow* parent,
      const std::shared_ptr<AbstractUserNotifier>& userNotifier,
      QThread* messageThread);
  void initDialogWrapper(
      const std::shared_ptr<CoreInitializer>& coreInit,
      MainWindow* parent,
      const std::shared_ptr<AbstractUserNotifier>& userNotifier,
      QThread* messageThread);

 private:
  std::shared_ptr<BaseSettingsDialog> mChannelSettingsDialog;
  std::shared_ptr<BaseSettingsDialog> mContactsSettingsDialog;
  std::shared_ptr<DialogCreation> mDialogCreation;
  std::unique_ptr<DialogUserViewWrapper> mDialogUserViewWrapper;
};

#endif  // GUIINITIALIZER_H
