#include "contactsavestrategy.h"
#include <sstream>
#include "utils.h"

std::string ReplaceAll(std::string str,
                       const std::string& from,
                       const std::string& to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length();  // Handles case where 'to' is a substring of 'from'
  }
  return str;
}

std::vector<typename ContactSaveStrategy::container::const_element>
ContactSaveStrategy::getContainerData(
    const typename ContactSaveStrategy::container& container) {
  return container.getAllElements();
}

std::string ContactSaveStrategy::to_str(
    const typename ContactSaveStrategy::container::const_element& obj) {
  std::stringstream ss;
  ss << obj->adress() << ";" << obj->channelMoniker() << ";" << obj->name()
     << ";" << obj->id() << ";" << ReplaceAll(obj->publicKey(), "\n", ",");
  return ss.str();
}

typename ContactSaveStrategy::container::element ContactSaveStrategy::from_str(
    const std::string& row) {
  std::vector<std::string> cols;
  split(row, ';', std::back_insert_iterator<std::vector<std::string>>(cols));
  if (cols.size() < 5) {
    throw std::runtime_error("Cols count less then 4");
  }
  return std::make_shared<Contact>(cols[1], cols[2], cols[0],
                                   ReplaceAll(cols[4], ",", "\n"), cols[3]);
}

void ContactSaveStrategy::add(
    typename ContactSaveStrategy::container& container,
    const typename ContactSaveStrategy::container::element& obj) {
  container.add(obj);
}
