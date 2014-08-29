
# csgo-scavenge

`scavenge` is a new type of map for *Counter-Strike: Global Offensive* supported
by a sourcemod script and extended usage of the Hammer editor. Scavenge maps randomly
generate the position of entities, bombsites, players, and weapons. This makes a new
game mode designed more around on-the-go strategy and adaption.

# Installation

Download a tarball from Github and extract it into your `csgo` directory for
your dedicated server:

    wget -O csgo-scavenge.tar.gz https://github.com/krisives/csgo-scavenge/archive/master.tar.gz
    tar xzf csgo-scavenge.tar.gz

The next time sourcemod plugins are loaded the scavenge plugin should be loaded.

# Creating Maps

Creating maps is easy!

* Add four (4) `info_null` entities called `bounds1` through `bounds4`. These
  should be placed around the perimeter of the spawnable area in the map.

* Tie each thing to be randomly placed to an entity in the Hammer editor
  starting with `p_` such as `p_littlehut`. By default it can spawn anywhere
  within the bounds.

* Add `constraint_group` to any entity to assign it to a constraint group. For example,
  a team of counter terrorists can be assigned to a group called `cts`.

* To force something to spawn a distance away from other entities use the `constraint_distance`
  key/value pair. The value is a space-separated list of group names followed by the distance.