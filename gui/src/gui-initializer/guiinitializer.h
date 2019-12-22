#ifndef GUIINITIALIZER_H
#define GUIINITIALIZER_H

#include <memory>
#include "communication/channel.h"

class MainWindow;
class AbstractCoreInitializer;
class AbstractUserNotifier;
class AbstractUserAsk;
class BaseSettingsDialog;
class DialogCreation;
class DialogUserViewWrapper;
class QThread;
class DiscoveredContactModel;
class RecievedContactsStorageWrapper;
class ApplicationSettingsGuard;
class TranslationMaster;

class GuiInitializer {
 public:
  GuiInitializer(MainWindow* parent,
                 const std::shared_ptr<AbstractCoreInitializer>& coreInit,
                 const std::shared_ptr<AbstractUserAsk>& userAsk,
                 const std::shared_ptr<AbstractUserNotifier>& userNotifier,
                 const std::shared_ptr<Channel::EventQueue>& eventQueue,
                 std::shared_ptr<ApplicationSettingsGuard> settingsGuard,
                 std::shared_ptr<TranslationMaster> translatorMaster);

 private:
  void initMessageWrapper(
      const std::shared_ptr<AbstractCoreInitializer>& coreInit,
      MainWindow* parent,
      const std::shared_ptr<AbstractUserNotifier>& userNotifier,
      QThread* messageThread);
  void initDialogWrapper(
      const std::shared_ptr<AbstractCoreInitializer>& coreInit,
      MainWindow* parent,
      const std::shared_ptr<AbstractUserNotifier>& userNotifier,
      QThread* messageThread);
  void initSimpleDialogs(
      MainWindow* parent,
      const std::shared_ptr<AbstractCoreInitializer>& coreInit,
      const std::shared_ptr<AbstractUserAsk>& userAsk,
      const std::shared_ptr<AbstractUserNotifier>& userNotifier,
      const std::shared_ptr<Channel::EventQueue>& eventQueue);

 private:
  std::shared_ptr<BaseSettingsDialog> mChannelSettingsDialog;
  std::shared_ptr<BaseSettingsDialog> mContactsSettingsDialog;
  std::shared_ptr<DialogCreation> mDialogCreation;
  std::unique_ptr<DialogUserViewWrapper> mDialogUserViewWrapper;
  std::shared_ptr<DiscoveredContactModel> mDiscoveredContactModel;
  std::shared_ptr<RecievedContactsStorageWrapper>
      mRecievedContactsStorageWrapper;
  std::shared_ptr<ApplicationSettingsGuard> mApplicationSettingsGuard;
  std::shared_ptr<TranslationMaster> mTranslatorMaster;
};

#endif  // GUIINITIALIZER_H
