#ifndef BASE64_H
#define BASE64_H
#include <string>
std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& encoded_string);
#endif  // BASE64_H
