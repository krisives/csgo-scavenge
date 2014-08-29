 

public DebugCommand() {
	new String:cmd[300];
	GetCmdArgString(cmd, sizeof(cmd));
	PrintToServer("[SCAVENGE] %s", cmd);
}

public Action:ForceCommand(args) {
	PrintToServer("[SCAVENGE] Forcing layout");
	LayoutMap();
}



public Action:GroupCommand(totalArgCount) {
	// scv_group <group> <distance> <particles>...
	
	DebugCommand();
	
	if (totalArgCount < 4) {
		PrintToServer("[SCAVENGE] scv_group missing parameters");
		return;
	}
	
	new String:groupName[32];
	new String:distanceText[32];
	new String:args[16][32];
	new argCount = argCount - 3;
	new i = 0;
	
	GetCmdArg(1, groupName, sizeof(groupName));
	GetCmdArg(2, distanceText, sizeof(distanceText));
	
	for (i=0; i < argCount; i++) {
		GetCmdArg(3 + i, args[i], 32);
	}
	
	GroupFromArray(groupName, distanceText, args, argCount);
}

public GroupFromLine(String:groupName[32], String:distanceText[32], String:listText[200]) {
	new String:args[16][32];
	new argCount = 0;
	
	argCount = ExplodeString(listText, " ", args, 16, 32);
	GroupFromArray(groupName, distanceText, args, argCount);
}

public GroupFromArray(String:groupName[32], String:distanceText[32], String:args[][], argCount) {
	new Float:distance = 0.0;
	new groupID = -1;
	
	if (argCount <= 0) {
		PrintToServer("[SCAVENGE] Tried to group nothing");
		return;
	}
	
	distance = StringToFloat(distanceText);

	if (distance <= 0) {
		PrintToServer("[SCAVENGE] Distance must be greater than zero");
		return;
	}
	
	groupID = FindParticleGroup(groupName);
	
	if (groupID == -1) {
		groupID = CreateParticleGroup(groupName, distance);
	}
	
	new i = 0;
	new particleID = -1;
	
	for (i = 0; i < argCount; i++) {
		if (strlen(args[i]) <= 0) {
			PrintToServer("[SCAVENGE] Empty argument");
			continue;
		}
		
		particleID = FindParticleByName(args[i]);
		
		if (particleID == -1) {
			PrintToServer("[SCAVENGE] Cannot find particle '%s'", args[i]);
			continue;
		}
		
		AddParticleToGroup(groupID, particleID);
		AddGroupConstraint(groupID, particleID, distance);
	}
}


public Action:ConstraintCommand(argCount) {
	// scv_constraint <distance> <a> <b>
	
	DebugCommand();
	
	if (argCount < 3) {
		PrintToServer("[SCAVENGE] scv_constraint missing params");
		return;
	}
	
	new String:distanceText[16];
	new String:sourceText[32];
	new String:destText[32];
	
	GetCmdArg(1, distanceText, sizeof(distanceText));
	GetCmdArg(2, sourceText, sizeof(sourceText));
	GetCmdArg(3, destText, sizeof(destText));
	
	Constraint(distanceText, sourceText, destText);
}

public Constraint(String:distanceText[16], String:sourceText[32], String:destText[32]) {
	new sourceTargetCount = 0;
	new sourceTargets[32];
	new destTargetCount = 0;
	new destTargets[32];
	new Float:distance = 0.0;
	new sourceGroupID = -1;
	new destGroupID = -1;
	new i = 0;
	
	distance = StringToFloat(distanceText);
	
	if (distance <= 0.0) {
		PrintToServer("[SCAVENGE] scv_constraint distance must be greater than zero (%s)", distanceText);
		return;
	}
	
	if (strlen(sourceText) <= 0 || strlen(destText) <= 0) {
		PrintToServer("[SCAVENGE] scv_constraint missing targets");
		return;
	}
	
	if (StrContains(sourceText, "p_") == 0) {
		sourceTargetCount = 1;
		sourceTargets[0] = FindParticleByName(sourceText);
	} else {
		sourceGroupID = FindParticleGroup(sourceText);
		
		if (sourceGroupID == -1) {
			PrintToServer("[SCAVENGE] scv_constraint cannot find source group '%s'", sourceText);
			return;
		}
		
		sourceTargetCount = particleGroupChildCount[sourceGroupID];
		
		for (i = 0; i < sourceTargetCount; i++) {
			sourceTargets[i] = particleGroupChild[sourceGroupID][i];
		}
		
		//sourceTargets = particleGroupChild[sourceGroupID];
	}
	
	if (StrContains(destText, "p_") == 0) {
		destTargetCount = 1;
		destTargets[0] = FindParticleByName(destText);
	} else {
		destGroupID = FindParticleGroup(destText);
		
		if (destGroupID == -1) {
			PrintToServer("[SCAVENGE] scv_constraint cannot find dest group '%s'", destText);
			return;
		}
		
		destTargetCount = particleGroupChildCount[destGroupID];
		
		for (i = 0; i < destTargetCount; i++) {
			destTargets[i] = particleGroupChild[destGroupID][i];
		}
		
		//destTargets = particleGroupChild[destGroupID];
	}
	
	if (sourceTargetCount <= 0 || destTargetCount <= 0) {
		PrintToServer("[SCAVENGE] scv_constraint has no targets");
		return;
	}
	
	new a = 0;
	new b = 0;
	
	for (a = 0; a < sourceTargetCount; a++) {
		for (b = 0; b < destTargetCount; b++) {
			AddConstraint(sourceTargets[a], destTargets[b], distance);
		}
	}
}


public Action:ReloadMapCommand(args) {
	ReloadMap();
}
