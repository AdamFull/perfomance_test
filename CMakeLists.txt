cmake_minimum_required(VERSION 3.18)
project(perfomance_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

file(GLOB PROJECT_SOURCE_FILES "source/*.cpp")

set(PROJECT_WHOLE_SOURCE_FILES
	${PROJECT_SOURCE_FILES}
	"main.cpp"
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/fmt")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/cpu_features")

if(ANDROID)
	add_library(${PROJECT_NAME} SHARED ${PROJECT_WHOLE_SOURCE_FILES})
	target_link_libraries(${PROJECT_NAME} PUBLIC log android ndk_compat)
else()
	add_executable(${PROJECT_NAME} ${PROJECT_WHOLE_SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC fmt cpu_features)

target_include_directories(${PROJECT_NAME}
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source"
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external"
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/fmt/include"
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/cpu_features/include"
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/cpu_features/ndk_compat"
)

target_precompile_headers( ${PROJECT_NAME} PUBLIC "common.h")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/test_cases.json" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/images" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")