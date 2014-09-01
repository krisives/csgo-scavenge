
# csgo-scavenge

`scavenge` is a new type of map for *Counter-Strike: Global Offensive* supported
by a sourcemod script and extended usage of the Hammer editor. Scavenge maps randomly
generate the position of entities, bombsites, players, and weapons. This makes a new
game mode designed more around on-the-go strategy and adaption.

# Installation

Download the [release zip file](https://github.com/krisives/csgo-scavenge/archive/master.zip) file from Github. It contains a few things:

* The `scavenge.amx` plugin which you can install into your
  `addons/sourcemod/plugins/` directory.

* The `maps/` directory contains any `scv_` you might want to run.

# Creating Maps

Creating maps is easy!

* Add four (4) `info_target` entities called `bounds1` through `bounds4`. These
  should be placed around the perimeter of the spawnable area in the map.

* Tie each thing to be randomly placed to an entity in the Hammer editor
  starting with `p_` such as `p_littlehut`. By default it can spawn anywhere
  within the bounds.

* To force an entity to spawn some distance away from another use add a key
  to that entity named `scv_constraint`. The value should be the distance followed
  by the name of the entity (separated by spaces, with no quotes)


