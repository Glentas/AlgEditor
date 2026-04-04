add_library(logic 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/first_rank_lines.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/second_rank_lines.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transformations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/polygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/polygonalisation.cpp
)
target_include_directories(logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(logic PRIVATE metadata)