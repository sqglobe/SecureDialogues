#include "uribuilder.h"
#include "curlexceptions.h"

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

UriBuilder::UriBuilder(const std::string& path) :
    mPath(path), mCurl(curl_easy_init()) {
  if (!mCurl) {
    throw CurlInitError("Failed init curl in UriBuilder");
  }
}

UriBuilder::UriBuilder() : mCurl(curl_easy_init()) {}

UriBuilder::UriBuilder(const UriBuilder& other) :
    mPath(other.mPath), mQuery(other.mQuery), mCurl(curl_easy_init()) {
  if (!mCurl) {
    throw CurlInitError("Failed init curl in UriBuilder");
  }
}

UriBuilder& UriBuilder::operator=(const UriBuilder& other) {
  this->mPath = other.mPath;
  this->mQuery = other.mQuery;
  return *this;
}

UriBuilder::~UriBuilder() {
  if (mCurl) {
    curl_easy_cleanup(mCurl);
  }
}

UriBuilder& UriBuilder::appendPath(const std::string& path) {
  mPath.append("/");
  appendEscape(path, mPath, mCurl);
  return *this;
}

UriBuilder& UriBuilder::appendQuery(const std::string& name,
                                    const std::string& value) {
  std::string query;
  appendEscape(name, query, mCurl);
  query.append("=");
  appendEscape(value, query, mCurl);
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
