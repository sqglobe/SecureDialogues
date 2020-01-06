#include "gmailplugindefs.h"
#include "inner/plugininnerdefs.h"

#include "gmailfacade.h"

static GmailFacade facade;

extern "C" PlaginFacade* get_facade() {
  return &facade;
}

const char* getPluginId() {
  return "3ccacfb5-b8be-42da-a9c8-3098e68dd76d";
}
