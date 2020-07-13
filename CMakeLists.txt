add_library(utils src/Logger.cxx)
target_link_libraries(utils PRIVATE fmt pthread)
target_link_directories(utils INTERFACE ./)
