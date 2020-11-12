# c-rpg

A simple text-based RPG written in C. Currently, this is a Work-In-Progress.

Compile from the project folder with:
```
make game
```

Then, head to the folder `/bin` and run the game from there:
```
./game
```


## Progress

### Ver 0.1 - Project Setup

* Game concept and setting up milestones
* Planning program structure

### Ver 0.2 - Characters & Player

* Setting up structures for game characters
* Functions for reading character data from data file.
* Character indexing functions.

### Ver 0.3 - Items

* Setting up structures for game items
* Functions for reading item data from data file.
* Item indexing functions.

### Ver 0.4 - Inventory system

### Ver 0.5 - Combat

Current Version.

Tasks:

- [x] Set-up basic turn-based combat between one player and one monster.
- [x] Allow for a player team and an enemy team. Choose who to attack.
- [x] Detailed combat-specific mechanics with CombatData structure: tells if a character is dead, stunned (and how many more turns), etc.
- [ ] Add option during turn: increase defence (if shield is equipped). Reduces percentage of damage received this round.
- [ ] Add option during turn: increase agility: better chance to dodge next attack.
- [ ] Figure out elegant way to sort out item types and their attributes.
- [ ] Add option during turn: use an item (a potion or switch weapons).
- [ ] Improve enemy AI.

### Milestone - Arena mini-game

* The player can enter an arena to battle monsters of differentl strength level.
* The player receives rewards from the kills, like gold.
* The player can buy new gear at a shop using the gold won.
* Uses: characters and combat, items and inventory, and basic shop menu.


### Future Versions

6. Magic system
7. Skills and levelling system
8. Locations, map, and dungeon generation
9. Savegames
10. Story & Quests
11. Polish GUI and menus

