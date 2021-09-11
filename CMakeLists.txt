cmake_minimum_required(VERSION 3.2)

# Project name
PROJECT(samplesys)

# common settings
set(CMAKE_CXX_STANDARD 11)
INCLUDE_DIRECTORIES(
include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast -fopenmp -Wall -Wno-strict-aliasing")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Ofast -fopenmp -Wall -Wno-strict-aliasing")

file(GLOB DIR_SRCS CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
file(GLOB_RECURSE DIR_ENGINE_SRCS CONFIGURE_DEPENDS "src/engine/*.cpp" "src/engine/*.c")
file(GLOB_RECURSE DIR_GRAPH_SRCS CONFIGURE_DEPENDS "src/graph/*.cpp" "src/graph/*.c")
file(GLOB_RECURSE DIR_UTILS_SRCS CONFIGURE_DEPENDS "src/utils/*.cpp" "src/utils/*.c")
file(GLOB_RECURSE DIR_APPS_GRAPH_ANALYSIS_SRCS CONFIGURE_DEPENDS "src/apps/graphanalysis/*.cpp")

#compile samplesys
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}
${DIR_SRCS} 
${DIR_APPS_GRAPH_ANALYSIS_SRCS}
${DIR_ENGINE_SRCS}
${DIR_GRAPH_SRCS}
${DIR_UTILS_SRCS}
)

# compile gen
ADD_EXECUTABLE(gen src/preprocess/gen.cpp)