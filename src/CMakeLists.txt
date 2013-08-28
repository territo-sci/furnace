set(OPENSPACE_DIR /media/snabba_disk/OpenSpace)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(Boost_INCLUDE_DIR ${OPENSPACE_DIR}/common_external/boost_1_53_0)
set(Boost_LIBRARY_DIR ${OPENSPACE_DIR}/common_external/boost_1_53_0/stage/lib)
find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${FurnaceApp_SOURCE_DIR}/inc)
include_directories(${OPENSPACE_DIR}/kameleon_plus-0.3.11/src)

link_directories(${Boost_LIBRARY_DIR})
link_directories(${OPENSPACE_DIR}/kameleon_plus-0.3.11/src/ccmc/.libs)
link_directories(${OPENSPACE_DIR}/cdf34_1-dist/lib)

add_executable(FurnaceApp 
	       FurnaceApp.cpp
	       ENLILProcessor.cpp 
	       MLProcessor.cpp
	       VolumeProcessor.cpp 
	       Furnace.cpp)

target_link_libraries(FurnaceApp ${Boost_LIBRARIES})
target_link_libraries(FurnaceApp libcdf.so)
target_link_libraries(FurnaceApp libkameleon_plus-0.3.11.so)
