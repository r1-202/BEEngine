#include <Camera.h>
#include <iostream>
int main()
{
  BERender::Camera camera = BERender::Camera();
  float prev = time(0);
  while(true)
  {
    camera.processKeyboard(BERender::Direction::FORWARD,time(0)-prev);
    _sleep(100);
    prev = time(0);
    std::cout<<camera.position.x<<' '<<camera.position.y<<' '<<camera.position.z<<'\n';
  }
}