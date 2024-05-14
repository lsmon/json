#[[
MIT License

Copyright (c) 2023 Salomon Lee

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
]]
cmake_minimum_required(VERSION 3.24)
project(json)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_TWEAK 1)
set(PROJECT_VERSION_PATCH 1)

set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_TWEAK}.${PROJECT_VERSION_PATCH})

set(INC
        include/JSONParser.hpp
        include/JSONObject.hpp
        include/JSONArray.hpp
        include/JSONValue.hpp
        include/JSONUtil.hpp
        )

set(SRC
        sources/JSONParser.cpp
        sources/JSONObject.cpp
        sources/JSONArray.cpp
        sources/JSONValue.cpp
        sources/JSONUtil.cpp
        sources/JSONValue.cpp)

include_directories(include)
include_directories(sources)

add_library(_${PROJECT_NAME}-${PROJECT_VERSION} ${SRC})

target_include_directories(_${PROJECT_NAME}-${PROJECT_VERSION} PUBLIC include)

add_executable(${PROJECT_NAME} main.cpp)
# target_include_directories(${PROJECT_NAME} PRIVATE sources)
target_link_libraries(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}-${PROJECT_VERSION})


add_executable(${PROJECT_NAME}_test tests/JSONTest.cpp)
# target_include_directories(${PROJECT_NAME}_test PRIVATE sources)
target_link_libraries(${PROJECT_NAME}_test PRIVATE _${PROJECT_NAME}-${PROJECT_VERSION})

enable_testing()

include(tests/CMakeLists.txt)
include(cpack/CMakeLists.txt)
