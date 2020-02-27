#ifndef PLUGINSCONTAINER_H
#define PLUGINSCONTAINER_H

#include <map>
#include <memory>

template <typename C>
class map_value_iterator
    : public std::iterator<std::forward_iterator_tag,
                           typename C::mapped_type,
                           ptrdiff_t,
                           typename C::mapped_type*,
                           const typename C::mapped_type&> {
 public:
  map_value_iterator(typename C::const_iterator iter) :
      mIter(std::move(iter)) {}

 public:
  bool operator!=(map_value_iterator const& other) const {
    return this->mIter != other.mIter;
  }
  bool operator==(map_value_iterator const& other) const {
    return this->mIter == other.mIter;
  }
  typename map_value_iterator::reference operator*() const {
    return this->mIter->second;
  }
  map_value_iterator& operator++() {
    ++mIter;
    return *this;
  }

 private:
  typename C::const_iterator mIter;
};
namespace plugin_support {
class PluginInterface;
class PluginsContainer {
 public:
  using const_iterator = map_value_iterator<
      std::map<std::string, std::shared_ptr<PluginInterface>>>;

 public:
  void add(std::shared_ptr<PluginInterface> plugin);
  std::shared_ptr<PluginInterface> get(const std::string& id) const;

 public:
  const_iterator cbegin() const;
  const_iterator cend() const;

 private:
  std::map<std::string, std::shared_ptr<PluginInterface>> mPlugins;
};
}  // namespace plugin_support

#endif  // PLUGINSCONTAINER_H
