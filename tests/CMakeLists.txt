add_executable(${PROJECT_NAME}_test tests/JSONTest.cpp)
target_include_directories(${PROJECT_NAME}_test PRIVATE include)
target_link_libraries(${PROJECT_NAME}_test PRIVATE _${PROJECT_NAME}-${PROJECT_VERSION})

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

add_executable(${PROJECT_NAME}_variant_test tests/VariantTest.cpp)
target_include_directories(${PROJECT_NAME}_variant_test PRIVATE include)
target_link_libraries(${PROJECT_NAME}_variant_test PRIVATE _${PROJECT_NAME}-${PROJECT_VERSION})
