
### Introduction

![An example of the arena during play.](/Resources/ReadmePics/image1.jpg)

Welcome to the Arena! In this game, each player battles to defend their base and destroy their enemy. Players spawn units to attack, place towers to defend, and design wall layouts to shape the game. This game is a mixture of real time strategy and tower defense. It is both simple and strategic. Players must think ahead and have quick reflexes to win.

### How to Play

Each player is given 3 "spawns". Players win by sending 20 units to each enemy spawn, without losing their own.

There are three types of units: square, triangle, and circle. Each follow a classic battle triangle:

1 square can take out 2 triangles.
1 triangle can take out 2 circles.
1 circle can take out 2 squares.

Each unit costs the same amount and does the same amount of damage to an enemy spawn. Units can be spawned by clicking on a spawn (highlighting it) and then selecting the appropriate unit button at the bottom.

At the bottom of the screen, the player has an "energy" level. Each unit costs 1 unit of energy; if the player has no energy, the unit buttons will be grayed out and unselectable. Energy passively accumulates throughout the game.

Players cannot control their units; only where they spawn and what type. However, players can place walls. Walls can be used to create chokepoints, funnels, or tunnels. Walls cannot be destroyed by units, and cannot seal off any spawn completely. Units will readjust in real-time.

Players can also place towers. Towers will automatically shoot any unit within range. Players cannot have towers shoot specific units, however, they can ask them to shoot other towers or walls. Towers can be selected, and then another tower/wall selected, in order to destroy that structure. Additionally, units will avoid towers if they can. They will evaluate whether it is worth going through the tower zone, or going around it.

Players can place up to 5 towers, but unlimited walls. Towers are quite expensive (20 energy!), and must be strategically planned for.

### What doesn't work

For the most part, the main mechanics and graphics for this game are working. However, multiplayer does not currently work. The matchmaking server has been taken down until more work can be done on the multiplayer aspect of things. The game works for multiplayer in theory once connections are established, but matchmaking has proven to be a difficult issue. This is likely the first aspect to be worked on in this.

The bots currently work, but are incredibly silly. See future work for the future of the bots.

### Getting Started

This program is built using Windows libraries, and can be compiled using:

```mingw32-make```

After compiling, the program can be rerun like so:

```.\main.exe```

Note this will not work without the required Windows libraries, including Direct2D 1.1.

Because this repo contains all the code for this game, players can mess around with the config.h file and find settings they like, including costs, income, movement speed, etc. There is also a template bot in case players wish to make their own AI. What bots play in singleplayer can be changed in network.cpp (this will be changed in the future to parameter file or config.h. See Future work.)

### Future Work

Currently, the bots are extremely simple. They either do nothing, or simply create one unit at a time of a specific type. At some point, I'd like to come back and make more intelligent bots.

Multiplayer currently doesn't work, and the matchmaking server is offline. At some point, I'll rewrite the server, and it be back online.

It's possible at some point I'll clean up the graphics and networking and publish this on Steam. (I think it would be fun!)

### Bugs

A small bug occurs at the top left corner of the window. The window should be titled "The Arena", but for whatever reason, only displays "T". I'm still working on this.

Other than that (and the multiplayer server), there are no known major bugs. Any bugs or questions can be reported to me at devinfrandsen2@gmail.com.

###### Written by Devin Frandsen; December 2025
