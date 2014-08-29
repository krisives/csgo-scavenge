

public ReadConstraints() {
	/*
	// TODO meta data saving is not working right now
	//ServerCommand("scv_group cts 400 p_ct1 p_ct2 p_ct3 p_ct4 p_ct5");
	//ServerCommand("scv_group terrorists 400 p_t1 p_t2 p_t3 p_t4 p_t5");
	//ServerCommand("scv_constraint 3000 p_spawn_t p_spawn_ct");
	//ServerCommand("scv_constraint 300 terrorists p_spawn_t");
	//ServerCommand("scv_constraint 300 cts p_spawn_ct");
	
	//ServerCommand("scv_constraint 30 p_barrel_white1 p_barrel_white2 p_barrel_white3");
	
	GroupFromLine("cts", "100", "p_ct1 p_ct2 p_ct3 p_ct4 p_ct5");
	GroupFromLine("terrorists", "100", "p_t1 p_t2 p_t3 p_t4 p_t5");
	
	Constraint("100", "cts", "p_spawn_ct");
	Constraint("100", "terrorists", "p_spawn_t");
	
	Constraint("3000", "p_spawn_t", "p_spawn_ct");
	
	Constraint("1500", "p_spawn_t", "p_statueroom");
	Constraint("1500", "p_spawn_ct", "p_statueroom");
	
	Constraint("1500", "p_spawn_t", "p_controlroom");
	Constraint("1500", "p_spawn_ct", "p_controlroom");
	
	Constraint("30", "p_barrel_white1", "p_barrel_white2");
	Constraint("30", "p_barrel_white2", "p_barrel_white3");
	Constraint("30", "p_barrel_white3", "p_barrel_white1");
	*/
	
	new entityID = -1;
	new lastEntityID = -1;
	
	PrintToServer("[SCAVENGE] Reading constraints...");
	
	while ((entityID = FindEntityByClassname(lastEntityID, "info_target")) != -1) {
		ReadConstraint(entityID);
		lastEntityID = entityID;
	}
}

public ReadConstraint(entityID) {
	new String:text[200];
	
	GetEntPropString(entityID, Prop_Data, "m_iName", text, sizeof(text));
	
	PrintToServer("[SCAVENGE] meta %s", text);
}

// Ensure two particles don't overlap each other by pushing one away
public SolveParticle(a, b) {
	new Float:aCenterX = particlePos[a][0];
	new Float:aCenterY = particlePos[a][1];
	
	new Float:bCenterX = particlePos[b][0];
	new Float:bCenterY = particlePos[b][1];

	new Float:aWidth = particleSize[a][0];
	new Float:aHeight = particleSize[a][1];
	new Float:bWidth = particleSize[b][0];
	new Float:bHeight = particleSize[b][1];
	
	new Float:aLeft = aCenterX - aWidth * 0.5;
	new Float:aRight = aCenterX + aWidth * 0.5;
	
	new Float:bLeft = bCenterX - bWidth * 0.5;
	new Float:bRight = bCenterX + bWidth * 0.5;
	
	new Float:aTop = aCenterY - aHeight * 0.5;
	new Float:aBottom = aCenterY + aHeight * 0.5;
	
	new Float:bTop = bCenterY - bHeight * 0.5;
	new Float:bBottom = bCenterY + bHeight * 0.5;
	
	if (
		aLeft > bRight ||
		aRight < bLeft ||
		aTop > bBottom ||
		aBottom < bTop
	) {
		return;
	}
	
	new Float:dx = bCenterX - aCenterX;
	new Float:dy = bCenterY - aCenterY;
	
	while (dx == 0.0) {
		dx = GetURandomFloat() - 0.5;
	}
	
	while (dy == 0.0) {
		dy = GetURandomFloat() - 0.5;
	}
	
	new Float:expectedWidth = aWidth * 0.5 + bWidth * 0.5;
	new Float:expectedHeight = aHeight * 0.5 + bHeight * 0.5;
	new Float:px = ((FloatAbs(dx) - expectedWidth) / FloatAbs(dx)) * 0.5;
	new Float:py = ((FloatAbs(dy) - expectedHeight) / FloatAbs(dy)) * 0.5;
	
	particlePos[a][0] = particlePos[a][0] + dx * px;
	particlePos[a][1] = particlePos[a][1] + dy * py;
	
	particlePos[b][0] = particlePos[b][0] - dx * px;
	particlePos[b][1] = particlePos[b][1] - dy * py;
}

