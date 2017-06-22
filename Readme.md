# quinMP

quinMP is a GTA 5 multiplayer mod. It originally started as a rewrite for a different multiplayer mod,
but forked out into its own open source variation instead.

There is currently no publicly available download, but you are free to clone this repository and build it
yourself.

## Features

As this project had only been in development for a few weeks, there isn't very much to it right now. These
are the features that are currently implemented:

* Dedicated server and client implemented using ENet.
* Player movement synchronization.
* Fast entity streaming.
* A simple chat using GTAO's chat scaleform.
* Rockstar-like user interface.

## Running

quinMP works as a .asi mod, and depends on ScriptHookV. So to run this mod, you will need to have the ASI
loader as `dinput8.dll`, ScriptHookV itself as `ScriptHookV.dll`, and the mod's ASI file all in the GTA 5
game folder. This has been tested on the Steam build as well as the Warehouse build.

It will generate a logfile which you can see the mod's events in, in case something breaks. It will also
attempt to load a .ini file for reading your nickname and quick-connect server. (F3 connects to this, and
F4 disconnects.) It's not required however, and you can still connect to any server by using the in-game
menu.

The server shouldn't require any special configuration at this point in time.

## Building

Building the client is done using Visual Studio 2015.

Building the server can be done on Linux using `make`, or on Windows with Visual Studio. This is handled
via `cmake`. On Linux you will need to have the ENet library installed (eg. on Debian, the package is
`libenet-dev`), and on Windows you will need to locate the library manually.

## Contributing

Whether this mod will have its development continued is questionable, so feel free to fork this repository
and improve on the project. I might also accept pull requests, depending on their quality.

## License

	quinMP, a multiplayer mod for GTA5.
	Copyright (C) 2017 github.com/codecat

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
