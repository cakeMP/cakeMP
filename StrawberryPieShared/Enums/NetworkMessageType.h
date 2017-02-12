#pragma once

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

	// Client to server: The player's information, the initial handshake.
	//  * string: The player's Social Club username.
	//  * string: The player's nickname.
	NMT_Handshake,

	// Server to client: A chat message should appear in the chat window.
	//  * string: The message in its entirety.
	//
	// Client to server: Client has typed a chat message.
	//  * string: The typed message.
	NMT_ChatMessage,

	// Server to client: A player has moved.
	//  * vec3: The player's new position.
	//  * vec3: The player's new rotation.
	//
	// Client to server: The client player has moved.
	//  * vec3: The client's new position.
	//  * vec3: The client's new rotation.
	NMT_PlayerMove,
};
