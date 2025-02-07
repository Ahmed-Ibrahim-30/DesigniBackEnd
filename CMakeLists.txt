cmake_minimum_required(VERSION 3.26)
project(Designi_BackEnd)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories("${CMAKE_SOURCE_DIR}/Lib/include")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/"src")
include_directories(${CMAKE_SOURCE_DIR}/"src"/"View")
include_directories(${CMAKE_SOURCE_DIR}/"src"/"Utils")
include_directories(${CMAKE_SOURCE_DIR}/"src"/"Model")
include_directories(${CMAKE_SOURCE_DIR}/"src"/"Controller")


file(GLOB_RECURSE SRC_FILES
        "${CMAKE_SOURCE_DIR}/src/Controller/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/Controller/*.h"

        "${CMAKE_SOURCE_DIR}/src/Model/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/Model/*.h"

        "${CMAKE_SOURCE_DIR}/src/View/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/View/*.h"

        "${CMAKE_SOURCE_DIR}/src/Utils/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/Utils/*.h"
)

add_executable(Designi_BackEnd
        main.cpp
        ${SRC_FILES}
        src/Model/Template/TemplateDesigns.cpp
        src/Model/Template/TemplateDesigns.h
        src/Model/Template/LivingRoomShapeStrategy.h
        src/Model/Template/FamilyRoomShapeStrategy.h
        src/Model/Template/BedroomRoomShapeStrategy.h
        src/Model/Template/RoomShapeStrategy.h
)

find_package(Threads REQUIRED)
target_link_libraries(Designi_BackEnd PRIVATE Threads::Threads)
