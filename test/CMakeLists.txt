set(BINARY ${CMAKE_PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.h *.cpp)
message(STATUS "Test Sources: ${TEST_SOURCES}")

set(SOURCES ${TEST_SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRC_ENABLE_GTEST=ON")

include_directories(${CMAKE_SOURCE_DIR}/lib/rapidcheck/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/rapidcheck/extras/gtest/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/googletest/googletest/include)

add_executable(${BINARY} ${TEST_SOURCES})
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib GTest::gtest rapidcheck rapidcheck_gtest)
add_test(NAME ${BINARY} COMMAND ${BINARY})





