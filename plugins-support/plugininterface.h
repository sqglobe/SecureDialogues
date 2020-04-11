#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "not_owned.h"
#include "owneddeletior.h"

#include <map>
#include <memory>

namespace dynalo {
class library;
}  // namespace dynalo

class PluginFacade;
class PluginWidget;
class PluginConnectionSerializer;
class PluginMessageCommunicator;
class PluginConnectionInfo;
class PluginAddressValidator;

namespace plugin_support {

class PluginWidgetWrapper;
class PluginMessageCommunicatorWrapper;

struct PlugingMetaInfo {
  std::string name;
  std::string description;
  std::string gettextDomain;
  std::string localeFolder;
  std::map<Language, std::string> languages;
};

class PluginInterface : public std::enable_shared_from_this<PluginInterface> {
 public:
  PluginInterface(dynalo::library&& lib,
                  PluginFacade* facade,
                  PlugingMetaInfo&& info);

 public:
  std::unique_ptr<PluginWidgetWrapper> getWidget() noexcept;
  const PluginConnectionSerializer* getSerializer() const noexcept;
  std::shared_ptr<const PluginAddressValidator> getAddressValidator() const
      noexcept;
  std::unique_ptr<PluginMessageCommunicatorWrapper> getCommunicator() noexcept;
  std::shared_ptr<PluginConnectionInfo> makeConnInfo() const noexcept;

 public:
  std::string getName() const noexcept;
  std::string getId() const noexcept;
  std::string getTranslationFileName(Language lang) const noexcept;
  std::string getGettextDomain() const noexcept;
  std::string getLocaleFolder() const noexcept;

 private:
  std::unique_ptr<dynalo::library> mLib;
  PluginFacade* mFacade;
  PlugingMetaInfo mMetaInfo;
};

}  // namespace plugin_support

#endif  // PLUGININTERFACE_H
