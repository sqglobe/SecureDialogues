#include "uribuilder.h"
#include <memory>
#include "curlexceptions.h"

using CurlHandler = std::unique_ptr<CURL, void (*)(CURL*)>;
static CurlHandler CURL_HANDLER(curl_easy_init(), curl_easy_cleanup);

void appendEscape(const std::string& param, std::string& out, CURL* curl) {
  if (char* output =
          curl_easy_escape(curl, param.c_str(), static_cast<int>(param.size()));
      !output) {
    throw CurlEscapeError("Failed eascape path");
  } else {
    out.append(output);
    curl_free(output);
  }
}

UriBuilder::UriBuilder(const std::string& path) : mPath(path) {}

UriBuilder::UriBuilder() {}

UriBuilder& UriBuilder::appendPath(const std::string& path) {
  if (!CURL_HANDLER) {
    throw CurlInitError("Failed init curl in UriBuilder");
  }
  mPath.append("/");
  appendEscape(path, mPath, CURL_HANDLER.get());
  return *this;
}

UriBuilder& UriBuilder::appendQuery(const std::string& name,
                                    const std::string& value) {
  if (!CURL_HANDLER) {
    throw CurlInitError("Failed init curl in UriBuilder");
  }
  std::string query;
  appendEscape(name, query, CURL_HANDLER.get());
  query.append("=");
  appendEscape(value, query, CURL_HANDLER.get());
  if (!mQuery.empty())
    mQuery.append("&");
  mQuery.append(query);
  return *this;
}

std::string UriBuilder::getQuery() const {
  return mQuery;
}

std::string UriBuilder::getPath() const {
  return mPath;
}

std::string UriBuilder::getUri() const {
  if (mPath.empty())
    return mQuery;

  std::string res(mPath);
  res.append("?").append(mQuery);
  return res;
}
