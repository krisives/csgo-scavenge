 
public Float:Sgn(Float:f) {
	if (f < 0) {
		return -1.0;
	}
	
	return 1.0;
}

// Get the lesser of two numbers
public Float:min(Float:a, Float:b) {
	if (a < b) {
		return a;
	}
	
	return b;
}

// Get the greater of two numbers
public Float:max(Float:a, Float:b) {
	if (a > b) {
		return a;
	}
	
	return b;
}