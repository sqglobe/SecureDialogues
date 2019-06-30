#ifndef DBFACTORY_H
#define DBFACTORY_H

#include <db_cxx.h>
#include <dbstl_map.h>

#include <functional>
#include <list>
#include <memory>

class RecoveryNeedError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class SharedMemoryLockError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class FileNotExistsError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class FailedOpenError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class DbFactory {
 public:
  DbFactory(const std::string& pass,
            const std::string& folder,
            const std::string& dbFile);
  std::shared_ptr<Db> getDb(const std::string& dbName);
  template <typename T>
  dbstl::db_map<std::string, T> makeContainer(const std::shared_ptr<Db>& db);
  ~DbFactory();

 public:
  DbFactory(const DbFactory&) = delete;
  DbFactory& operator=(const DbFactory&) = delete;

 private:
  std::unique_ptr<DbEnv, std::function<void(DbEnv*)>> mEnv;
  std::string mDbFile;
  std::list<std::weak_ptr<Db>> neededForDelete;
};
#endif  // DBFACTORY_H
