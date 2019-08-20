#include "fakefile.h"

FakeFile::FakeFile(std::string readline) : mReadline(std::move(readline)) {}

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
