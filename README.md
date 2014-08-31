
# csgo-scavenge

`scavenge` is a new type of map for *Counter-Strike: Global Offensive* supported
by a sourcemod script and extended usage of the Hammer editor. Scavenge maps randomly
generate the position of entities, bombsites, players, and weapons. This makes a new
game mode designed more around on-the-go strategy and adaption.

# Installation

Download the .zip file from Github. It contains a few things:

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

* Add `constraint_group` to any entity to assign it to a constraint group. For example,
  a team of counter terrorists can be assigned to a group called `cts`.

* To force something to spawn a distance away from other entities use the `constraint_distance`
  key/value pair. The value is a space-separated list of group names followed by the distance.
