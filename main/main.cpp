#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include "mainwindow.h"
#include "passask.h"
#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>

static const std::string FILE_DIGEST = "conf/pass.digest";

int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::debug);  // Set global log level to info
  auto console = spdlog::stdout_color_mt("main");
  console->warn("Start app");
  spdlog::get("main")->info("debug throw get");

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

    MainWindow w(pass);
    w.show();

    return a.exec();
  } catch (std::exception& ex) {
    QMessageBox::critical(
        nullptr, "Abort!",
        QString("Выполнение приложение завершено с сообщением ")
            .append(ex.what()));
  }
  return 123;
}
