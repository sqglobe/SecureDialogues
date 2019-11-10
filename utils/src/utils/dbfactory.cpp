#include "dbfactory.h"

#include <db_cxx.h>
#include <dbstl_common.h>

DbEnv* make_db_env(const std::string& path, const std::string& pass) {
  auto penv = new DbEnv(DB_CXX_NO_EXCEPTIONS);
  penv->set_encrypt(pass.c_str(), DB_ENCRYPT_AES);
  auto res = penv->open(path.c_str(),
                        DB_INIT_LOCK | DB_INIT_MPOOL | DB_INIT_TXN | DB_CREATE |
                            DB_RECOVER | DB_PRIVATE | DB_THREAD | DB_ENCRYPT,
                        0600);
  if (res) {
    delete penv;
    throw std::runtime_error(
        std::string("Error occur during system intialisation ").append(path));
  }
  dbstl::register_db_env(penv);
  return penv;
}

Db* make_db(const std::string& file,
            const std::string& name,
            DbEnv* penv,
            u_int32_t flags) {
  auto db = new Db(penv, DB_CXX_NO_EXCEPTIONS);
  db->set_flags(flags | DB_ENCRYPT | DB_TXN_NOT_DURABLE);
  auto res = db->open(nullptr, file.c_str(), name.c_str(), DB_BTREE,
                      DB_CREATE | DB_THREAD | DB_AUTO_COMMIT, 0600);

  if (res) {
    delete db;
    throw std::runtime_error(
        std::string("Error occur during database intialisation ")
            .append(file)
            .append(" name ")
            .append(name));
  }
  dbstl::register_db(db);
  return db;
}
