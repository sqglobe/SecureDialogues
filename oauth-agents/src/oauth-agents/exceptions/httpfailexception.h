#ifndef HTTPFAILEXCEPTION_H
#define HTTPFAILEXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

class HttpFailException : public std::exception {
 public:
  HttpFailException(unsigned short code,
                    const std::string& url,
                    const std::string& body) :
      mCode(code),
      mUrl(url), mBody(body) {
    std::stringstream ss;
    ss << "Failed send request to url " << url << " with code " << code;
    mMessage = ss.str();
  }

 public:
  virtual const char* what() const noexcept override {
    return mMessage.c_str();
  }
  unsigned short code() const noexcept { return mCode; }
  std::string url() const { return mUrl; }
  std::string body() const { return mBody; }

 private:
  std::string mMessage;
  unsigned short mCode;
  std::string mUrl;
  std::string mBody;
};

#endif  // HTTPFAILEXCEPTION_H
