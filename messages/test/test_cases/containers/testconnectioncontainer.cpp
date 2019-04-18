#include <QTest>
#include <atomic>
#include <thread>
#include "communication/channel.h"
#include "communication/messagedespatcher.h"
#include "containers/connectcontainerwatcher.h"
#include "containers/connectioninfocontainer.h"
#include "cryptosystemfake.h"
#include "fakenotifier.h"
#include "interfaces/abstractchanneladapter.h"
#include "interfaces/changewatcher.h"
#include "primitives/connectionholder.h"
#include "utils/messagemarshaller.h"

#include "fakenotifier.h"
#include "interfaces/cryptosystem.h"

#include <functional>
#include <memory>

std::shared_ptr<MessageDespatcher> makeFakeMessageDispatcher() {
  return std::make_shared<MessageDespatcher>(
      std::make_shared<CryptoSystemFake>(), std::make_shared<FakeNotifier>());
}

class TestConnectionContainer : public QObject {
  Q_OBJECT
 public:
  explicit TestConnectionContainer(QObject* parent = nullptr);

 signals:

 private slots:
  void testAddConnectionInfo();

  void testUpdateConnectionInfo();
  void testUpdateConnectionInfoAtPos();
  void testRemoveAtChannelName();
  void testRemoveAtPosition();
};

class FakeCalledRecieveAdapter : public AbstractChannelAdapter {
 public:
  FakeCalledRecieveAdapter(
      std::atomic<bool>& recieveCalled,
      std::atomic<bool>& destructorCalled,
      const std::shared_ptr<AbstractUserNotifier>& notifier) :
      AbstractChannelAdapter(notifier),
      mRecieveCall(recieveCalled), mDestructorCall(destructorCalled) {}
  ~FakeCalledRecieveAdapter() override { mDestructorCall = true; }

 public:
  virtual void send([[maybe_unused]] const std::string& message,
                    [[maybe_unused]] const std::string& adress) override {}
  virtual std::pair<std::string, std::string> receive() override {
    mRecieveCall = true;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return std::pair<std::string, std::string>("", "");
  }
  virtual bool connect([[maybe_unused]] const ConnectionHolder& conn) override {
    return true;
  }

  virtual bool isConnected() const noexcept override { return true; }

 private:
  std::atomic<bool>& mRecieveCall;
  std::atomic<bool>& mDestructorCall;
};

class FakeConnInfoWatcher : public ChangeWatcher<ConnectionHolder> {
 public:
  virtual void added([[maybe_unused]] const element& obj) override {
    mMethodCalled = "addedElement";
  }
  virtual void changed([[maybe_unused]] const element& obj) override {
    mMethodCalled = "updatedElement";
  }
  virtual void removed([[maybe_unused]] const element& obj) override {
    mMethodCalled = "removedElement";
  }

 public:
  std::string mMethodCalled;
};

TestConnectionContainer::TestConnectionContainer(QObject* parent) :
    QObject(parent) {}

void TestConnectionContainer::testAddConnectionInfo() {
  std::atomic<bool> isRecieveCalled(false);
  std::atomic<bool> isDestructor(false);
  auto fakeFactory = [&isRecieveCalled, &isDestructor](
                         [[maybe_unused]] const ConnectionHolder& info) {
    return std::unique_ptr<AbstractChannelAdapter>(new FakeCalledRecieveAdapter(
        isRecieveCalled, isDestructor, std::make_shared<FakeNotifier>()));
  };

  auto messageDispatcher = makeFakeMessageDispatcher();
  auto connectWatcher = std::make_shared<ConnectContainerWatcher>(
      messageDispatcher,
      std::function<std::unique_ptr<AbstractChannelAdapter>(
          const ConnectionHolder&)>(fakeFactory),
      std::make_shared<MessageMarshaller>());
  ConnectionInfoContainer connCont;
  connCont.registerWatcher(connectWatcher);

  auto watcher = std::make_shared<FakeConnInfoWatcher>();
  connCont.registerWatcher(watcher);
  connCont.add(
      ConnectionHolder(GmailConnaection{"fake@gmail.com", "234"}, "fake"));
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  QVERIFY(isRecieveCalled.load());
  QVERIFY(connCont.has("fake"));
  QCOMPARE(watcher->mMethodCalled, std::string("addedElement"));
  QCOMPARE(connCont.size(), 1);
  QCOMPARE(connCont.at(0).getConnectionName(), std::string("fake"));
  QCOMPARE(connCont.at(0).getType(), ConnectionType::GMAIL);

  auto gmailConn = connCont.at(0).getConnection<GmailConnaection>();

  QCOMPARE(gmailConn.email, std::string("fake@gmail.com"));
}

