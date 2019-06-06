#ifndef FAKEFILE_H
#define FAKEFILE_H

/**
 *  Needed functions:
 *  std::string getline(F &fileobject);
 *  bool at_end(F &fileObject);
 *  void writeline(F &fileObject, const std::string &)
 */

#include <string>
#include <vector>
class FakeFile {
 public:
  FakeFile(std::string readline);

 public:
  std::string mReadline;
  std::vector<std::string> mwritelines;
};

std::string getline(FakeFile& fileobject);
bool at_end(FakeFile& fileObject);
void writeline(FakeFile& fileObject, const std::string& str);

#endif  // FAKEFILE_H
