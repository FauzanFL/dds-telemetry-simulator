cmake_minimum_required(VERSION 3.16)

project(dds_telemetry_simulator LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CycloneDDS-CXX REQUIRED)

idlcxx_generate(
    TARGET generated_idl
    FILES
    idl/MissionRoute.idl
    idl/RCWSCommand.idl
    idl/RCWSStatus.idl
    idl/TargetTrack.idl
    idl/Telemetry.idl
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(dds_telemetry_simulator
  main.cpp
)
target_link_libraries(dds_telemetry_simulator PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    CycloneDDS-CXX::ddscxx
    generated_idl)

include(GNUInstallDirs)
install(TARGETS dds_telemetry_simulator
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
