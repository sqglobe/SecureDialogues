#include "dialogsavestrategy.h"
#include <sstream>
#include "primitives/dialog.h"
#include "utils.h"

void DialogSaveStrategy::setContactContainer(
    const std::shared_ptr<const ContactContainer>& cc) {
  mContactContainer = cc;
}

std::vector<typename DialogSaveStrategy::container::const_element>
DialogSaveStrategy::getContainerData(
    const typename DialogSaveStrategy::container& container) const {
  return container.getAllElements();
}

std::string DialogSaveStrategy::to_str(
    const typename DialogSaveStrategy::container::const_element& obj) const {
  std::stringstream ss;

  ss << obj->getDialogId() << ";" << obj->getContactId() << ";"
     << static_cast<int>(obj->getStatus());

  return ss.str();
}

typename DialogSaveStrategy::container::element DialogSaveStrategy::from_str(
    const std::string& row) const {
  assert(mContactContainer);
  std::vector<std::string> cols;
  split(row, ';', std::back_insert_iterator<std::vector<std::string>>(cols));
  if (cols.size() < 3) {
    throw std::runtime_error("Cols count less then 3");
  }

  auto contact = mContactContainer->get(cols[1]);

  return std::make_shared<Dialog>(
      contact, cols[0], static_cast<Dialog::Status>(std::stoi(cols[2])));
}

void DialogSaveStrategy::add(
    typename DialogSaveStrategy::container& container,
    const typename DialogSaveStrategy::container::element& obj) {
  container.add(obj);
}
