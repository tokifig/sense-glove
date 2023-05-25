#include <nova/nova_glove.h>
#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <sstream>  // std::stringstream

namespace nova {

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(novapy, m) {
  py::enum_<GloveType>(m, "GloveType")
      .value("Left", GloveType::Left)
      .value("Right", GloveType::Right);

  py::enum_<TendonIndex>(m, "TendonIndex")
      .value("ThumbFlexion", TendonIndex::ThumbFlexion)
      .value("ThumbAbduction", TendonIndex::ThumbAbduction)
      .value("IndexFlexion", TendonIndex::IndexFlexion)
      .value("MiddleFlexion", TendonIndex::MiddleFlexion)
      .value("RingFlexion", TendonIndex::RingFlexion)
      .export_values();

  py::class_<SensorData>(m, "SensorData")
      .def_readwrite("tendon_lengths", &SensorData::tendon_lengths)
      .def_readwrite("quaternion", &SensorData::quaternion)
      .def("__repr__", [](const SensorData& data) {
        std::stringstream ss;
        ss << data;
        return ss.str();
      });

  py::class_<Glove>(m, "Glove")
      .def(py::init<GloveType, bool>(), "type"_a, "connect"_a = true)
      .def_property_readonly("is_connected", &Glove::is_connected)
      .def_property_readonly("battery_level", &Glove::battery_level)
      .def("connect", &Glove::Connect,
           py::call_guard<py::gil_scoped_release, py::scoped_ostream_redirect,
                          py::scoped_estream_redirect>())
      .def("get_sensor_data", &Glove::GetSensorData,
           py::call_guard<py::gil_scoped_release>())
      .def("send_haptics", &Glove::SendHaptics,
           py::call_guard<py::gil_scoped_release>(), "force"_a = std::nullopt,
           "buzz"_a = std::nullopt, "palm_buzz"_a = std::nullopt);
};

}  // namespace nova
