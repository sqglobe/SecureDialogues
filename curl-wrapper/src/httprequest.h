#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>

struct HttpHeader {
  std::string name;
  std::string value;
};

class HttpRequest {
 public:
  std::string get(const std::string& uri) const;
  std::string get(const std::string& uri,
                  std::initializer_list<HttpHeader> headers) const;

  std::string post(const std::string& uri, const std::string& body) const;
  std::string post(const std::string& uri,
                   const std::string& body,
                   std::initializer_list<HttpHeader> headers);
};

#endif  // HTTPREQUEST_H
