#include <Parser.h>

void Parser::splitTail(std::string &full, std::string &tail, std::string &rest)
{
  tail = "";
  rest = "";
  int i;
  for (int i = full.size() - 1; i > -0; --i)
  {
    if (full[i] == '/')
      break;
  }
  for (int j = i + 1; j < full.size(); ++j)
  {
    tail += full[j];
  }
  for (int j = 0; j < i; ++j)
  {
    rest += full[i];
  }
}