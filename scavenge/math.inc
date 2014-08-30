

// Copy part of a string to another
public SubStringCopy(const String:from[], fromPos, len, String:to[]) {
	if (len <= 0) {
		to[0] = '\0';
		return;
	}
	
	new i;
	
	for (i=0; i < len; i++) {
		to[i] = from[fromPos + i];
	}
	
	to[len] = '\0';
}

// Parses the next quoted string from a line
public ParseQuotedString(const String:line[], pos, String:buffer[]) {
	new String:c;
	new quoteStart = -1;
	
	while ((c = line[pos]) != '\0') {
		pos = pos + 1;
		
		if (c != '"') {
			continue;
		}
		
		if (quoteStart == -1) {
			quoteStart = pos;
			continue;
		} else {
			SubStringCopy(line, quoteStart, pos - quoteStart - 1, buffer);
			return pos + 1;
		}
	}
	
	return -1;
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