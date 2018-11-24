cmake_minimum_required(VERSION 2.8.9)
PROJECT(concurrent-cache-trie)

# CMake configuration
#-------------------------------------------------------------------------------

    add_definitions(-std=c++11 -lpthread -Wall -fgnu-tm -O3)


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
    Testing
)

LINK_DIRECTORIES(

)

# Definition of the source and headers files for the project
#-------------------------------------------------------------------------------

SET(HEADERS
    Nodes/ANode.h
    Nodes/AnyNode.h
    Nodes/SNode.h
    Nodes/ENode.h
    Nodes/FNode.h
    DataStructures/CacheTrie.h
    DataStructures/CacheNode.h
)

SET(SOURCES
    DataStructures/CacheNode.cpp
    DataStructures/CacheTrie.cpp
    Nodes/ANode.cpp
    Nodes/AnyNode.cpp
    Nodes/SNode.cpp
    Nodes/ENode.cpp
    Nodes/FNode.cpp
    Testing/TestProgram.cpp
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
    pthread
)
