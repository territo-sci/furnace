set(OPENSPACE_DIR CACHE PATH "Path to OpenSpace code")

set(KAMELEON_DIR ${OPENSPACE_DIR}/modules/kameleon/ext/kameleon)
set(CDF_DIR ${KAMELEON_DIR}/ext/cdf)
set(CDF_VERSION 35_0 CACHE STRING "Version number of CDF library (form 'major_minor')")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${FurnaceApp_SOURCE_DIR}/inc)
include_directories(${KAMELEON_DIR}/src)

link_directories(${Boost_LIBRARY_DIR})
link_directories(${KAMELEON_DIR}/src/ccmc/.libs)
link_directories(${CDF_DIR}/cdf${CDF_VERSION}-dist/src/include)

add_executable(FurnaceApp 
	       FurnaceApp.cpp
	       ENLILProcessor.cpp 
	       MLProcessor.cpp
	       VolumeProcessor.cpp 
	       Furnace.cpp)

target_link_libraries(FurnaceApp ${Boost_LIBRARIES})
target_link_libraries(FurnaceApp libcdf.so)
target_link_libraries(FurnaceApp libkameleon_plus.so)
