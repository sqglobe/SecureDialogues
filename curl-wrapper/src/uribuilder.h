#ifndef URIBUILDER_H
#define URIBUILDER_H

#include <curl/curl.h>
#include <string>

class UriBuilder {
 public:
  explicit UriBuilder(const std::string& path);
  UriBuilder(const UriBuilder& other);
  UriBuilder& operator=(const UriBuilder& other);
  ~UriBuilder();

 public:
  UriBuilder& appendPath(const std::string& path);
  UriBuilder& appendQuery(const std::string& name, const std::string& values);
  std::string getQuery() const;
  std::string getPath() const;
  std::string getUri() const;

 private:
  std::string mPath;
  std::string mQuery;
  CURL* mCurl;
};

#endif  // URIBUILDER_H
