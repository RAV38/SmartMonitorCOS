
set_property(GLOBAL PROPERTY TOOLCHAIN_DIR "")
set_property(GLOBAL PROPERTY TOOLCHAIN_PREFIX "arm-none-eabi-")
set_property(GLOBAL PROPERTY CORE "ARM")
set_property(GLOBAL PROPERTY MCU_MANUFACTURER "Gigadevice")
set_property(GLOBAL PROPERTY MCU "GD32F103TB")
set_property(GLOBAL PROPERTY PROJECT "SMART_MONITOR")
set_property(GLOBAL PROPERTY PROJECT_DEFINITIONS 
  "GD32F103" 
  "GD32F10X_MD"
)

# Add you header dirs
set_property(GLOBAL PROPERTY PROJECT_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
  "${CMAKE_CURRENT_SOURCE_DIR}/BLDL/Inc"
)

# Add you source files
file(GLOB SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/MIG/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/BLDL/*.c"
)

set_property(GLOBAL PROPERTY PROJECT_SOURCES ${SRC})
