#ifndef PLUGININNERDEFS_H
#define PLUGININNERDEFS_H

#include <QtGlobal>

class PluginFacade;

extern "C" Q_DECL_EXPORT PluginFacade* get_secure_dialogues_plugin_facade();

#endif  // PLUGININNERDEFS_H
