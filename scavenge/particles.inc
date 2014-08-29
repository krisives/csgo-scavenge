 



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

