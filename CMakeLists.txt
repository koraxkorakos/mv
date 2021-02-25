project(mv LANGUAGES CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

#find_package(Boost, REQUIRED program_options system filesystem exception)
#find_package(Eigen3 REQUIRED NO_MODULE)

add_subdirectory(testutils)
add_library(mv)

target_sources(mv
               PRIVATE
               src/bitset.cpp
               src/expression.cpp
               src/meta.cpp
               src/mv.cpp
               src/mv_mixins.cpp
               src/nlist.cpp
               src/rational_constant.cpp
               src/tlist.cpp)


target_include_directories(mv
                           PUBLIC 
                           $<INSTALL_INTERFACE:include>    
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/src)
               
set_property(TARGET mv
             PROPERTY PUBLIC_HEADER
             include/mv/expression.hpp
             include/mv/meta.hpp
             include/mv/metric.hpp
             include/mv/mv.hpp
             include/mv/mv_mixins.hpp
             include/mv/nlist.hpp
             include/mv/rational_constant.hpp
             include/mv/tlist.hpp)                           

target_link_libraries(mv
                      PUBLIC)

install(TARGETS mv 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

option(BUILD_TESTS "Build unit tests" On)        
if(${BUILD_TESTS})
   enable_testing()
   add_subdirectory("test")
endif()