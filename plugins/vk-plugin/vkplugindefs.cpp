#include "vkplugindefs.h"

#include "inner/plugininnerdefs.h"

#include "vkfacade.h"

static VkFacade facade;

extern "C" PluginFacade* get_secure_dialogues_plugin_facade() {
  return &facade;
}

const char* getPluginId() {
  return "3c6594e0-972f-456b-9eea-ef8fcb8f289c";
}
