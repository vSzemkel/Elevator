
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Elevator")

set(HEADERS "IElevator.h" "Elevator.h" "ElevatorUser.h")
set(SOURCE_FILES "Program.cpp" "Elevator.cpp" "ElevatorUser.cpp")
add_executable (Elevator ${HEADERS} ${SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Elevator PROPERTY CXX_STANDARD 20)
endif()
