#include <iostream>

namespace Parser
{
  struct Triple
  {
    int a;
    int b;
    int c;
  };
  void splitTail(std::string &full, std::string &tail, std::string &rest);
  void clearSpace(std::string &buffer, int &i);
  std::string getNextToken(std::string &buffer, int &i);
  float getNextFloat(std::string &buffer, int &i);
  int getNextInt(std::string &buffer, int &i);
  Triple getNextTriple(std::string &buffer, int &i);
}