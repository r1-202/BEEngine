#include <Parser.h>

void Parser::splitTail(std::string &full, std::string &tail, std::string &rest)
{
  tail = "";
  rest = "";
  int i;
  for (int i = full.size() - 1; i >= 0; --i)
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

void Parser::clearSpace(std::string &buffer, int &i)
{
  while (buffer[i] == ' ' && i < buffer.size())
    ++i;
}

std::string Parser::getNextToken(std::string &buffer, int &i)
{
  clearSpace(buffer, i);
  std::string token = "";
  while (buffer[i] != ' ' && i < buffer.size())
  {
    token += buffer[i];
    ++i;
  }
  return token;
}

float Parser::getNextFloat(std::string &buffer, int &i)
{
  clearSpace(buffer, i);
  std::string token = getNextToken(buffer, i);
  return std::stof(token);
}

int Parser::getNextInt(std::string &buffer, int &i)
{
  clearSpace(buffer, i);
  std::string token = getNextToken(buffer, i);
  return std::stoi(token);
}