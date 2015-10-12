cmake_minimum_required(VERSION 2.8)

#Variable definition
set( EXECNAME concu)



#Project name
project(${EXECNAME})




#Compiler setting-------------------------------------------------------------------------------------------

#Compiler flags config

#WARNING: as default, it will compile .c files with gcc and .cpp files with g++.The following flags are just for g++ compiler, if needed .c files can be configured to be compiled with g++ compiler as so: set_source_files_properties(filename.c PROPERTIES LANGUAGE CXX ).

if(CMAKE_COMPILER_IS_GNUCXX)
	#Compiler flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -g")


	#Include directtarget_link_libraries(${TESTNAME} pthread libgtest)ories
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

	include_directories(include)

endif()



#Libs and source files settings-------------------------------------------------------------------------------------------


#Object files
file(GLOB OSRCS "${CMAKE_CURRENT_LIST_DIR}/*.[cph]*")
list(REMOVE_ITEM OSRCS ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
add_library(objectFiles OBJECT ${OSRCS})


#Exec
add_executable(${EXECNAME} main.cpp $<TARGET_OBJECTS:objectFiles>)











