#pragma once

// The network message type.
//
// Things to do:
//  * Separate rotation from NMT_PlayerMove
//  * Can we figure out a way to put strings in NetStructs?
//  * Maybe implement an "Array" type that we can easily access via read/write? We wouldn't have the integer overhead then.
enum NetworkMessageType
{
	// Unknown message, do not use.
	NMT_Unknown,

	// Server to client: The server is forcefully disconnecting the client.
	//  * string: The reason for the disconnect.
	//
	// Client to server: The client is disconnecting from the server.
	//  * string: The reason for the disconnect.
	NMT_Disconnect,

	// Server to client: The player's information according to the server.
	//  * NetHandle: The player's new network handle.
	//  * vec3: The player's spawn position.
	//  * string: The server name.
	//  * int: The maximum number of players for this server.
	//
	// Client to server: The player's information, the initial handshake.
	//  * string: The player's Social Club username.
	//  * string: The player's nickname.
	//  * uint32: The player's model hash.
	NMT_Handshake,

	// Server to client: Sending over a batch of entities for the client to stream in.
	//  * uint32: Amount of entities to create.
	//  * for n:
	//    * NetworkEntityType: The type of entity.
	//    * NetHandle: The entity's NetHandle.
	//    * <serialized data based on entity type, see Entity::NetworkSerialize>
	//      if t == ET_Player:
	//      * NetStructs::CreatePed: The ped that must be created for the player.
	//      if t == ET_Vehicle:
	//      * NetStructs::CreateVehicle: The vehicle that must be created.
	NMT_StreamIn,

	// Server to client: Sending a batch of nethandles for the client to stream out.
	//  * uint32: Amount of entities to delete.
	//  * for n:
	//    * NetHandle: The entity's handle
	NMT_StreamOut,

	// Server to client: A new player has joined the game.
	//  * NetHandle: The handle for the player.
	//  * string: The player's Social Club username.
	//  * string: The player's nickname.
	NMT_PlayerJoin,

	// Server to client: A player has left the game.
	//  * NetHandle: The player that left.
	NMT_PlayerLeave,

	// Server to client: A chat message should appear in the chat window.
	//  * string: The message in its entirety.
	//
	// Client to server: Client has typed a chat message.
	//  * string: The typed message.
	NMT_ChatMessage,

	// Server to client: A player has moved.
	//  * NetHandle: The player that moved.
	//  * vec3: The player's new position.
	//  * float: The player's new heading.
	//  * vec3: The player's velocity.
	//  * uint8: The player's movement type, where 1 is walking, 2 is running, 3 is sprinting.
	//
	// Client to server: The client player has moved.
	//  * vec3: The client's new position.
	//  * float: The client's new heading.
	//  * vec3: The client's new velocity.
	//  * uint8: The client's new movement type.
	NMT_PlayerMove,

	// Server to client: The weather has changed.
	//  * string: The weather type.
	NMT_Weather,

	// Server to client: The clock time has changed.
	//  * int: Hours
	//  * int: Minutes
	//  * int: Seconds
	NMT_ClockTime,

	// Server to client: A player started tring to enter a vehicle.
	//  * NetHandle: The player that is trying to enter the vehicle.
	//  * NetHandle: The vehicle that is trying to get entered.
	//  * int: The seat of the vehicle that is trying to be entered.
	//
	// Client to server: Player started trying to enter a vehicle.
	//  * NetHandle: The vehicle that is being entered.
	//  * int: The seat of the vehicle that is trying to be entered.
	NMT_EnteringVehicle,

	// Server to client:
	//  * NetHandle: The player that has entered the vehicle.
	//  * NetHandle: The vehicle that the player has entered.
	//  * int: The seat of the vehicle that the player has entered.
	//
	// Client to server:
	//  * NetHandle: The vehicle that the player has entered.
	//  * int: The seat that the player is in.
	NMT_EnteredVehicle,

	// Server to client:
	//  * NetHandle: The player that has left the vehicle.
	//  * NetHandle: The vehicle that the player has left.
	//  * int: The seat of the vehicle that the player has left.
	//
	// Client to server:
	//  * NetHandle: The vehicle that the player has left.
	//  * int: The seat that the player was in.
	NMT_LeftVehicle,
};
