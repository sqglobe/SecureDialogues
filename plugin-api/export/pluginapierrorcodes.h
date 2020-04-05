#ifndef PLUGINAPIERRORCODES_H
#define PLUGINAPIERRORCODES_H

#include "export-definition.h"

enum class EXPORTED PluginApiErrorCodes {
  NoError,
  Disconected,
  NotAuthorized,
  SendFailed,
  RecieveFailed,
  InvalidAddress
};

#endif  // PLUGINAPIERRORCODES_H
