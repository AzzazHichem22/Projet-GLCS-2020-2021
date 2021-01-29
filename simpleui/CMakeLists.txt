cmake_minimum_required(VERSION 3.18)

project(ProjectName)
find_package(HDF5 REQUIRED COMPONENTS C)
set(HDF5_PREFER_PARALLEL TRUE)


add_library(simpleui
		src/commandlineconfig.cpp
		src/screenprinter.cpp
)
add_library(GLCS2020_project::simpleui ALIAS simpleui)
target_include_directories(simpleui PUBLIC include/)
target_link_libraries(simpleui GLCS2020_project::baselib hdf5::hdf5)

