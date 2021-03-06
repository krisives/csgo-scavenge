#include <sourcemod>
#include <float>
#include <entity>
#include <sdkhooks>
#include <sdktools_functions>
#include <sdktools_entinput>

#include "scavenge/math.inc"
#include "scavenge/globals.inc"
#include "scavenge/particles.inc"
#include "scavenge/bsp.inc"
#include "scavenge/map.inc"
#include "scavenge/entity.inc"
#include "scavenge/constraints.inc"
#include "scavenge/commands.inc"
#include "scavenge/hooks.inc"

public Plugin:myinfo =
{
	name = "Scavenge Randomizer",
	author = "Kristopher Ives",
	description = "Randomizes the location of things in a map",
	version = "2.0",
	url = "https://github.com/krisives/csgo-scavenge"
};

public OnPluginStart()
{
	PrintToServer("[SCAVENGE] Plugin loaded");
	
	HookEvent("round_start", OnRoundEndHook, EventHookMode_Post);
	
	RegServerCmd("scv_constraint", ConstraintCommand);
	RegServerCmd("scv_regen", ReloadMapCommand);
	RegServerCmd("scv_force", ForceCommand);
}

public OnMapStart()
{
	PrintToServer("[SCAVENGE] OnMapStart()");
	
	ReadMap();
	ReloadMap();
}

public Action:OnLevelInit(const String:mapName[], String:mapData[2097152]) {
	PrintToServer("[SCAVENGE] OnLevelInit() %d", strlen(mapData));
}

public OnMapEnd() {
	PrintToServer("[SCAVENGE] OnMapEnd()");
	
}

// Creates a new map ID and loads it
public GenerateMap() {
	PrintToServer("[SCAVENGE] Generating new map...");
	mapID = GetURandomInt();
	SetParticlePositions();
}

// Loads the current map ID by placing things in random locations
public SetParticlePositions() {
	PrintToServer("[SCAVENGE] Loading map %d", mapID);
	
	// Set the random generator seed
	SetURandomSeedSimple(mapID + 12345 * mapID);
	
	new i = 0;
	new Float:size[3];
	
	// Calculate size of the map
	size[0] = mapBoundsMax[0] - mapBoundsMin[0];
	size[1] = mapBoundsMax[1] - mapBoundsMin[1];
	size[2] = mapBoundsMax[2] - mapBoundsMin[2];
	
	// Randomly place each entity within the map bounds
	for (i=0; i < particleCount; i++) {
		particlePos[i][0] = mapBoundsMin[0] + GetURandomFloat() * size[0];
		particlePos[i][1] = mapBoundsMin[1] + GetURandomFloat() * size[1];
	}
}

public SolveMap() {
	new i = 0;
	
	PrintToServer("[SCAVENGE] Solving map...");
	PrintToServer("[SCAVENGE] %d constraints", constraintCount);
	PrintToServer("[SCAVENGE] %d particles", particleCount);
	
	for (i = 0; i < 64; i++) {
		SolveMapStep();
	}
}

public SolveMapStep() {
	SolveMapConstraints();
	SolveMapBounds();
	SolveMapParticles();
}

public SolveMapParticles() {
	new a = 0;
	new b = 0;
	new count = particleCount;
	
	for (a = 0; a < count; a++) {
		for (b = 0; b < count; b++) {
			if (a != b) {
				SolveParticle(a, b);
			}
		}
	}
}

public LayoutMap() {
	new i = 0;
	new count = 0;
	
	PrintToServer("[SCAVENGE] Laying out map...");
	
	count = allEntityCount;
	
	for (i=0; i < count; i++) {
		NoClipEntity(i);
	}
	
	count = particleCount;
	
	for (i=0; i < count; i++) {
		LayoutMapParticle(i);
	}
	
	count = allEntityCount;
	
	for (i=0; i < count; i++) {
		LayoutEntity(i);
	}
}

public NoClipEntity(i) {
	if (!IsValidEntity(allEntities[i])) {
		return;
	}
	
	SetEntityMoveType(allEntities[i], MOVETYPE_NOCLIP);
}

public LayoutEntity(i) {
	if (!IsValidEntity(allEntities[i])) {
		return;
	}
	
	new Float:absPos[3];
	new parentID = -1;
	
	SetEntityMoveType(allEntities[i], allEntityMovementTypes[i]);
	
	if (strlen(allEntityParents[i]) > 0) {
		parentID = FindParticleByName(allEntityParents[i]);
		
		if (parentID != -1) {
			absPos[0] = allEntityPos[i][0];
			absPos[1] = allEntityPos[i][1];
			absPos[2] = allEntityPos[i][2];
			
			//PrintToServer("[SCAVENGE] %s (%f, %f, %f)", allEntityParents[i], absPos[0], absPos[1], absPos[2]);
			TeleportEntity(allEntities[i], absPos, NULL_VECTOR, NULL_VECTOR);
		}
	}
	
	
}

public LayoutMapParticle(id) {	
	//KeepParticleWithinBounds(id);
	
	if (!IsValidEntity(particleEnt[id])) {
		PrintToServer("[SCAVENGE] %s is not valid entity", particleName[id]);
		return;
	}
	
	// Move the entity into place
	TeleportEntity(particleEnt[id], particlePos[id], NULL_VECTOR, NULL_VECTOR);
}
