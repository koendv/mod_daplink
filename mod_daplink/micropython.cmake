# Create an INTERFACE library for our C module.
add_library(usermod_dap INTERFACE)

# Add our source files to the lib
target_sources(usermod_dap INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/mod_daplink.c
    ${CMAKE_CURRENT_LIST_DIR}/DAPLink/source/daplink/cmsis-dap/DAP.c
    ${CMAKE_CURRENT_LIST_DIR}/DAPLink/source/daplink/cmsis-dap/SW_DP.c
    ${CMAKE_CURRENT_LIST_DIR}/DAPLink/source/daplink/cmsis-dap/JTAG_DP.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/${MICROPY_BOARD}/info.c
)

set_source_files_properties(
    ${CMAKE_CURRENT_LIST_DIR}/DAPLink/source/daplink/cmsis-dap/DAP.c
    PROPERTIES
    COMPILE_OPTIONS "-Wno-unused-variable"
)

# Add the current directory as an include directory.
target_include_directories(usermod_dap INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/DAPLink/source/daplink/cmsis-dap
    ${CMAKE_CURRENT_LIST_DIR}/DAPLink/source/daplink/
    ${CMAKE_CURRENT_LIST_DIR}/DAPLink/source/cmsis-core
    ${CMAKE_CURRENT_LIST_DIR}/platform/${MICROPY_BOARD}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_dap)
