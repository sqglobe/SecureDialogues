#include "plugindiscovery.h"

#include <boost/dll/shared_library.hpp>
#include <boost/filesystem.hpp>
#include "pluginscontainer.h"

#include "export/pluginfacade.h"
#include "plugininterface.h"

void plugin_support::discover_plugins(const std::string& path,
                                      PluginsContainer& container) {
  using namespace boost::filesystem;
  boost::filesystem::path rootPath{path};
  for (directory_iterator iter{rootPath}; iter != directory_iterator{};
       ++iter) {
    const auto& entery = *iter;
    if (entery.status().type() == regular_file) {
      boost::dll::fs::error_code code;
      boost::dll::shared_library lib(entery.path(), code,
                                     boost::dll::load_mode::rtld_now |
                                         boost::dll::load_mode::rtld_local |
                                         boost::dll::load_mode::rtld_deepbind);

      if (!code && lib.has("get_secure_dialogues_plugin_facade")) {
        auto func =
            lib.get<PluginFacade*()>("get_secure_dialogues_plugin_facade");
        container.add(
            std::make_shared<PluginInterface>(std::move(lib), func()));
      }
    }
  }
}
