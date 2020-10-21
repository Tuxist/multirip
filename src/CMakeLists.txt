include_directories(
)

add_definitions(-D${CMAKE_SYSTEM_NAME})

set(multirip_src
    main.cpp
    rip.cpp
    cddevice.h
    cddevice.cpp
    os/cddevice_linux.cpp
)

add_executable(multirip ${multirip_src})
