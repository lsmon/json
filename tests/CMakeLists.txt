add_executable(${BIN_NAME}_test tests/JSONTest.cpp)
target_include_directories(${BIN_NAME}_test PRIVATE include)
target_link_libraries(${BIN_NAME}_test PRIVATE ${LIB_NAME} pthread)

enable_testing()

add_test(JSON_TEST_SIMPLE ${BIN_NAME}_test simple)

add_test(JSON_TEST_NESTED ${BIN_NAME}_test nested)

add_test(JSON_TEST_ARRAY ${BIN_NAME}_test array)

add_test(JSON_TEST_ARRAY_OF_OBJ ${BIN_NAME}_test array-of-objects)

add_test(JSON_TEST_ASYNC_PARSE ${BIN_NAME}_test async-parse)

add_test(JSON_TEST_OBJ_BASIC_OP ${BIN_NAME}_test object-basic-op)

add_test(JSON_TEST_ARR_BASIC_OP ${BIN_NAME}_test array-basic-op)

add_test(JSON_TEST_STRING_TO_OBJECT ${BIN_NAME}_test string-to-object)

add_test(JSON_TEST_STRING_TO_ARRAY ${BIN_NAME}_test string-to-array)

