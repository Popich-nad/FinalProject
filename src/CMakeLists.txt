cmake_minimum_required (VERSION 3.8)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

set(Src
	"ConverterJSON.cpp"
	"SearchEngine.cpp"
	"InvertedIndex.cpp"
	"main.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(FinalProject ${Src})

target_link_libraries(FinalProject nlohmann_json::nlohmann_json)
target_include_directories(FinalProject PUBLIC ${MY_INCLUDE_DIR})