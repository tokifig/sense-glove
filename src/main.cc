#include <chrono>    // std::chrono
#include <iostream>  // std::cout
#include <thread>    // std::this_thread

#include "nova/nova_glove.h"

int main(int argc, char* argv[]) {
  nova::Glove left_glove(nova::GloveType::Left);
  nova::Glove right_glove(nova::GloveType::Right);
  std::cout << "LEFT: " << left_glove.Connect() << std::endl;
  std::cout << "RIGHT: " << right_glove.Connect() << std::endl;

  while (left_glove.is_connected() || right_glove.is_connected()) {
    if (left_glove.is_connected()) {
      const nova::SensorData data = left_glove.GetSensorData();
      std::cout << "LEFT: " << data << std::endl;
    }
    if (right_glove.is_connected()) {
      const nova::SensorData data = right_glove.GetSensorData();
      std::cout << "RIGHT: " << data << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
