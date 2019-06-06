#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>

template <typename Out>
void split(const std::string& s, char delim, Out result) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

template <typename Out>
void splitToViews(const std::string& s, char delim, Out result) {
  std::string_view main_view(s);

  for (auto pos = main_view.find(delim);; pos = main_view.find(delim)) {
    *(result++) = main_view.substr(0, pos);
    if (pos == std::string_view::npos) {
      break;
    }
    main_view.remove_prefix(pos + 1);
  }
}

std::string make_uiid();

long get_timestamp();

#endif  // UTILS_H
