

public CreateParticleGroup(String:groupName[32], Float:distance) {
	if (strlen(groupName) <= 0) {
		PrintToServer("[SCAVENGE] Failed to create group with empty name");
		return -1;
	}
	
	if (distance <= 0) {
		PrintToServer("[SCAVENGE] Failed to create group '%s' because distance must be positive", groupName);
		return -1;
	}
	
	new groupID = particleGroupCount;
	particleGroupCount = particleGroupCount + 1;
	particleGroupDistance[groupID] = distance;
	particleGroupName[groupID] = groupName;
	particleGroupChildCount[groupID] = 0;
	
	PrintToServer("[SCAVENGE] Created group '%s' (%d)", groupName, groupID);
	
	return groupID;
}


public AddParticleToGroup(groupID, particleID) {
	if (groupID == -1) {
		PrintToServer("[SCAVENGE] Tried to add to invalid particle group");
		return -1;
	}
	
	if (particleID == -1) {
		PrintToServer("[SCAVENGE] Tried to add nothing to a group");
		return -1;
	}
	
	new index = particleGroupChildCount[groupID];
	particleGroupChild[groupID][index] = particleID;
	particleGroupChildCount[groupID]++;
	
	return index;
}

public FindParticleGroup(String:name[]) {
	new i = 0;
	new count = particleGroupCount;
	
	PrintToServer("[SCAVENGE] Looking for group '%s'...", name);
	
	for (i=0; i < count; i++) {
		if (strcmp(name, particleGroupName[i]) == 0) {
			return i;
		}
	}
	
	PrintToServer("[SCAVENGE] Cannot find group '%s'...", name);
	
	return -1;
}


public AddGroupConstraint(groupID, particleID, Float:distance) {
	if (particleID < 0) {
		PrintToServer("[SCAVENGE] Warning tried to add constraint to invalid particle");
		return;
	}
	
	if (groupID < 0) {
		PrintToServer("[SCAVENGE] Warning tried to add constraint to invalid group");
		return;
	}
	
	if (distance <= 0) {
		PrintToServer("[SCAVENGE] Warning constraint distances must be greater than zero");
		return;
	}
	
	new i = 0;
	new count = particleGroupChildCount[groupID];
	new otherParticle = -1;
	
	for (i=0; i < count; i++) {
		otherParticle = particleGroupChild[groupID][i];
		
		if (particleID != otherParticle) {
			AddConstraint(particleID, otherParticle, distance);
		}
	}
}