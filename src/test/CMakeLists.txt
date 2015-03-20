include_directories(../include)

add_executable(test_config test_config.cc)
target_link_libraries(test_config tools)

add_executable(test_charset test_charset.cc)
target_link_libraries(test_charset tools)
