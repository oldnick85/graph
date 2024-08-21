enable_testing()
find_package(GTest REQUIRED)

add_executable(unittest unittest.cpp)
target_link_libraries(unittest 
                    PRIVATE graph
                    GTest::gtest GTest::gtest_main)
