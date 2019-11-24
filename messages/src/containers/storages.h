#ifndef STORAGES_H
#define STORAGES_H

#include <persistent-storage/storages/childstorage.h>
#include <persistent-storage/storages/storage.h>

#include <persistent-storage/deleters/childthatisparentdeleter.h>
#include <persistent-storage/deleters/defaultchilddeleter.h>
#include <persistent-storage/deleters/parentsdeleter.h>
#include <persistent-storage/storages/registertransactionmanager.h>

#include <memory>

#include "eventwatchers.h"
#include "persistent_storage/connectionmarshaller.h"
#include "persistent_storage/contactmarshaller.h"
#include "persistent_storage/dialogmarshaller.h"
#include "primitives/connectionholder.h"
#include "primitives/contact.h"
#include "primitives/dialog.h"

std::string get_id(const ConnectionHolder& holder);
std::string get_id(const Dialog& holder);
std::string get_id(const Contact& holder);

// Make be better for compile time to replace using with inheritance
// becouse class may be declared in header and defined in cpp

using DialogStorage =
    prstorage::ChildStorage<Dialog,
                            Contact,
                            DialogMarshaller,
                            EventListeners<Dialog>,
                            prstorage::RegisterTransactionManager>;

using ContactDeleterType =
    prstorage::ChildThatIsParentDeleter<decltype(
                                            get_id(std::declval<Contact>())),
                                        Contact,
                                        ConnectionHolder,
                                        DialogStorage>;
using ContactStorage =
    prstorage::ChildStorage<Contact,
                            ConnectionHolder,
                            ContactMarshaller,
                            EventListeners<Contact>,
                            prstorage::RegisterTransactionManager,
                            ContactDeleterType>;

using ConnectionDeleter =
    prstorage::ParentsDeleter<decltype(
                                  get_id(std::declval<ConnectionHolder>())),
                              ConnectionHolder,
                              ContactStorage>;
using ConnectionStorage =
    prstorage::Storage<ConnectionHolder,
                       ConnectionMarshaller,
                       EventListeners<ConnectionHolder>,
                       prstorage::RegisterTransactionManager,
                       ConnectionDeleter>;

std::shared_ptr<DialogStorage> make_dialog_storage(Db* primary,
                                                   Db* secondary,
                                                   DbEnv* penv);
std::shared_ptr<ContactStorage> make_contact_storage(
    Db* primary,
    Db* secondary,
    DbEnv* penv,
    std::shared_ptr<DialogStorage> dialogs);
std::shared_ptr<ConnectionStorage> make_connection_storage(
    Db* primary,
    DbEnv* penv,
    std::shared_ptr<ContactStorage> contacts);

#endif  // STORAGES_H
