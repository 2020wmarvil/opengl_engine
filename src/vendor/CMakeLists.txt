add_library(SDB_IMAGE STATIC 
    sdb_image.h
    sdb_image.cpp
)

target_include_directories(SDB_IMAGE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
