#include <spdlog/sinks/stdout_color_sinks.h>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include "mainwindow.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

#include <db_cxx.h>
#include <dbstl_common.h>

#include "stand-helpers/queuedactionschanneladapter.h"
#include "stand/onechannelcoreinitializer.h"
#include "utils/messagemarshaller.h"

#include "translation/translationmaster.h"
#include "utils/applicationsettings.h"

#include <chrono>

void send_100_messages(OneChannelCoreInitializer* init) {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  MessageMarshaller marshaller;
  auto [adapter, sec] =
      init->make_dialog("address 1", "test dialog 1", "Test 1");
  if (adapter == nullptr) {
    std::cerr << "Failed to crate dialog" << std::endl;
    return;
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));

  for (auto i = 0; i < 1000; i++, sec++) {
    adapter->sendMessageToApplication(
        {"address 1",
         "SECURE DIALOG MESSAGE\n"
         "VERSION:1.0\n"
         "ACTION:MESSAGE\n"
         "SEQUENTAL_NUMBER:" +
             std::to_string(sec) +
             "\n"
             "DIALOG:test dialog 1\n"
             "TIMESTAMP: " +
             std::to_string(get_timestamp()) +
             "\n"
             "SIGNATURE: 1\n"
             "--------------------------------------------------------------\n"
             "test message " +
             std::to_string(i) + "\n"});
    auto messageFromApp = adapter->recieveMessageFromApplication(2000);
    if (!messageFromApp.has_value()) {
      std::cerr << "Message on step " << i << " have no containe " << std::endl;
      return;
    }
    if (auto parsedMessageFromApp = marshaller.unmarshall(
            messageFromApp->message, messageFromApp->address);
        !parsedMessageFromApp.has_value()) {
      std::cerr << "Failed parse message on step " << i << std::endl;
      return;
    } else if (parsedMessageFromApp->action() != DialogMessage::Action::ACK) {
      std::cout << "Get message that differ from ACK: "
                << static_cast<int>(parsedMessageFromApp->action())
                << ", step: " << i << std::endl;
      return;
    }
  }
}

int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::debug);  // Set global log level to info
  spdlog::rotating_logger_mt("root_logger", "regression_with_one_dialog.log",
                             1048576 * 5, 3);
  dbstl::dbstl_startup();

  QApplication a(argc, argv);

  QDir curr;

  if (curr.cd("regression_with_one_dialog_env")) {
    curr.removeRecursively();
  }
  if (!QDir().mkpath("regression_with_one_dialog_env")) {
    QMessageBox::critical(
        nullptr, "Abort!",
        QString("Выполнение приложение завершено из-за невозможности созать "
                "папку для конфигурации"));
    return 1;
  }

  try {
    auto ptr = std::make_unique<OneChannelCoreInitializer>(
        "regression_with_one_dialog_env");
    auto coreInit = ptr.get();
    auto pluginContainer = std::make_shared<plugin_support::PluginsContainer>();
    std::shared_ptr<ApplicationSettingsGuard> settingsGuard =
        std::make_shared<ApplicationSettingsGuard>();
    std::shared_ptr<TranslationMaster> translatorMaster =
        std::make_shared<TranslationMaster>(settingsGuard->getSettings(),
                                            "locale", pluginContainer);
    MainWindow w(std::move(settingsGuard), std::move(translatorMaster),
                 std::move(ptr), pluginContainer);
    w.show();
    std::thread th(send_100_messages, coreInit);

    auto res = a.exec();

    th.join();
    dbstl::dbstl_exit();

    return res;
  } catch (std::exception& ex) {
    QMessageBox::critical(
        nullptr, "Abort!",
        QString("Выполнение приложение завершено с сообщением ")
            .append(ex.what()));
    dbstl::dbstl_exit();
  }
  return 123;
}
