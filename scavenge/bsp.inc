 
 
public ReadMapData() {
	if (mapFile != INVALID_HANDLE) {
		CloseHandle(mapFile);
		mapFile = INVALID_HANDLE;
	}
	
	if (mapMetaData != INVALID_HANDLE) {
		CloseHandle(mapMetaData);
		mapMetaData = INVALID_HANDLE;
	}
	
	new String:mapFileName[200];
	new String:stringBuffer[16];
	new dataBuffer[16];
	
	Format(mapFileName, sizeof(mapFileName), "maps/%s.bsp", currentMap);
	
	mapFile = OpenFile(mapFileName, "r");
	
	if (mapFile == INVALID_HANDLE) {
		PrintToServer("[SCAVENGE] Cannot read map file '%s'", mapFileName);
		return;
	}
	
	FileSeek(mapFile, 0, SEEK_SET);
	
	// Check for VBSP header
	if (ReadFileString(mapFile, stringBuffer, sizeof(stringBuffer), 4) != 4) {
		PrintToServer("[SCAVENGE] Cannot read VBSP header from '%s'", mapFileName);
		return;
	}
	
	if (strcmp(stringBuffer, "VBSP") != 0) {
		PrintToServer("[SCAVENGE] Unknown VBSP header from '%s'", mapFileName);
		return;
	}
	
	FileSeek(mapFile, 8, SEEK_SET);
	
	if (ReadFile(mapFile, dataBuffer, 4, 1) != 4) {
		PrintToServer("[SCAVENGE] Cannot read meta data '%s'", mapFileName);
		return;
	}
	
	PrintToServer("dataBuffer[0] = %d", dataBuffer[0]);
	PrintToServer("dataBuffer[1] = %d", dataBuffer[1]);
	PrintToServer("dataBuffer[2] = %d", dataBuffer[2]);
	PrintToServer("dataBuffer[3] = %d", dataBuffer[3]);
	
	mapMetaOffset = (dataBuffer[0] & 0xFF) | ((dataBuffer[1] & 0xFF) << 8) | ((dataBuffer[2] & 0xFF) <<  16) | ((dataBuffer[3] & 0xFF) << 24);
	
	PrintToServer("text offset = %d", mapMetaOffset);
	
	if (mapMetaOffset <= 0) {
		PrintToServer("[SCAVENGE] Cannot find offset for meta data in map '%s'", mapFileName);
		return;
	}
	
	new String:textData[30480];
	new textDataLength = -1;
	
	if (!FileSeek(mapFile, mapMetaOffset, SEEK_SET)) {
		PrintToServer("[SCAVENGE] Cannot seek to meta offset '%s'", mapFileName);
		return;
	}
	
	textDataLength = ReadFileString(mapFile, textData, sizeof(textData), -1);
	
	if (textDataLength <= 0) {
		PrintToServer("[SCAVENGE] Cannot find offset for meta data in map '%s'", mapFileName);
		return;
	}
	
	PrintToServer("text lenth = %d", textDataLength);
	PrintToServer(textData);
	return;
	
	
	if (mapFile != INVALID_HANDLE) {
		CloseHandle(mapFile);
		mapFile = INVALID_HANDLE;
	}
	
	mapMetaData = CreateKeyValues("");
	
	if (!StringToKeyValues(mapMetaData, textData)) {
		PrintToServer("[SCAVENGE] Failed to parse key values");
		return;
	}
	
}
