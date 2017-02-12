#pragma once

enum NetworkMessageType
{
	// Unknown message, do not use.
	NMT_Unknown,

	// Server to client: The server is forcefully disconnecting the client.
	//   * string: The reason for the disconnect.
	//
	// Client to server: The client is disconnecting from the server.
	//   * string: The reason for the disconnect.
	NMT_Disconnect,
};
