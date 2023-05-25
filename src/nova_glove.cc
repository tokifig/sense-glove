#include "nova/nova_glove.h"

#include <array>     // std::array
#include <iostream>  // std::ostream
#include <memory>  // std::dynamic_pointer_cast, std::make_unique, std::shared_ptr

#include "NovaGlove.h"  // SGCore::Nova
#include "SGConnect.h"

namespace nova {

Glove::~Glove() {
  if (!glove_) return;
  glove_->SendHaptics(SGCore::Haptics::SG_FFBCmd::off,
                      SGCore::Haptics::SG_BuzzCmd::off,
                      SGCore::Haptics::ThumperCmd::off);
  // SGConnect::Dispose();
}

float Glove::battery_level() const {
  if (!sensor_data_) {
    throw std::runtime_error("Glove not connected.");
  }
  return sensor_data_->batteryLevel;
}

bool Glove::Connect() {
  // Connect to device.
  std::shared_ptr<SGCore::HapticGlove> haptic_glove;
  const int num_active = SGConnect::ActiveDevices();
  std::cout << "num_active " << num_active << std::endl;
  if (num_active <= 0) {
    SGConnect::SetDebugLevel(0);
    int init_code = SGConnect::Init();
    std::cout << "init_code " << init_code << std::endl;
    std::cout << SGConnect::GetConnectionStates() << std::endl;
  }
  const bool is_connected =
      SGCore::HapticGlove::GetGlove(type_ == GloveType::Right, haptic_glove);

  // Cast to Nova glove.
  glove_ = std::dynamic_pointer_cast<SGCore::Nova::NovaGlove>(haptic_glove);
  if (!glove_) return false;

  // Populate sensor data for the first time.
  if (!sensor_data_) {
    sensor_data_ = std::make_shared<SGCore::Nova::Nova_SensorData>();
  }
  glove_->GetSensorData(*sensor_data_);

  return is_connected;
}

SensorData Glove::GetSensorData() {
  if (!glove_ || !sensor_data_) {
    throw std::runtime_error("Glove not connected.");
  }
  glove_->GetSensorData(*sensor_data_);

  // Extract only non-zero values from the array.
  const std::vector<SGCore::Kinematics::Vect3D>& sensor_values =
      sensor_data_->sensorValues;
  const SGCore::Kinematics::Quat& orientation = sensor_data_->IMURotation;
  return SensorData{
      {sensor_values[0].y, sensor_values[0].z, sensor_values[1].y,
       sensor_values[2].y, sensor_values[3].y},
      {orientation.w, orientation.x, orientation.y, orientation.z}};
}

namespace {

SGCore::Haptics::SG_FFBCmd CreateForceFeedback(
    const std::optional<std::array<int, 4>>& force) {
  if (!force) {
    return SGCore::Haptics::SG_FFBCmd::off;
  }

  const std::array<int, 4>& f = *force;
  return SGCore::Haptics::SG_FFBCmd(f[0], f[1], f[2], f[3], 0);
}

SGCore::Haptics::SG_BuzzCmd CreateBuzz(
    const std::optional<std::array<int, 2>>& buzz) {
  if (!buzz) {
    return SGCore::Haptics::SG_BuzzCmd::off;
  }

  const std::array<int, 2>& b = *buzz;
  return SGCore::Haptics::SG_BuzzCmd(b[0], b[1], 0, 0, 0);
}

SGCore::Haptics::ThumperCmd CreatePalmBuzz(
    const std::optional<int>& palm_buzz) {
  if (!palm_buzz) {
    return SGCore::Haptics::ThumperCmd::off;
  }

  return SGCore::Haptics::ThumperCmd(*palm_buzz);
}

}  // namespace

void Glove::SendHaptics(const std::optional<std::array<int, 4>>& force,
                        const std::optional<std::array<int, 2>>& buzz,
                        const std::optional<int>& palm_buzz) {
  SGCore::Haptics::SG_FFBCmd force_feedback = CreateForceFeedback(force);

  if (!buzz && !palm_buzz) {
    glove_->SendHaptics(force_feedback);
  } else {
    glove_->SendHaptics(force_feedback, CreateBuzz(buzz),
                        CreatePalmBuzz(palm_buzz));
  }
}

}  // namespace nova
