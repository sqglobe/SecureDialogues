#include "httprequest.h"
#include <curl/curl.h>
#include <functional>
#include <memory>
#include "curlexceptions.h"

using CurlHandler = std::unique_ptr<CURL, void (*)(CURL*)>;
using HeaderHandler = std::unique_ptr<curl_slist, void (*)(curl_slist*)>;

std::tuple<HttpCode, std::string> do_request(const std::string& uri,
                                             const CurlHandler& handler) {
  curl_easy_setopt(handler.get(), CURLOPT_URL, uri.c_str());
  std::string output;
  output.reserve(100);
  curl_easy_setopt(handler.get(), CURLOPT_WRITEDATA, &output);
  if (CURLcode res = curl_easy_perform(handler.get()); res != CURLE_OK) {
    throw CurlHttpSendError(curl_easy_strerror(res));
  }
  auto response_code{0L};
  curl_easy_getinfo(handler.get(), CURLINFO_RESPONSE_CODE, &response_code);
  return std::make_tuple(static_cast<HttpCode>(response_code),
                         std::move(output));
}

void bind_headers(HeaderHandler& handler,
                  const std::vector<HttpHeader>& headers) {
  for (const auto& [name, value] : headers) {
    auto header = name;
    header.append(":").append(value);
    if (auto tmp = curl_slist_append(handler.get(), header.c_str()); tmp) {
      handler.release();
      handler.reset(tmp);
    } else {
      throw CurlHttpHeaderError("Failed to append header to http request");
    }
  }
}

size_t read_curl_data(char* contents,
                      size_t size,
                      size_t nmemb,
                      std::string* s) {
  size_t newLength = size * nmemb;
  s->append(contents, newLength);
  return newLength;
}

CurlHandler make_curl() {
  CURL* curl = curl_easy_init();
  if (!curl) {
    throw CurlInitError("Failed init curl in HttpRequest");
  }
  CurlHandler ptr(curl, curl_easy_cleanup);
  curl_easy_setopt(ptr.get(), CURLOPT_SSL_VERIFYPEER, 0L);  // only for https
  curl_easy_setopt(ptr.get(), CURLOPT_SSL_VERIFYHOST, 0L);  // only for https
  curl_easy_setopt(ptr.get(), CURLOPT_WRITEFUNCTION, read_curl_data);
  curl_easy_setopt(ptr.get(), CURLOPT_VERBOSE,
                   0L);  // remove this to disable verbose output
  return ptr;
}

std::tuple<HttpCode, std::string> HttpRequest::get(
    const std::string& uri) const {
  auto curl = make_curl();
  curl_easy_setopt(curl.get(), CURLOPT_HTTPGET, 1);
  return do_request(uri, curl);
}

std::tuple<HttpCode, std::string> HttpRequest::get(
    const std::string& uri,
    std::initializer_list<HttpHeader> headers) const {
  auto curl = make_curl();
  curl_easy_setopt(curl.get(), CURLOPT_HTTPGET, 1);
  HeaderHandler headersHandler(nullptr, curl_slist_free_all);
  bind_headers(headersHandler, headers);
  curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, headersHandler.get());
  return do_request(uri, curl);
}

std::tuple<HttpCode, std::string> HttpRequest::post(
    const std::string& uri,
    const std::string& body) const {
  auto curl = make_curl();
  curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, body.c_str());
  return do_request(uri, curl);
}

std::tuple<HttpCode, std::string> HttpRequest::post(
    const std::string& uri,
    const std::string& body,
    std::initializer_list<HttpHeader> headers) {
  auto curl = make_curl();
  curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, body.c_str());

  HeaderHandler headersHandler(nullptr, curl_slist_free_all);
  bind_headers(headersHandler, headers);
  curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, headersHandler.get());

  return do_request(uri, curl);
}
