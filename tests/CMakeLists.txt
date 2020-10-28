
include_directories(PUBLIC ${CMAKE_SOURCE_DIR})


add_executable(test_read_file test_read_file.c ../json.c)

add_test(NAME test_read_file COMMAND test_read_file)

target_link_libraries(test_read_file PRIVATE json-c)

configure_file(test_read_file.json test_read_file.json COPYONLY)


add_executable(test_extract_price_data test_extract_price_data.c ../json.c)

add_test(NAME test_extract_price_data COMMAND test_extract_price_data)

target_link_libraries(test_extract_price_data PRIVATE json-c)

configure_file(test_extract_price_data.json test_extract_price_data.json COPYONLY)


add_executable(test_string_to_double test_string_to_double.c ../json.c)

add_test(NAME test_string_to_double COMMAND test_string_to_double)

target_link_libraries(test_string_to_double PRIVATE json-c)
