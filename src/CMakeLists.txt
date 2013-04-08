set(OPENSPACE_DIR /home/vsand/OpenSpace)

include_directories(${FurnaceApp_SOURCE_DIR}/inc)
include_directories(${OPENSPACE_DIR}/kameleon_plus-0.3.11/src)

link_directories(${OPENSPACE_DIR}/kameleon_plus-0.3.11/src/ccmc/.libs)
link_directories(${OPENSPACE_DIR}/cdf34_1-dist/lib)

add_executable(FurnaceApp FurnaceApp.cpp VDFData.cpp CDFReader.cpp ENLILReader.cpp VDFWriter.cpp Furnace.cpp)

target_link_libraries(FurnaceApp libcdf.so)
target_link_libraries(FurnaceApp libkameleon_plus-0.3.11.so)
