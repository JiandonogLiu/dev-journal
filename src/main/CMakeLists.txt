file(GLOB_RECURSE main_SRCS ./main.cpp)

add_executable(main_run ${main_SRCS})

target_link_libraries(main_run runtime source middleware ${THIRD_PARTY_LIB})

install(TARGETS main_run DESTINATION ${CMAKE_INSTALL_LIBDIR})
