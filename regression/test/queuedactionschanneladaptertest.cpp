#include <QtTest>
#include "stand-helpers/queuedactionschanneladapter.h"

Q_DECLARE_METATYPE(std::string);

class FakeNotifier : public AbstractUserNotifier {
 public:
  void notify(Severity, const std::string&) override {}
};

class QueuedActionsChannelAdapterTest : public QObject {
  Q_OBJECT

 public:
 private Q_SLOTS:
  void testRecieveFromApplication();
  void testRecieveFromApplication_data();

  void testSendToApplication();
  void testSendToApplication_data();
};

void QueuedActionsChannelAdapterTest::testRecieveFromApplication() {
  QFETCH(std::string, address);
  QFETCH(std::string, message);

  QueuedActionsChannelAdapter adapter(std::make_shared<FakeNotifier>(),
                                      ConnectionHolder("no"));
  std::async(std::launch::async, [&adapter, message, address]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    adapter.send(message, address);
  });

  auto res = adapter.recieveMessageFromApplication(200);
  QVERIFY(res.has_value());
  QCOMPARE(address, res->address);
  QCOMPARE(message, res->message);
}

void QueuedActionsChannelAdapterTest::testRecieveFromApplication_data() {
  QTest::addColumn<std::string>("address");
  QTest::addColumn<std::string>("message");

  QTest::newRow("1") << std::string("test addr 1")
                     << std::string("test message1");
  QTest::newRow("2") << std::string("test addr 2")
                     << std::string("test message2");
  QTest::newRow("3") << std::string("test addr 3")
                     << std::string("test message3");
  QTest::newRow("4") << std::string("test addr 4")
                     << std::string("test message4");
}

void QueuedActionsChannelAdapterTest::testSendToApplication() {
  QFETCH(std::string, address);
  QFETCH(std::string, message);

  QueuedActionsChannelAdapter adapter(std::make_shared<FakeNotifier>(),
                                      ConnectionHolder("no"));

  std::async(std::launch::async, [&adapter, message, address]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    adapter.sendMessageToApplication({address, message});
  });

  auto res = adapter.receive();

  QCOMPARE(res.first, address);
  QCOMPARE(res.second, message);
}

void QueuedActionsChannelAdapterTest::testSendToApplication_data() {
  QTest::addColumn<std::string>("address");
  QTest::addColumn<std::string>("message");

  QTest::newRow("1") << std::string("test addr 1")
                     << std::string("test message1");
  QTest::newRow("2") << std::string("test addr 2")
                     << std::string("test message2");
  QTest::newRow("3") << std::string("test addr 3")
                     << std::string("test message3");
  QTest::newRow("4") << std::string("test addr 4")
                     << std::string("test message4");
}

QTEST_APPLESS_MAIN(QueuedActionsChannelAdapterTest)

#include "queuedactionschanneladaptertest.moc"
