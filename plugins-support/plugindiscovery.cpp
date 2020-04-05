#include "plugindiscovery.h"

#include <boost/dll/shared_library.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

#include "pluginscontainer.h"

#include "export/pluginfacade.h"
#include "plugininterface.h"

#include <algorithm>
#include <nlohmann/json-schema.hpp>
#include "spdlog/spdlog.h"

#include "api-version.h"

static bool is_plugin_version_compatable(const PluginApiVersion& api,
                                         const nlohmann::json& plugin);
static plugin_support::PlugingMetaInfo get_meta(
    const boost::filesystem::path& rootPath,
    const nlohmann::json& plugin);

class MyValidator {
 public:
  MyValidator(const std::string& path_to_schema);

 public:
  std::pair<bool, nlohmann::json> isValid(const std::string& path) const;

 private:
  nlohmann::json_schema::json_validator mValidator;
  bool mFileLoaded{false};
};

MyValidator::MyValidator(const std::string& path_to_schema) {
  std::ifstream in{path_to_schema};
  if (!in)
    return;
  try {
    nlohmann::json schema = nlohmann::json::parse(in);
    mValidator.set_root_schema(schema);
    mFileLoaded = true;
  } catch (const std::exception& ex) {
    spdlog::get("root_logger")
        ->warn("Failed to load plugin description schema {0} with error: {1}",
               path_to_schema, ex.what());
  }
}

std::pair<bool, nlohmann::json> MyValidator::isValid(
    const std::string& path) const {
  try {
    std::ifstream in{path};

    if (!in)
      return {false, {}};

    nlohmann::json descr = nlohmann::json::parse(in);

    if (!mFileLoaded)
      return {true, descr};

    mValidator.validate(descr);

    return {is_plugin_version_compatable(ApiVersion, descr), descr};
  } catch (const std::exception& e) {
    spdlog::get("root_logger")
        ->warn("Failed to validate schema with error {0}", e.what());
  }

  return {false, {}};
}

bool is_plugin_version_compatable(const PluginApiVersion& api,
                                  const nlohmann::json& plugin) {
  const auto v = plugin.at("api-version");
  return api.major == v.at("major").get<unsigned int>() &&
         api.minor == v.at("minor").get<unsigned int>() &&
         api.revision == v.at("revision").get<unsigned int>();
}

static Language convert_to_lang(const std::string& str) {
  if (str == "ru") {
    return Language::RU;
  } else if (str == "en") {
    return Language::EN;
  } else {
    assert(false);
  }
  return Language::EN;
}

plugin_support::PlugingMetaInfo get_meta(
    const boost::filesystem::path& rootPath,
    const nlohmann::json& plugin) {
  plugin_support::PlugingMetaInfo meta{
      plugin.at("name").get<std::string>(),
      plugin.at("descripton").get<std::string>(),
      plugin.at("gettext-domain").get<std::string>(),
      (rootPath / plugin.at("language").at("folder").get<std::string>())
          .native(),
      {}};

  const auto transl = plugin.at("language").at("translations");
  std::transform(
      std::cbegin(transl), std::cend(transl),
      std::inserter(meta.languages, meta.languages.end()),
      [](const nlohmann::json& item) -> std::pair<Language, std::string> {
        return {convert_to_lang(item.at("lang").get<std::string>()),
                item.at("name").get<std::string>()};
      });

  return meta;
}

void plugin_support::discover_plugins(const std::string& path,
                                      const std::string& schema_path,
                                      PluginsContainer& container) {
  using namespace boost::filesystem;
  using nlohmann::json;
  using nlohmann::json_schema::json_validator;
  MyValidator validator(schema_path);

  boost::filesystem::path rootPath{path};
  for (directory_iterator iter{rootPath}; iter != directory_iterator{};
       ++iter) {
    const auto& entery = *iter;
    if (entery.status().type() == directory_file &&
        entery.path().extension() == ".plugin") {
      const auto descriptionPath = entery.path() / "description.json";

      std::ifstream in{descriptionPath.string()};

      if (auto [is_valid, description] =
              validator.isValid(descriptionPath.string());
          is_valid) {
        boost::dll::fs::error_code code;

        boost::dll::shared_library lib(
            entery.path() / description.at("root-module").get<std::string>(),
            code,
            boost::dll::load_mode::rtld_now |
                boost::dll::load_mode::rtld_local |
                boost::dll::load_mode::rtld_deepbind);

        if (!code && lib.has("get_secure_dialogues_plugin_facade")) {
          auto func =
              lib.get<PluginFacade*()>("get_secure_dialogues_plugin_facade");
          container.add(std::make_shared<PluginInterface>(
              std::move(lib), func(), get_meta(entery.path(), description)));
        }
      }
    }
  }
}
