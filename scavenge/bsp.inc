
// Functions for parsing the entity data from BSP. There are
// supposed to be better ways of doing this, such as using the
// OnLevelInit() hook - but at the time of writing (2014) they
// do not appear to work at all. Instead we have to parse the
// BSP header (very easy) and grab the data from somewhere in
// the map file.

// After parsing we look for scv_group and scv_constraint and
// match them up using "targetname" (the name of the entity)
 
public ReadMapMetaData() {
	if (!ExtractEntFromBsp()) {
		CleanupBSP();
		return;
	}
}

public CleanupBSP() {
	if (mapFile != INVALID_HANDLE) {
		CloseHandle(mapFile);
		mapFile = INVALID_HANDLE;
	}
}

public bool:ExtractEntFromBsp() {
	// Buffer to store the BSP filename in
	new String:bspFileName[100];
	
	// Read the BSP in 4K chunks
	new String:readBuffer[4096];
	
	
	PrintToServer("[SCAVENGE] ExtractEntFromBsp");
	
	Format(bspFileName, sizeof(bspFileName), "maps/%s.bsp", currentMap);
	
	CleanupBSP();
	
	mapFile = OpenFile(bspFileName, "r");
	
	if (mapFile == INVALID_HANDLE) {
		PrintToServer("[SCAVENGE] Cannot open .bsp file %s", bspFileName);
		return false;
	}
	
	FileSeek(mapFile, 0, SEEK_SET);
	
	new pos = 0;
	new bspSize = FileSize(bspFileName);
	new bytesRead = -1;
	new bytesParsed = -1;
	
	if (bspSize <= 0) {
		return false;
	}
	
	// Reads the BSP header and where the .etn file is
	if (!ReadMetaOffset()) {
		return false;
	}
	
	// Seek to where the .ent file starts in the BSP
	pos = mapMetaOffset;
	
	do {
		if (!FileSeek(mapFile, pos, SEEK_SET)) {
			PrintToServer("[SCAVENGE] Failed to seek to BPS offset %d", pos);
			return false;
		}
		
		bytesRead = ReadFileString(mapFile, readBuffer, sizeof(readBuffer), -1);
		
		if (bytesRead == -1) {
			PrintToServer("[SCAVENGE] Failed to read bsp file offset");
			return false;
		}
		
		bytesParsed = ReadMetaText(readBuffer, bytesRead);
		
		if (bytesParsed <= 0) {
			break;
		}
		
		pos = pos + bytesParsed;
	} while (pos < bspSize);
	
	return true;
}

// Reads a chunk of text that should begin with a '{' until it ends with a '}' then
// calls ParseMetaText() with that substring
public ReadMetaText(String:readBuffer[], bytesRead) {
	new i;
	new String:c;
	new endPos = -1;
	new nextStartPos = -1;
	new limit = bytesRead - 1;
	
	if (limit <= 0) {
		PrintToServer("[SCAVENGE] Data chunk is too small");
		return -1;
	}
	
	c = readBuffer[0];
	
	if (c != '{') {
		PrintToServer("[SCAVENGE] Chunk doesn't start with a '{'");
		return -1;
	}
	
	for (i=1; i < limit; i++) {
		c = readBuffer[i];
		
		if (c == '}') {
			endPos = i;
			break;
		}
	}
	
	if (endPos < 0) {
		PrintToServer("[SCAVENGE] Cannot find end of chunk");
		return endPos;
	}
	
	for (i = endPos + 1; i < limit; i++) {
		c = readBuffer[i];
		
		if (c == '{') {
			nextStartPos = i;
			break;
		}
		
		if (c == '\0') {
			readBuffer[endPos + 1] = '\0';
			ParseMetaText(readBuffer);
			return -1;
		}
	}
	
	if (nextStartPos < 0) {
		return -1;
	}
	
	readBuffer[endPos + 1] = '\0';
	ParseMetaText(readBuffer);
	return nextStartPos;
}

public ParseMetaText(String:readBuffer[]) {
	//PrintToServer("--------");
	//PrintToServer("%s", readBuffer);
	//PrintToServer("--------");
	
	new String:lines[64][512];
	new String:key[32];
	new String:value[512];
	new String:entityName[512];
	new lineCount = -1;
	new i;
	
	lineCount = ExplodeString(readBuffer, "\n", lines, 64, 512);
	
	for (i=0; i < lineCount; i++) {
		TrimString(lines[i]);
		
		if (!ParseMetaLine(lines[i], key, value)) {
			continue;
		}
		
		if (strcmp(key, "targetname") == 0) {
			strcopy(entityName, sizeof(entityName), value);
			break;
		}
	}
	
	if (strlen(entityName) <= 0) {
		return;
	}
	
	for (i=0; i < lineCount; i++) {
		if (!ParseMetaLine(lines[i], key, value)) {
			continue;
		}
		
		if (StrContains(key, "scv_") == 0) {
			ParseMetaKey(entityName, key, value);
		}
	}
}

public bool:ParseMetaLine(const String:line[], String:key[], String:value[]) {
	if (strlen(line) <= 0) {
		key[0] = '\0';
		value[0] = '\0';
		return false;
	}
	
	if (line[0] == '{' || line[0] == '}') {
		key[0] = '\0';
		value[0] = '\0';
		return false;
	}
	
	new keyPos;
	new valuePos;
	
	keyPos = ParseQuotedString(line, 0, key);
	
	if (keyPos < 0) {
		key[0] = '\0';
		value[0] = '\0';
		return false;
	}
	
	valuePos = ParseQuotedString(line, keyPos, value);
	
	if (valuePos == -1) {
		key[0] = '\0';
		value[0] = '\0';
		return false;
	}
	
	return true;
}

// Parse a key on an entity that matched scv_
public ParseMetaKey(const String:entityName[], String:key[], const String:value[]) {
	PrintToServer("[SCAVENGE] %s = %s", key, value);
	
	if (strcmp(key, "scv_constraint") == 0) {
		ParseConstraint(entityName, value);
		return;
	}
	
	if (strcmp(key, "scv_group") == 0) {
		return;
	}
}

// Parse a scv_constraint on an entity
public ParseConstraint(const String:entityName[], const String:line[]) {
	new String:args[16][32];
	new argCount;
	new Float:distance;
	
	argCount = ExplodeString(line, " ", args, 16, 32);
	
	if (argCount <= 0) {
		PrintToServer("[SCAVENGE] Missing arguments for scv_constraint");
		return;
	}
	
	distance = StringToFloat(args[0]);
	AddConstraintByName(entityName, args[1], distance);
}

public bool:ReadMetaOffset() {
	new String:str[16];
	new data[16];
	
	// Check for VBSP header
	if (ReadFileString(mapFile, str, sizeof(str), 4) != 4) {
		PrintToServer("[SCAVENGE] Cannot read VBSP header");
		return false;
	}
	
	if (strcmp(str, "VBSP") != 0) {
		PrintToServer("[SCAVENGE] Missing VBSP header");
		return false;
	}
	
	if (!FileSeek(mapFile, 8, SEEK_SET)) {
		PrintToServer("[SCAVENGE] Cannot seek to header 0x08");
		return false;
	}
	
	if (ReadFile(mapFile, data, 4, 1) != 4) {
		PrintToServer("[SCAVENGE] Cannot read meta data offset bytes");
		return false;
	}
	
	// Combine 32-bit integer from 4 bytes
	mapMetaOffset = (data[0] & 0xFF) | ((data[1] & 0xFF) << 8) | ((data[2] & 0xFF) <<  16) | ((data[3] & 0xFF) << 24);
	
	return true;
}



