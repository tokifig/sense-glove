project(novapy)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}")

# Find pybind11.
add_subdirectory("${CMAKE_SOURCE_DIR}/external/pybind11" "${CMAKE_BINARY_DIR}/external" EXCLUDE_FROM_ALL)


# Build Python wrapper.
pybind11_add_module(novapy novapy.cc)

target_link_libraries(novapy
  PUBLIC
    pybind11::pybind11
    nova::nova
)

