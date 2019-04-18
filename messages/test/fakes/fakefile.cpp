#include "fakefile.h"

FakeFile::FakeFile(const std::string& readline) : mReadline(readline) {}

std::string getline(FakeFile& fileobject) {
  auto retval = fileobject.mReadline;
  fileobject.mReadline.clear();
  return retval;
}

bool at_end(FakeFile& fileObject) {
  return fileObject.mReadline.empty();
}

void writeline(FakeFile& fileObject, const std::string& str) {
  fileObject.mwritelines.push_back(str);
}
