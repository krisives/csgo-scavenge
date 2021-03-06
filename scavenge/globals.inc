 

// Each time a map is loaded it has a unique number that
// is the seed. Using the same seed generates the same map
new mapID = 12345;

new String:currentMap[32];

// Map bounds are computed using entities called "bounds1" through "bounds4"
// to create a bounding volume of where things can spawn.
new mapBoundsPoints = 0;
new Float:mapBoundsMin[3];
new Float:mapBoundsMax[3];
new Float:mapCenter[3];

new mapMetaOffset = -1;
new Handle:mapFile = INVALID_HANDLE;


// Constraints that limit two particles to a distance
new constraintCount = 0;                // Total number of constraints
new constraints[4096][2];               // Constraints contain the index of two particles
new Float:constraintDistances[4096];    // How far away the two particles should stay


// Each thing in the map that can move around is a particle within
// a verlet simulation. Entities in the map that start with p_ are
// put into this array
new particleCount = 0;
new Float:particlePos[1024][3];
new Float:particleSize[1024][3];
new particleEnt[1024];
new String:particleName[1024][32];


new allEntities[4096];
new String:allEntityParents[4096][32];
new String:allEntityNames[4096][32];
new MoveType:allEntityMovementTypes[4096];
new allEntityCount = 0;
new Float:allEntityPos[4096][3];

