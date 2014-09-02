 


// Find an entity by its name
public FindEntityByName(String:entityName[]) {
	new lastEntityID = -1;
	new entityID = -1;
	new String:name[64];
	
	//PrintToServer("[SCAVENGE] Finding entity named '%s'", entityName);
	
	while ((entityID = FindEntityByClassname(lastEntityID, "*")) != -1) {
		GetEntPropString(entityID, Prop_Data, "m_iName", name, sizeof(name));
		
		if (strcmp(name, entityName) == 0) {
			return entityID;
		}
		
		lastEntityID = entityID;
	}
	
	PrintToServer("[SCAVENGE] Unable to find entity '%s'", name);
	
	return -1;
}



// Scans each entity in the map passing it to ReadEntity()
public ReadMapEntities() {
	new lastEntityID = -1;
	new entityID = -1;
	
	PrintToServer("[SCAVENGE] Reading map entities...");
	
	while ((entityID = FindEntityByClassname(lastEntityID, "*")) != -1) {
		ReadMapEntity(entityID);
		lastEntityID = entityID;
	}
	
	PrintToServer("[SCAVENGE] %d total entities", allEntityCount);
}

public ReadMapEntity(entityID) {
	new i = -1;
	new String:className[32];
	
	i = allEntityCount;
	allEntities[i] = entityID;
	allEntityCount = allEntityCount + 1;
	
	ReadParticle(entityID);
	
	// Remember the movement type
	allEntityMovementTypes[i] = GetEntityMoveType(entityID);
	
	allEntityNames[i][0] = '\0';
	allEntityParents[i][0] = '\0';
	
	// Remember entity name and parent name
	GetEntPropString(entityID, Prop_Data, "m_iName", allEntityNames[i], 32);
	GetEntPropString(entityID, Prop_Data, "m_iParent", allEntityParents[i], 32);
	GetEntPropVector(entityID, Prop_Data, "m_vecOrigin", allEntityPos[i]);
	
	GetEntityClassname(entityID, className, sizeof(className));
	
	PrintToServer("[SCAVENGE] %s (%s)", allEntityNames[i], className);
	
	// While moving things around ensure they don't clip each other
	SetEntityMoveType(entityID, MOVETYPE_NOCLIP);
}

// Checks if an entity starts with "p_" and makes a particle for it
public ReadParticle(entityID) {
	new String:name[32] = "";
	new particleID = -1;
	
	if (!IsValidEntity(entityID)) {
		return -1;
	}
	
	if (entityID == -1) {
		PrintToServer("[SCAVENGE] Cannot read entity -1");
		return -1;
	}
	
	// Read the name of the entity and ensure it starts with "p_"
	GetEntPropString(entityID, Prop_Data, "m_iName", name, sizeof(name));
	
	if (StrContains(name, "p_") != 0) {
		return -1;
	}
	
	// Create a new particle
	particleID = particleCount;
	particleCount = particleCount + 1;
	
	//particleName[particleID] = name;
	strcopy(particleName[particleID], 32, name);
	particleEnt[particleID] = entityID;
	ReadEntityBounds(particleID);
	
	return particleID;
}

public ReadEntityBounds(id) {
	new Float:mins[3];
	new Float:maxs[3];
	
	GetEntPropVector(particleEnt[id], Prop_Send, "m_vecOrigin", particlePos[id]);
	GetEntPropVector(particleEnt[id], Prop_Send, "m_vecMins", mins);
	GetEntPropVector(particleEnt[id], Prop_Send, "m_vecMaxs", maxs);
	
	particleSize[id][0] = maxs[0] - mins[0];
	particleSize[id][1] = maxs[1] - mins[1];
	particleSize[id][2] = maxs[2] - mins[2];
	
	if (particleSize[id][0] <= 0.0) {
		particleSize[id][0] = 32.0;
	}
	
	if (particleSize[id][1] <= 0.0) {
		particleSize[id][1] = 32.0;
	}
	
	//PrintToServer("Particle %d pos (%f, %f, %f)", id, particlePos[id][0], particlePos[id][1], particlePos[id][2]); 
	//PrintToServer("Particle %s size (%f, %f, %f)", particleName[id], particleSize[id][0], particleSize[id][1], particleSize[id][2]); 
}
