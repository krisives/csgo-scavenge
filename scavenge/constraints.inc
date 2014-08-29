


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







