
add_library(cmtestutils)
target_sources(cmtestutils 
               PRIVATE 
               src/demangle.cpp)
set_property(TARGET cmtestutils 
             PROPERTY PUBLIC_HEADER 
             include/testutils/demangle.hpp)  

target_include_directories(cmtestutils PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)                   

install(TARGETS cmtestutils DESTINATION lib)
install(FILES 
        include/testutils/demangle.hpp             
        DESTINATION include/testutils)                      
                           