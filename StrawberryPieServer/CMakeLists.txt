cmake_minimum_required(VERSION 3.8)
project(Strawberry)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")
set(CMAKE_CXX_STANDARD 11)

set(SRC
	src/main.cpp

	src/Common.h

	src/Logging.cpp
	src/Logging.h

	src/GameServer.cpp
	src/GameServer.h

	src/Settings.cpp
	src/Settings.h

	# Network
	src/Network/NetworkManager.cpp
	src/Network/NetworkManager.h

	# Entities
	src/Entities/Player.cpp
	src/Entities/Player.h

	# World
	src/World/World.cpp
	src/World/World.h

	src/World/Entity.cpp
	src/World/Entity.h

	src/World/Queries/Range.cpp
	src/World/Queries/Range.h
)

set(SHARED_SRC
	../StrawberryPieShared/SharedUtils.h
	../StrawberryPieShared/RefCounted.h
	../StrawberryPieShared/Octree.h
	../StrawberryPieShared/Enums/NetworkMessageType.h
	../StrawberryPieShared/Network/NetworkMessage.h
	../StrawberryPieShared/Network/NetHandle.h
	../StrawberryPieShared/Network/NetworkEntityType.h
	../StrawberryPieShared/Network/Structs/CreatePed.h
)

add_executable(StrawberryServer
	${SRC}
	${SHARED_SRC}
)

source_group(TREE "${CMAKE_SOURCE_DIR}/src/" PREFIX Src FILES ${SRC})
source_group(TREE "${CMAKE_SOURCE_DIR}/../StrawberryPieShared/" PREFIX Shared FILES ${SHARED_SRC})

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
	${CMAKE_SOURCE_DIR}/../Libs/glm
	${CMAKE_SOURCE_DIR}/../StrawberryPieShared
)
