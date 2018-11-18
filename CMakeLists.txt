cmake_minimum_required(VERSION 2.8.9)
PROJECT(concurrent-cache-trie)

# CMake configuration
#-------------------------------------------------------------------------------

    add_definitions(-std=c++11 -pthread)


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
    ConcurrentCache-Trie/Nodes/ANode.h
    ConcurrentCache-Trie/Nodes/AnyNode.h
    ConcurrentCache-Trie/Nodes/SNode.h
    ConcurrentCache-Trie/Nodes/ENode.h
    ConcurrentCache-Trie/Nodes/FNode.h
    ConcurrentCache-Trie/DataStructures/CacheTrie.h
    ConcurrentCache-Trie/DataStructures/CacheNode.h
)

SET(SOURCES
    ConcurrentCache-Trie/DataStructures/CacheNode.cpp
    ConcurrentCache-Trie/DataStructures/CacheTrie.cpp
    ConcurrentCache-Trie/Nodes/ANode.cpp
    ConcurrentCache-Trie/Nodes/AnyNode.cpp
    ConcurrentCache-Trie/Nodes/SNode.cpp
    ConcurrentCache-Trie/Nodes/ENode.cpp
    ConcurrentCache-Trie/Nodes/FNode.cpp
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
)