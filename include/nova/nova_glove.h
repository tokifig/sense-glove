#include <array>     // std::array
#include <iostream>  // std::ostream
#include <memory>    // std::shared_ptr
#include <optional>  // std::optional

namespace SGCore {
namespace Nova {

class NovaGlove;
class Nova_SensorData;

}  // namespace Nova
}  // namespace SGCore

namespace nova {

enum class GloveType { Left, Right };

enum TendonIndex {
  ThumbFlexion = 0,
  ThumbAbduction = 1,
  IndexFlexion = 2,
  MiddleFlexion = 3,
  RingFlexion = 4
};

struct SensorData {
  std::array<float, 5> tendon_lengths;
  std::array<float, 4> quaternion;

  friend std::ostream& operator<<(std::ostream& os, const SensorData& data) {
    os << "NovaSensorData(thumb_flexion="
       << data.tendon_lengths[TendonIndex::ThumbFlexion] << ", thumb_abduction="
       << data.tendon_lengths[TendonIndex::ThumbAbduction]
       << ", index_flexion=" << data.tendon_lengths[TendonIndex::IndexFlexion]
       << ", middle_flexion=" << data.tendon_lengths[TendonIndex::MiddleFlexion]
       << ", ring_flexion=" << data.tendon_lengths[TendonIndex::RingFlexion]
       << ", quaternion=(" << data.quaternion[0] << "; " << data.quaternion[1]
       << ", " << data.quaternion[2] << ", " << data.quaternion[3] << "))";
    return os;
  }
};

class Glove {
 public:
  Glove(GloveType type, bool connect = true) : type_(type) {
    if (connect) Connect();
  }

  virtual ~Glove();

  bool is_connected() const { return bool(glove_); };

  float battery_level() const;

  bool Connect();

  SensorData GetSensorData();

  void SendHaptics(
      const std::optional<std::array<int, 4>>& force = std::nullopt,
      const std::optional<std::array<int, 2>>& buzz = std::nullopt,
      const std::optional<int>& palm_buzz = std::nullopt);

 private:
  GloveType type_;
  float battery_level_;
  std::shared_ptr<SGCore::Nova::NovaGlove> glove_;
  std::shared_ptr<SGCore::Nova::Nova_SensorData> sensor_data_;
};

}  // namespace nova
