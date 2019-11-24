#ifndef GUI_HELPERS_H
#define GUI_HELPERS_H

#include <QString>
#include <string_view>

inline QString make_qstring(std::string_view view) {
  return QString::fromUtf8(view.data(), static_cast<int>(view.size()));
}

#endif  // GUI_HELPERS_H
