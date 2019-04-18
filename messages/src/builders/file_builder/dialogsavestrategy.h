#ifndef DIALOGSAVESTRATEGY_H
#define DIALOGSAVESTRATEGY_H

#include <memory>
#include <vector>
#include "containers/contactcontainer.h"
#include "containers/dialogmanager.h"

class DialogSaveStrategy {
 public:
  using container = DialogManager;

 public:
  void setContactContainer(const std::shared_ptr<const ContactContainer>& cc);

 public:
  std::vector<typename container::const_element> getContainerData(
      const container& container) const;

  std::string to_str(const typename container::const_element& obj) const;
  typename container::element from_str(const std::string& row) const;

  void add(container& container, const typename container::element& obj);

 private:
  std::shared_ptr<const ContactContainer> mContactContainer;
};

#endif  // DIALOGSAVESTRATEGY_H
