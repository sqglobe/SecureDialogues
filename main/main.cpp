#include <spdlog/sinks/stdout_color_sinks.h>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include "mainwindow.h"
#include "passask.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

#include <db_cxx.h>
#include <dbstl_common.h>

#include "core-initializer/coreinitializer.h"
#include "translation/translationmaster.h"
#include "utils/applicationsettings.h"

static const std::string FILE_DIGEST = "conf/pass.digest";

int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::debug);  // Set global log level to info
  spdlog::rotating_logger_mt("root_logger", "sequre-dialogues", 1048576 * 5, 3);
  dbstl::dbstl_startup();

  QApplication a(argc, argv);

  QDir curr;

  if (!curr.mkpath("conf")) {
    QMessageBox::critical(
        nullptr, "Abort!",
        QString("Выполнение приложение завершено из-за невозможности созать "
                "папку для конфигурации"));
    return 1;
  }

  try {
    auto pass = getPassword(FILE_DIGEST);
    std::shared_ptr<ApplicationSettingsGuard> settingsGuard =
        std::make_shared<ApplicationSettingsGuard>();
    std::shared_ptr<TranslationMaster> translatorMaster =
        std::make_shared<TranslationMaster>(
            settingsGuard->getSettings(),
            curr.absoluteFilePath("locale").toStdString());

    MainWindow w(std::move(settingsGuard), std::move(translatorMaster),
                 std::make_unique<CoreInitializer>(pass));
    w.show();

    auto res = a.exec();

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
