if(NOT TARGET pybind11::pybind11)
    init_git_submodule(pybind11.git)
    add_subdirectory(pybind11.git)
endif()
