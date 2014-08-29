 



public FindParticleByName(String:name[]) {
	new i = 0;
	new count = particleCount;
	
	for (i = 0; i < count; i++) {
		if (strcmp(particleName[i], name) == 0) {
			return i;
		}
	}
	
	return -1;
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