#include <assert.h>
#include <QTest>
#include <iostream>
#include <memory>
#include "containers/basecontainer.h"
#include "containers/contactcontainer.h"
#include "containers/transparentcontainerelementwrapper.h"
struct A {
  int a;
};

class TestTrasparentContainerElementWrapper : public QObject {
  Q_OBJECT

 private Q_SLOTS:
  void testWrapperForPlainContainerElements();
  void testModifyThroughWrapper();
  void testCancelThroughWrapper();
  void testRemoveThroughWrapper();
};

void TestTrasparentContainerElementWrapper::
    testWrapperForPlainContainerElements() {
  static_assert(
      std::is_same<decltype(std::declval<TransparentContainerElementWrapper<
                                BaseContainer<A, A>>>()
                                .
                                operator->()),
                   A*>::value,
      "not same");

  static_assert(
      std::is_same<
          decltype(std::declval<TransparentContainerElementWrapper<
                       BaseContainer<std::shared_ptr<A>, std::shared_ptr<A>>>>()
                       .
                       operator->()),
          std::shared_ptr<A>&>::value,
      "not same");

  static_assert(
      std::is_same<decltype(std::declval<TransparentContainerElementWrapper<
                                BaseContainer<A*, A*>>>()
                                .
                                operator->()),
                   A*>::value,
      "not same");
}

void TestTrasparentContainerElementWrapper::testModifyThroughWrapper() {
  auto dialogs = std::make_shared<ContactContainer>();
  dialogs->add(std::make_shared<Contact>("test_channel", "test_name",
                                         "test_addr", "no pass", "id"));
  dialogs->add(std::make_shared<Contact>("test_channel1", "test_name1",
                                         "test_addr1", "no pass", "id1"));

  auto wrapper1 = dialogs->wrapper("id");
  wrapper1->name("name 2");
  wrapper1.save();

  QCOMPARE(dialogs->get("id")->name(), std::string("name 2"));
  QCOMPARE(dialogs->get("id1")->name(), std::string("test_name1"));
}

void TestTrasparentContainerElementWrapper::testCancelThroughWrapper() {
  auto dialogs = std::make_shared<ContactContainer>();
  dialogs->add(std::make_shared<Contact>("test_channel", "test_name",
                                         "test_addr", "no pass", "id"));
  dialogs->add(std::make_shared<Contact>("test_channel1", "test_name1",
                                         "test_addr1", "no pass", "id1"));

  auto wrapper1 = dialogs->wrapper("id");
  wrapper1->name("name 2");

  QCOMPARE(dialogs->get("id")->name(), std::string("test_name"));
  QCOMPARE(dialogs->get("id1")->name(), std::string("test_name1"));
}

void TestTrasparentContainerElementWrapper::testRemoveThroughWrapper() {
  auto dialogs = std::make_shared<ContactContainer>();
  dialogs->add(std::make_shared<Contact>("test_channel", "test_name",
                                         "test_addr", "no pass", "id"));
  dialogs->add(std::make_shared<Contact>("test_channel1", "test_name1",
                                         "test_addr1", "no pass", "id1"));

  auto wrapper1 = dialogs->wrapper("id");
  wrapper1->name("name 2");
  wrapper1.remove();

  QCOMPARE(dialogs->get("id1")->name(), std::string("test_name1"));
  QVERIFY(!dialogs->has("id"));
}

QTEST_APPLESS_MAIN(TestTrasparentContainerElementWrapper)

#include "testtrasparentcontainerelementwrapper.moc"
