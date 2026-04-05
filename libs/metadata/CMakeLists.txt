add_library(metadata 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data_definitions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/figure_class.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tools.cpp
)

target_include_directories(metadata PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(metadata PRIVATE logic)