#include "dbfactory.h"

std::unique_ptr<DbEnv, std::function<void(DbEnv*)>> make_env() {
  return std::unique_ptr<DbEnv, std::function<void(DbEnv*)>>(
      new DbEnv(DB_CXX_NO_EXCEPTIONS), [](DbEnv* env) {
        env->close(0u);
        delete env;
      });
}

DbFactory::DbFactory(const std::string& pass,
                     const std::string& folder,
                     const std::string& dbFile) :
    mEnv(make_env()),
    mDbFile(dbFile) {
  mEnv->set_encrypt(pass.c_str(), DB_ENCRYPT_AES);
  mEnv->set_error_stream(&std::cerr);
  mEnv->set_message_stream(&std::cout);
  mEnv->set_cachesize(0, 64 * 1024, 0);
  if (auto i =
          mEnv->open(folder.c_str(),
                     DB_INIT_MPOOL | DB_CREATE | DB_PRIVATE | DB_THREAD, 0);
      i != 0) {
    switch (i) {
      case DB_RUNRECOVERY:
        throw RecoveryNeedError("Recovery needed for " + folder);
      case EAGAIN:
        throw SharedMemoryLockError("Shared memory locked for " + folder);
      case ENOENT:
        throw FileNotExistsError("File " + folder + " not exists");
      default:
        throw FailedOpenError("Failed open env in " + folder);
    }
  }
}

std::shared_ptr<Db> DbFactory::getDb(const std::string& dbName) {
  std::shared_ptr<Db> res(new Db(mEnv.get(), DB_CXX_NO_EXCEPTIONS), [](Db* db) {
    db->close(0u);
    delete db;
  });
  res->set_flags(DB_ENCRYPT);
  res->set_error_stream(&std::cerr);
  res->set_message_stream(&std::cout);
  if (auto i = res->open(nullptr, mDbFile.c_str(), dbName.c_str(), DB_HASH,
                         DB_CREATE | DB_THREAD, 0600);
      i != 0) {
    switch (i) {
      case ENOENT:
        throw FileNotExistsError("File " + mDbFile + " not exists");
      default:
        throw FailedOpenError("Failed open db " + dbName);
    }
  }
  neededForDelete.push_back(std::weak_ptr<Db>(res));
  return res;
}

DbFactory::~DbFactory() {
  for (auto e : neededForDelete) {
    if (auto lock = e.lock()) {
      lock->close(0u);
    }
  }
}

template <typename T>
dbstl::db_map<std::string, T> DbFactory::makeContainer(
    const std::shared_ptr<Db>& db) {
  dbstl::db_map<std::string, T> res(db.get(), mEnv.get());
  return res;
}
