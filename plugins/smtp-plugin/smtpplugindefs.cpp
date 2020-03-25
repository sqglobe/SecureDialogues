#include "smtpplugindefs.h"
#include "inner/plugininnerdefs.h"

#include "smtpfacade.h"

static SmtpFacade facade;

extern "C" PluginFacade* get_secure_dialogues_plugin_facade() {
  return &facade;
}

const char* getPluginId() {
  return "467ecc7a-66c9-11ea-9db3-2c56dc9baedc";
}
