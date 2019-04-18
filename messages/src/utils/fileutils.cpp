#include "fileutils.h"

std::string getline(std::fstream& fileobject) {
  std::string line;
  std::getline(fileobject, line);
  return line;
}

bool at_end(std::fstream& fileObject) {
  return !fileObject || fileObject.eof();
}

void writeline(std::fstream& fileObject, const std::string& line) {
  fileObject << line << "\n";
}

std::string getline(std::stringstream& fileobject) {
  std::string line;
  std::getline(fileobject, line);
  return line;
}

bool at_end(std::stringstream& fileObject) {
  return !fileObject || fileObject.eof();
}

void writeline(std::stringstream& fileObject, const std::string& line) {
  fileObject << line << "\n";
}
