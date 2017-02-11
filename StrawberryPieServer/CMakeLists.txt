cmake_minimum_required(VERSION 3.0)
project(Strawberry)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

add_executable(StrawberryServer
	src/main.cpp

	src/Common.h

	src/GameServer.cpp
	src/GameServer.h

	src/Settings.cpp
	src/Settings.h

	src/Network/NetworkManager.cpp
	src/Network/NetworkManager.h

	src/Network/NetworkMessage.cpp
	src/Network/NetworkMessage.h

	src/Network/Player.cpp
	src/Network/Player.h
)

find_package(ENet REQUIRED)

target_link_libraries(StrawberryServer
	${ENet_LIBRARIES}
)

if(WIN32)
	target_link_libraries(StrawberryServer
		Ws2_32
		WinMM
	)
endif()

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${ENet_INCLUDE_DIRS}
)
