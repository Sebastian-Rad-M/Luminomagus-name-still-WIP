# Luminomagus

*A game about making something out of nothing.*

**Luminomagus** is a roguelike deckbuilder built entirely from scratch in C++. It merges the deep, combo-chaining of cards with a physics phemomena grimdark setting. 

The player acts as the last "Luminomagus" a caster wielding the building blocks of existence (Light, Warmth, Sound) to fight back formless Void. No one is coming to save you, your doom is certain, however you have to not give up

## Core Mechanics

* The player casts spells using light of 3 difrent frequences and draws cards in an attempt to find their win-condition card and execute it.  

## Technical Stack
Luminomagus abandons traditional game engines in favor of a hardcore, lightweight C++ architecture:
* **Core Language:** C++
* **Rendering & Input:** [Raylib](https://www.raylib.com/) - Used for raw texture rendering, window management, and custom GLSL post-processing shaders.
* **User Interface:** [RmlUi](https://github.com/mikke89/RmlUi) - The entire card layout, typography, and glowing "Light" effects are driven dynamically by HTML/CSS running directly inside the C++ application. 
* **Custom made bridge between the two:** [RaylibRmlUImodernbridge](https://github.com/Sebastian-Rad-M/RaylibRmlUImodernbridge)

## Art & Assets

To maintain the grounded, anti-fantasy atmosphere, the game uses strictly CC0 / Public Domain imagery (macro photography of shattered glass, ferrofluids, soap film interference, etc.), heavily processed for maximum black/white contrast. No AI generation is used in the game's assets.