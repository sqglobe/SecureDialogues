#ifndef DBFACTORY_H
#define DBFACTORY_H

#include <string>
class DbEnv;
class Db;

DbEnv* make_db_env(const std::string& path, const std::string& pass);
Db* make_db(const std::string& file,
            const std::string& name,
            DbEnv* penv,
            u_int32_t flags = 0);

#endif  // DBFACTORY_H
