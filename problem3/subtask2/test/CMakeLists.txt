project(MallTest)

set(SOURCES run_test.cpp mall_tests.cpp)

add_executable(mall_test ${SOURCES})

target_link_libraries(mall_test mall_lib gtest gmock)