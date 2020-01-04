#include "storages.h"

std::string get_id(const ConnectionHolder& holder) {
  return holder.getConnectionName();
}

std::string get_id(const Dialog& holder) {
  return std::string(holder.getDialogId());
}

std::string get_id(const Contact& holder) {
  return std::string(holder.id());
}

int get_dialog_secondary_index(Db* /* secondary */,
                               const Dbt* /* key */,
                               const Dbt* data,
                               Dbt* result) {
  Dialog d;
  DialogMarshaller::restore(d, data->get_data());
  auto contactId = d.getContactId();
  if (auto chars = static_cast<char*>(malloc(contactId.size() + 1))) {
    memset(chars, 0, contactId.size() + 1);
    result->set_flags(DB_DBT_APPMALLOC);
    strncpy(chars, contactId.data(), contactId.size());
    result->set_data(chars);
    result->set_size(static_cast<uint32_t>(contactId.size()) + 1);
    return 0;
  }
  return 1;
}

int get_contact_secondary_index(Db* /* secondary */,
                                const Dbt* /* key */,
                                const Dbt* data,
                                Dbt* result) {
  Contact contact;
  ContactMarshaller::restore(contact, data->get_data());
  auto connName = contact.channelMoniker();
  if (auto chars = static_cast<char*>(malloc(connName.size() + 1))) {
    memset(chars, 0, connName.size() + 1);
    result->set_flags(DB_DBT_APPMALLOC);
    strncpy(chars, connName.data(), connName.size());
    result->set_data(chars);
    result->set_size(static_cast<uint32_t>(connName.size()) + 1);
    return 0;
  }
  return 1;
}

std::shared_ptr<DialogStorage> make_dialog_storage(Db* primary,
                                                   Db* secondary,
                                                   DbEnv* penv) {
  primary->associate(nullptr, secondary, get_dialog_secondary_index, DB_CREATE);
  return std::make_shared<DialogStorage>(primary, secondary, penv);
}

std::shared_ptr<ContactStorage> make_contact_storage(
    Db* primary,
    Db* secondary,
    DbEnv* penv,
    std::shared_ptr<DialogStorage> dialogs) {
  primary->associate(nullptr, secondary, get_contact_secondary_index,
                     DB_CREATE);
  return std::make_shared<ContactStorage>(
      primary, secondary, penv, ContactDeleterType(std::move(dialogs)));
}

std::shared_ptr<ConnectionStorage> make_connection_storage(
    Db* primary,
    DbEnv* penv,
    std::shared_ptr<ContactStorage> contacts) {
  return std::make_shared<ConnectionStorage>(
      primary, penv, ConnectionDeleter(std::move(contacts)));
}

std::string get_id(const DiscoveredContact& holder) {
  return std::string(holder.dialog_id());
}

std::shared_ptr<DiscoveredContactStorage> make_discovered_contact_storage(
    Db* primary,
    DbEnv* penv) {
  return std::make_shared<DiscoveredContactStorage>(primary, penv);
}
