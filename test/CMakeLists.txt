find_package(GTest REQUIRED HINT /usr/local)

add_executable(mvtests main.cpp)

target_sources(mvtests 
               PRIVATE  
               test_utils.cpp
               test_meta.cpp
               test_metric.cpp
               test_mv.cpp
               test_mv_mixins.cpp
               test_nlist.cpp
               test_bitset.cpp
               test_be.cpp)

target_link_libraries(mvtests mv GTest::Main cmtestutils)

gtest_discover_tests(mvtests)

set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)

