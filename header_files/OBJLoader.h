#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <iostream>
#include "Model.h"
namespace BERender
{
  void loadOBJ(std::string path, Model& model);
};
#endif