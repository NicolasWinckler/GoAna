add_subdirectory (comparison)
add_subdirectory (options)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/workspace/config/OscillationAnalysis.cfg")
   configure_file( ${CMAKE_SOURCE_DIR}/analysis/options/OscillationAnalysis.cfg.in ${CMAKE_SOURCE_DIR}/workspace/config/OscillationAnalysis.cfg )
endif()
