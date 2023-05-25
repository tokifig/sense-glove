# Find SGCore library.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_library(LIB_SGCORE
        NAMES SGCoreCpp
        HINTS "${PROJECT_SOURCE_DIR}/Core/SGCoreCpp/lib/linux/Debug"
    )
    find_library(LIB_SGCONNECT
        NAMES SGConnect
        HINTS "${PROJECT_SOURCE_DIR}/Core/SGConnect/lib/linux/Debug"
    )
else()
    find_library(LIB_SGCORE
        NAMES SGCoreCpp
        HINTS "${PROJECT_SOURCE_DIR}/Core/SGCoreCpp/lib/linux/Release"
    )
    find_library(LIB_SGCONNECT
      NAMES SGConnect
        HINTS "${PROJECT_SOURCE_DIR}/Core/SGConnect/lib/linux/Release"
    )
endif()

# Build library.
set(LIB_NAME nova)
add_library(${LIB_NAME} SHARED)
add_library(${LIB_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME}
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
  PRIVATE
    ${PROJECT_SOURCE_DIR}/Core/SGCoreCpp/incl
    ${PROJECT_SOURCE_DIR}/Core/SGConnect/incl
)

target_link_libraries(${LIB_NAME}
  PRIVATE
    ${LIB_SGCORE}
    ${LIB_SGCONNECT}
)

target_sources(${LIB_NAME}
  PRIVATE
    nova_glove.cc
)

# Build Python wrapper.
add_subdirectory(python)

# Build example app.
add_executable(sense_glove)
target_link_libraries(sense_glove
  PRIVATE
    nova::nova
)
target_sources(sense_glove
  PRIVATE
    main.cc
)

# Build example connect.
# add_executable(sense_connect)
# target_include_directories(sense_connect
#   PRIVATE
#     ${PROJECT_SOURCE_DIR}/Core/SGCoreCpp/incl
#     ${PROJECT_SOURCE_DIR}/Core/SGConnect/incl
# )
# target_link_libraries(sense_connect
#   PRIVATE
#     ${LIB_SGCORE}
#     ${LIB_SGCONNECT}
# )
# target_sources(sense_connect
#   PRIVATE
#     ../Core/SGConnect/examples/SGConnect_TestProgram.cpp
# )

add_executable(sense_core)
target_include_directories(sense_core
  PRIVATE
    ${PROJECT_SOURCE_DIR}/Core/SGCoreCpp/incl
    ${PROJECT_SOURCE_DIR}/Core/SGConnect/incl
)
target_link_libraries(sense_core
  PRIVATE
    ${LIB_SGCORE}
    ${LIB_SGCONNECT}
)
target_sources(sense_core
  PRIVATE
  ../Core/SGCoreCpp/examples/StandaloneCpp/main.cpp
)
