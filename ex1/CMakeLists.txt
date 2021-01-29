cmake_minimum_required(VERSION 3.19)

 project(HDF5LIB)

find_package(MPI REQUIRED COMPONENTS CXX)
set(HDF5_PREFER_PARALLEL TRUE)
find_package(HDF5 REQUIRED COMPONENTS C)
 add_library(GLCS2020_project::ex1 ALIAS ex1)
 
target_link_libraries(ex1
GLCS2020_project::baselib
hdf5::hdf5 MPI::MPI_CXX
 )

 target_include_directories(ex1 PUBLIC include/)
 add_library(ex1
		src/commandlineconfig.cpp
		src/screenprinter.cpp
)


