
include_directories(PUBLIC ${CMAKE_SOURCE_DIR})


add_executable(test_read_json test_read_json.c ../json.c)

add_test(NAME test_read_json COMMAND test_read_json)

target_link_libraries(test_read_json PRIVATE json-c)

configure_file(test_read_json.json test_read_json.json COPYONLY)


add_executable(test_extract_data test_extract_data.c ../json.c)

add_test(NAME test_extract_data COMMAND test_extract_data)

target_link_libraries(test_extract_data PRIVATE json-c)

configure_file(test_extract_data.json test_extract_data.json COPYONLY)


add_executable(test_extract_price test_extract_price.c ../json.c)

add_test(NAME test_extract_price COMMAND test_extract_price)

target_link_libraries(test_extract_price PRIVATE json-c)
