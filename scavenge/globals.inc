 

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

// Constraints contain the particle indices and the distance that should be between them
new constraintCount = 0;
new constraints[4096][2];
new Float:constraintDistances[4096];



// Groups of particles such as a team of terrorists, CTs, weapons, bombs,
// are things that will have constraints between them
new particleGroupCount = 0;
new String:particleGroupName[32][50];
new particleGroupChildCount[32];
new particleGroupChild[32][32];
new Float:particleGroupDistance[32];


// Each thing in the map that can move around is a particle within
// a verlet simulation. Entities in the map that start with p_ are
// put into this array
new particleCount = 0;
new Float:particlePos[1024][3];
new Float:particleSize[1024][3];
new particleEnt[1024];
new String:particleName[1024][32];


new allEntities[4096];
new allEntityParents[4096];
new MoveType:allEntityMovementTypes[4096];
new allEntityCount = 0;

