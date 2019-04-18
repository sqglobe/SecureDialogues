#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <fstream>
#include <iostream>
#include <sstream>

std::string getline(std::fstream& fileobject);
bool at_end(std::fstream& fileObject);
void writeline(std::fstream& fileObject, const std::string& line);

std::string getline(std::stringstream& fileobject);
bool at_end(std::stringstream& fileObject);
void writeline(std::stringstream& fileObject, const std::string& line);

#endif  // FILEUTILS_H