public SolveMapConstraints() {
	new i = 0;
	new count = constraintCount;
	
	for (i = 0; i < count; i++) {
		SolveConstraint(i);
	}
}

public SolveConstraint(id) {
	new a = constraints[id][0];
	new b = constraints[id][1];
	new Float:expectedDistance = constraintDistances[id];
	new Float:dx = particlePos[b][0] - particlePos[a][0];
	new Float:dy = particlePos[b][1] - particlePos[a][1];
	new Float:distance = SquareRoot(dx * dx + dy * dy);
	
	while (distance == 0.0) {
		dx = GetURandomFloat() * 5.0 - 10.0;
		dy = GetURandomFloat() * 5.0 - 10.0;
		distance = SquareRoot(dx * dx + dy * dy);
	}
	
	new Float:p = ((distance - expectedDistance) / distance) * 0.5;
	
	particlePos[a][0] = particlePos[a][0] + dx * p;
	particlePos[a][1] = particlePos[a][1] + dy * p;
	
	particlePos[b][0] = particlePos[b][0] - dx * p;
	particlePos[b][1] = particlePos[b][1] - dy * p;
}

public SolveMapBounds() {
	new i = 0;
	new count = particleCount;
	
	for (i=0; i < count; i++) {
		KeepParticleWithinBounds(i);
	}
}


public bool:HasConstraint(a, b) {
	new i = 0;
	new count = particleCount;
	
	for (i = 0; i < count; i++) {
		if (constraints[i][0] == a && constraints[i][1] == b) {
			return true;
		}
		
		if (constraints[i][0] == b && constraints[i][1] == a) {
			return true;
		}
	}
	
	return false;
}

public AddConstraint(a, b, Float:distance) {
	if (a < 0) {
		PrintToServer("[SCAVENGE] Warning constraint has invalid source entity");
		return -1;
	}
	
	if (b < 0) {
		PrintToServer("[SCAVENGE] Warning constraint has invalid dest entity");
		return -1;
	}
	
	if (distance <= 0) {
		PrintToServer("[SCAVENGE] Warning constraint distances must be greater than zero");
		return -1;
	}
	
	if (a == b) {
		PrintToServer("[SCAVENGE] Warning tried to add constraint to self");
		return -1;
	}
	
	if (HasConstraint(a, b)) {
		return -1;
	}
	
	new index = constraintCount;
	constraintCount = constraintCount + 1;
	
	constraints[index][0] = a;
	constraints[index][1] = b;
	constraintDistances[index] = distance;
	
	return index;
}


public KeepParticleWithinBounds(id) {
	new Float:width = particleSize[id][0];
	new Float:height = particleSize[id][1];
	new Float:centerX = particlePos[id][0];
	new Float:centerY = particlePos[id][1];
	
	new Float:left = centerX - width * 0.5;
	new Float:top = centerY - height * 0.5;
	new Float:right = centerX + width * 0.5;
	new Float:bottom = centerY + height * 0.5;
	
	if (left < mapBoundsMin[0]) {
		left = mapBoundsMin[0];
		centerX = left + width * 0.5;
		right = left + width;
		particlePos[id][0] = centerX;
	}
	
	if (right > mapBoundsMax[0]) {
		left = mapBoundsMax[0] - width;
		centerX = left + width * 0.5;
		right = left + width;
		particlePos[id][0] = centerX;
	}
	
	if (top < mapBoundsMin[1]) {
		top = mapBoundsMin[1];
		centerY = top + height * 0.5;
		bottom = top + height;
		particlePos[id][1] = centerY;
	}
	
	if (bottom > mapBoundsMax[1]) {
		top = mapBoundsMax[1] - height;
		centerY = top + height * 0.5;
		bottom = top + height;
		particlePos[id][1] = centerY;
	}
}