void TestConnectionContainer::testUpdateConnectionInfo() {
  std::atomic<bool> isRecieveCalled(false);
  std::atomic<bool> isDestructor(false);
  auto fakeFactory = [&isRecieveCalled, &isDestructor](
                         [[maybe_unused]] const ConnectionHolder& info) {
    return std::unique_ptr<AbstractChannelAdapter>(new FakeCalledRecieveAdapter(
        isRecieveCalled, isDestructor, std::make_shared<FakeNotifier>()));
  };
  auto messageDispatcher = makeFakeMessageDispatcher();
  auto connectWatcher = std::make_shared<ConnectContainerWatcher>(
      messageDispatcher,
      std::function<std::unique_ptr<AbstractChannelAdapter>(
          const ConnectionHolder&)>(fakeFactory),
      std::make_shared<MessageMarshaller>());
  ConnectionInfoContainer connCont;
  connCont.registerWatcher(connectWatcher);
  auto watcher = std::make_shared<FakeConnInfoWatcher>();
  connCont.registerWatcher(watcher);
  connCont.add(
      ConnectionHolder(GmailConnaection{"fake@gmail.com", "234"}, "fake"));
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  isRecieveCalled = false;
  isDestructor = false;

  connCont.update(
      ConnectionHolder(GmailConnaection{"fake@gmail.com", "234231"}, "fake"));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  QVERIFY(isRecieveCalled.load());
  QVERIFY(isDestructor.load());
  QCOMPARE(watcher->mMethodCalled, std::string("updatedElement"));
}

void TestConnectionContainer::testUpdateConnectionInfoAtPos() {
  std::atomic<bool> isRecieveCalled(false);
  std::atomic<bool> isDestructor(false);
  auto fakeFactory = [&isRecieveCalled, &isDestructor](
                         [[maybe_unused]] const ConnectionHolder& info) {
    return std::unique_ptr<AbstractChannelAdapter>(new FakeCalledRecieveAdapter(
        isRecieveCalled, isDestructor, std::make_shared<FakeNotifier>()));
  };
  auto messageDispatcher = makeFakeMessageDispatcher();
  auto connectWatcher = std::make_shared<ConnectContainerWatcher>(
      messageDispatcher,
      std::function<std::unique_ptr<AbstractChannelAdapter>(
          const ConnectionHolder&)>(fakeFactory),
      std::make_shared<MessageMarshaller>());
  ConnectionInfoContainer connCont;
  connCont.registerWatcher(connectWatcher);
  auto watcher = std::make_shared<FakeConnInfoWatcher>();
  connCont.registerWatcher(watcher);
  connCont.add(
      ConnectionHolder(GmailConnaection{"fake@gmail.com", "234"}, "fake"));
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  isRecieveCalled = false;
  isDestructor = false;

  connCont.update(
      ConnectionHolder(GmailConnaection{"fake@gmail.com", "2331234"}, "fake"));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  QVERIFY(isRecieveCalled.load());
  QVERIFY(isDestructor.load());
  QCOMPARE(watcher->mMethodCalled, std::string("updatedElement"));
}

void TestConnectionContainer::testRemoveAtChannelName() {
  std::atomic<bool> isRecieveCalled(false);
  std::atomic<bool> isDestructor(false);
  auto fakeFactory = [&isRecieveCalled,
                      &isDestructor](const ConnectionHolder&) {
    return std::unique_ptr<AbstractChannelAdapter>(new FakeCalledRecieveAdapter(
        isRecieveCalled, isDestructor, std::make_shared<FakeNotifier>()));
  };
  auto messageDispatcher = makeFakeMessageDispatcher();
  auto connectWatcher = std::make_shared<ConnectContainerWatcher>(
      messageDispatcher,
      std::function<std::unique_ptr<AbstractChannelAdapter>(
          const ConnectionHolder&)>(fakeFactory),
      std::make_shared<MessageMarshaller>());
  ConnectionInfoContainer connCont;
  connCont.registerWatcher(connectWatcher);
  auto watcher = std::make_shared<FakeConnInfoWatcher>();
  connCont.registerWatcher(watcher);
  connCont.add(
      ConnectionHolder(GmailConnaection{"fake@gmail.com", "2331234"}, "fake"));
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  isRecieveCalled = false;
  isDestructor = false;

  connCont.remove("fake");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  QVERIFY(isDestructor.load());
  QCOMPARE(watcher->mMethodCalled, std::string("removedElement"));
}

void TestConnectionContainer::testRemoveAtPosition() {
  std::atomic<bool> isRecieveCalled(false);
  std::atomic<bool> isDestructor(false);
  auto fakeFactory = [&isRecieveCalled,
                      &isDestructor](const ConnectionHolder&) {
    return std::unique_ptr<AbstractChannelAdapter>(new FakeCalledRecieveAdapter(
        isRecieveCalled, isDestructor, std::make_shared<FakeNotifier>()));
  };
  auto messageDispatcher = makeFakeMessageDispatcher();
  auto connectWatcher = std::make_shared<ConnectContainerWatcher>(
      messageDispatcher,
      std::function<std::unique_ptr<AbstractChannelAdapter>(
          const ConnectionHolder&)>(fakeFactory),
      std::make_shared<MessageMarshaller>());
  ConnectionInfoContainer connCont;
  connCont.registerWatcher(connectWatcher);
  connCont.add(
      ConnectionHolder(GmailConnaection{"fake@gmail.com", "2331234"}, "fake"));
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  auto watcher = std::make_shared<FakeConnInfoWatcher>();
  connCont.registerWatcher(watcher);

  isRecieveCalled = false;
  isDestructor = false;

  connCont.remove(0);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  QVERIFY(isDestructor.load());
  QCOMPARE(watcher->mMethodCalled, std::string("removedElement"));
}

QTEST_APPLESS_MAIN(TestConnectionContainer)

#include "testconnectioncontainer.moc"
