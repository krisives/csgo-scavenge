
// Called every time the map may have been changed
public ReadMap() {
	new String:mapName[32];
	
	GetCurrentMap(mapName, sizeof(mapName));
	
	if (strcmp(currentMap, mapName) == 0) {
		return;
	}
	
	// Map is actually different from what we last saw
	strcopy(currentMap, sizeof(currentMap), mapName);
	
	particleCount = 0;
	constraintCount = 0;
	allEntityCount = 0;
	mapBoundsPoints = 0;
	
	// Only apply the plugin to maps of this game type
	if (StrContains(mapName, "scv_") != 0) {
		return;
	}
	
	PrintToServer("[SCAVENGE] Reading map %s", mapName);
	
	// Reads the data from the world
	ReadMapEntities();
	GenerateWeapons();
	
	// Reads extra meta data
	ReadMapMetaData();
	CalculateMapBounds();
}

public GenerateWeapons() {
	AddWeaponParticle("weapon_p90");
	AddWeaponParticle("weapon_ak47");
	AddWeaponParticle("weapon_m4a1");
	AddWeaponParticle("weapon_m4a1_silencer");
	
	AddWeaponParticle("weapon_p250");
	AddWeaponParticle("weapon_deagle");
	AddWeaponParticle("weapon_fiveseven");
	
	AddWeaponParticle("weapon_awp");
	AddWeaponParticle("weapon_ssg08");
	
	for (new i=0; i < 5; i++) {
		AddWeaponParticle("weapon_hegrenade");
		AddWeaponParticle("weapon_flashbang");
		AddWeaponParticle("weapon_flashbang");
		AddWeaponParticle("weapon_flashbang");
	}
}

public AddWeaponParticle(const String:className[]) {
	new entityID = -1;
	new id = -1;
	
	entityID = CreateEntityByName(className);
	
	if (entityID == -1) {
		PrintToServer("Cannot create weapon");
		return -1;
	}
	
	DispatchSpawn(entityID);
	
	id = particleCount;
	particleCount = particleCount + 1;

	
	strcopy(particleName[id], 32, "");
	particleEnt[id] = entityID;
	ReadEntityBounds(id);
	
	return id;
}

// Find the bounds of the map and the center. This is the area
// where entities may spawn within
public CalculateMapBounds() {
	PrintToServer("[SCAVENGE] Calculating map bounds...");
	
	mapBoundsMin[0] =  999999.0;
	mapBoundsMin[1] =  999999.0;
	mapBoundsMin[2] =  999999.0;
	
	mapBoundsMax[0] = -999999.0;
	mapBoundsMax[1] = -999999.0;
	mapBoundsMax[2] = -999999.0;
	
	mapCenter[0] = 0.0;
	mapCenter[1] = 0.0;
	mapCenter[2] = 0.0;
	
	mapBoundsPoints = 0;
	
	UpdateMapBoundsByEntityName("bounds1");
	UpdateMapBoundsByEntityName("bounds2");
	UpdateMapBoundsByEntityName("bounds3");
	UpdateMapBoundsByEntityName("bounds4");
	
	PrintToServer("[SCAVENGE] Map bounds had %d points", mapBoundsPoints);
	
	if (mapBoundsPoints > 0) {
		mapCenter[0] = mapCenter[0] / (0.0 + mapBoundsPoints);
		mapCenter[1] = mapCenter[1] / (0.0 + mapBoundsPoints);
		mapCenter[2] = mapCenter[2] / (0.0 + mapBoundsPoints);
	}
	
	PrintToServer("[SCAVENGE] Map center is (%f, %f, %f)", mapCenter[0], mapCenter[1], mapCenter[2]);
	PrintToServer("[SCAVENGE] Min (%f, %f, %f)", mapBoundsMin[0], mapBoundsMin[1], mapBoundsMin[2]);
	PrintToServer("[SCAVENGE] Max (%f, %f, %f)", mapBoundsMax[0], mapBoundsMax[1], mapBoundsMax[2]);
	
}

// Update the map bounds using the position of the entity by name
public UpdateMapBoundsByEntityName(String:entityName[]) {
	new entityID = -1;
	
	entityID = FindEntityByName(entityName);
	
	if (entityID == -1) {
		return;
	}
	
	UpdateMapBoundsByEntity(entityID);
}

// Update the bounds by the position of an entity by it's index
public UpdateMapBoundsByEntity(entityID) {
	new Float:pos[3];
	GetEntPropVector(entityID, Prop_Send, "m_vecOrigin", pos);
	UpdateMapBoundsByPosition(pos[0], pos[1], pos[2]);
}

// Update the map bounds by a 3D vector
public UpdateMapBoundsByPosition(Float:x, Float:y, Float:z) {
	mapBoundsMin[0] = min(mapBoundsMin[0], x);
	mapBoundsMin[1] = min(mapBoundsMin[1], y);
	mapBoundsMin[2] = min(mapBoundsMin[2], z);
	
	mapBoundsMax[0] = max(mapBoundsMax[0], x);
	mapBoundsMax[1] = max(mapBoundsMax[1], y);
	mapBoundsMax[2] = max(mapBoundsMax[2], z);
	
	mapCenter[0] = mapCenter[0] + x;
	mapCenter[1] = mapCenter[1] + y;
	mapCenter[2] = mapCenter[2] + z;
	
	mapBoundsPoints = mapBoundsPoints + 1;
}


public ReloadMap() {
	PrintToServer("[SCAVENGE] Map reload");
	
	GenerateMap();
	SolveMap();
	LayoutMap();
}


