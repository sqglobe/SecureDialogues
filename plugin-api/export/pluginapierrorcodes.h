#ifndef PLUGINAPIERRORCODES_H
#define PLUGINAPIERRORCODES_H

#include <QtGlobal>

enum class Q_DECL_EXPORT PluginApiErrorCodes {
  NoError,
  Disconected,
  NotAuthorized,
  SendFailed,
  RecieveFailed,
  InvalidAddress
};

#endif  // PLUGINAPIERRORCODES_H
