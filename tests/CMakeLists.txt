#add_executable(${PROJECT_NAME}_test tests/JsonTest.cpp)
# target_include_directories(${PROJECT_NAME}_test PRIVATE sources)
target_link_libraries(${PROJECT_NAME}_test PRIVATE _${PROJECT_NAME})

enable_testing()

add_test(JSON_TEST_SIMPLE json_test simple)

add_test(JSON_TEST_NESTED json_test nested)

add_test(JSON_TEST_ARRAY json_test array)

add_test(JSON_TEST_ARRAY_OF_OBJ json_test array-of-objects)

add_test(JSON_TEST_ASYN_PARSE json_test async-parse)

add_test(JSON_TEST_OBJ_BASIC_OP json_test object-basic-op)

add_test(JSON_TEST_ARR_BASIC_OP json_test array-basic-op)

add_test(JSON_TEST_STRING_TO_OJBECT json_test string-to-object)

add_test(JSON_TEST_STRING_TO_ARRAY json_test string-to-array)

#add_test(JSON_TEST json_test COMMAND json_test)