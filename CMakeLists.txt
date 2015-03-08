CMAKE_MINIMUM_REQUIRED( VERSION 3.1.2 )

get_filename_component(SOURCE_DIRECTORY_NAME ${CMAKE_SOURCE_DIR} NAME)



# Section for project specific settings

set( PROJ_NAME "python_boost" )

set( BOOST_PATH F:/_compilers/_lib/boost_1_57_0 )
set( PYTHON_PATH F:/_compilers/python_3.4.3_x64 )

set ( PYTHON_INCLUDES ${PYTHON_PATH}/include )
set ( PYTHON_LIBRARIES ${PYTHON_PATH}/python34 )
#set ( BOOST_INCLUDES ${BOOST_PATH}/include )
#set ( BOOST_LIBRARIES  ${BOOST_PATH}/lib/libboost_system-mgw49-1_57.a ${BOOST_PATH}/lib/libboost_python-mgw49-1_57.a )
#file( GLOB_RECURSE BOOST_LIBRARIES ${BOOST_PATH}/lib/*.a )

link_directories ( ${PYTHON_PATH} )
set( PROJ_LIBRARIES "ws2_32" "wsock32" "python34" ${BOOST_LIBRARIES} )
set( PROJ_INCLUDES ${PYTHON_INCLUDES} ${BOOST_INCLUDES} lib src )
file( GLOB_RECURSE PROJ_SOURCES *.cpp lib/serial/*.cpp lib/serial/*.c )

# End of special section


set( CMAKE_CXX_FLAGS "-std=c++11" )
if ( UNIX )
    set( CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}" )
endif ( UNIX )


set(CMAKE_SHARED_LINKER_FLAGS "-static")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../___bin___/${SOURCE_DIRECTORY_NAME}___${CMAKE_BUILD_TYPE}/" )
project( ${PROJ_NAME} )
include_directories( ${PROJ_INCLUDES} )
add_executable( ${PROJ_NAME} ${PROJ_SOURCES} )
target_link_libraries( ${PROJ_NAME} ${PROJ_LIBRARIES} )