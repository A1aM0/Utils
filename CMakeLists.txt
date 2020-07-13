add_library(utils src/Logger.cxx)
target_link_libraries(utils PRIVATE fmt)
target_link_directories(utils INTERFACE ./)
