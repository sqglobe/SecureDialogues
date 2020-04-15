#include "dummyplugindefs.h"

#include "inner/plugininnerdefs.h"

extern "C" PluginFacade* get_secure_dialogues_plugin_facade() {
  //  return &facade;
}

const char* getPluginId() {
  return "a2e86280-91ff-4291-86f0-03b23a5e3370";
}
