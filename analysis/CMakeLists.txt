add_subdirectory (comparison)
add_subdirectory (options)

configure_file( ${CMAKE_SOURCE_DIR}/analysis/options/OscillationAnalysis.cfg.in ${CMAKE_SOURCE_DIR}/workspace/config/OscillationAnalysis.cfg )

