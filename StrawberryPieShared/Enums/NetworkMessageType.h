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
	//
	// Client to server: The player's information, the initial handshake.
	//  * string: The player's Social Club username.
	//  * string: The player's nickname.
	//  * uint32: The player's model hash.
	NMT_Handshake,

	// Server to client: Sending over a batch of entities for the client to create.
	//  * uint32: Amount of entities to create.
	//  * for n:
	//    * NetworkEntityType: The type of entity.
	//    * <serialized data based on entity type, see Entity::NetworkSerialize>
	//      if t == ET_Player:
	//      * NetStructs::CreatePed: The ped that must be created for the player.
	//      * string: The player's Social Club username.
	//      * string: The player's nickname;
	NMT_CreateEntities,

	// Server to client: A new player has joined the game.
	//  * NetStructs::CreatePed: The ped that must be created for the player.
	//  * string: The player's Social Club username.
	//  * string: The player's nickname.
	NMT_PlayerJoin,

	// Server to client: A player has left the game.
	//  * NetHandle: The player that left.
	NMT_PlayerLeave,

	// Server to client: A chat message should appear in the chat window.
	//  * NetHandle: The player that sent the message, if any.
	//  * string: The message in its entirety.
	//
	// Client to server: Client has typed a chat message.
	//  * string: The typed message.
	NMT_ChatMessage,

	// Server to client: A player has moved.
	//  * NetHandle: The player that moved.
	//  * vec3: The player's new position.
	//  * vec3: The player's new rotation.
	//
	// Client to server: The client player has moved.
	//  * vec3: The client's new position.
	//  * vec3: The client's new rotation.
	NMT_PlayerMove,
};
