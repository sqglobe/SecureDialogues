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
#include "plugindiscovery.h"
#include "pluginscontainer.h"
#include "translation/translationmaster.h"
#include "utils/applicationsettings.h"

#include "fmt/core.h"

#include <libintl.h>
#include <locale.h>
#include <iostream>

static const std::string FILE_DIGEST = "conf/pass.digest";

int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::debug);  // Set global log level to info
  spdlog::rotating_logger_mt("root_logger", "sequre-dialogues", 1048576 * 5, 3);
  dbstl::dbstl_startup();

  QApplication a(argc, argv);

  QDir curr;

  std::shared_ptr<ApplicationSettingsGuard> settingsGuard =
      std::make_shared<ApplicationSettingsGuard>();

  if (!curr.mkpath("conf")) {
    QMessageBox::critical(nullptr, "Abort!",
                          dgettext("SecureDialogues",
                                   "Application was aborted because of failed "
                                   "creation of configuration folder"));
    return 1;
  }

  try {
    auto pass = getPassword(FILE_DIGEST);
    auto container = std::make_shared<plugin_support::PluginsContainer>();
    plugin_support::discover_plugins(
        "plugins", curr.absoluteFilePath("plugin.schema.json").toStdString(),
        *container);

    std::shared_ptr<TranslationMaster> translatorMaster =
        std::make_shared<TranslationMaster>(
            settingsGuard->getSettings(),
            curr.absoluteFilePath("locale").toStdString(), container);

    MainWindow w(std::move(settingsGuard), std::move(translatorMaster),
                 std::make_unique<CoreInitializer>(pass, container), container);
    w.show();

    auto res = a.exec();

    dbstl::dbstl_exit();

    return res;
  } catch (std::exception& ex) {
    QMessageBox::critical(
        nullptr, "Abort!",
        fmt::format(dgettext("SecureDialogues",
                             "Application was aborted with message {}"),
                    ex.what())
            .c_str());
    dbstl::dbstl_exit();
  }
  return 123;
}
