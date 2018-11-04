cmake_minimum_required(VERSION 2.8.9)
PROJECT(concurrent-cache-trie)

# CMake configuration
#-------------------------------------------------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

# Qt5 configuration
#-------------------------------------------------------------------------------

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include and library directories
#-------------------------------------------------------------------------------

INCLUDE_DIRECTORIES(
    DataStructures
    Nodes
    Operations
)

LINK_DIRECTORIES(

)

# Definition of the source and headers files for the project
#-------------------------------------------------------------------------------

SET(HEADERS
    Nodes/ANode.h
    Nodes/AnyNode.h
    Nodes/SNode.h
)

SET(SOURCES
    DataStructures/Cache.cpp
    DataStructures/CacheTrie.cpp
    Nodes/ANode.cpp
    Nodes/AnyNode.cpp
    Nodes/SNode.cpp
)


# Compilation
#-------------------------------------------------------------------------------
ADD_EXECUTABLE(
    Concurrent-cache-trie
    ${SOURCES}
    ${HEADERS}
)

TARGET_LINK_LIBRARIES(
    Concurrent-cache-trie
)