#ifndef CONNECTIONHOLDER_H
#define CONNECTIONHOLDER_H

#include <memory>
#include <string>

class PluginConnectionInfo;
class ConnectionHolder {
 public:
  ConnectionHolder(const std::string& connName,
                   std::shared_ptr<PluginConnectionInfo> pluginConnInfo);
  ConnectionHolder(const std::string& connName);
  ConnectionHolder() = default;

 public:
  std::string connName() const;
  void setConnName(const std::string& connName);

  std::shared_ptr<PluginConnectionInfo> pluginConnInfo() const;
  void setPluginConnInfo(
      const std::shared_ptr<PluginConnectionInfo>& pluginConnInfo);

 private:
  std::string mConnName;
  std::shared_ptr<PluginConnectionInfo> mPluginConnInfo;
};
#endif  // CONNECTIONHOLDER_H
