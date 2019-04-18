#include <QTest>
#include "utils/timeoutedrepository.h"

const static std::string REMOVED = "removed";
const static std::string TIMEOUTED = "timeouted";
static std::string last_oper = "";

namespace std {

void removed(const std::string& str) {
  last_oper = REMOVED;
  QCOMPARE(str, REMOVED);
}

void timeouted(const std::string& str) {
  last_oper = TIMEOUTED;
  QCOMPARE(str, TIMEOUTED);
}
}  // namespace std

class TimeoutedRepositoryTest : public QObject {
  Q_OBJECT

 public:
 private slots:
  void testTimeoutedAndNormalRemoveCases();
};

void TimeoutedRepositoryTest::testTimeoutedAndNormalRemoveCases() {
  auto tmRepo = TimeoutedRrepository<std::string, std::string::size_type>::make(
      std::chrono::seconds(10), std::chrono::milliseconds(1));
  tmRepo->store(REMOVED, REMOVED.size(), std::chrono::seconds(1));
  tmRepo->store(TIMEOUTED, TIMEOUTED.size(), std::chrono::seconds(2));
  std::this_thread::sleep_for(std::chrono::milliseconds(900));
  tmRepo->remove(REMOVED.size());
  QCOMPARE(last_oper, REMOVED);
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  QCOMPARE(last_oper, TIMEOUTED);
}

QTEST_APPLESS_MAIN(TimeoutedRepositoryTest)

#include "timeoutedrepositorytest.moc"
