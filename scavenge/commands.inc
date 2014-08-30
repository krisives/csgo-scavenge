 

public DebugCommand() {
	new String:cmd[300];
	GetCmdArgString(cmd, sizeof(cmd));
	PrintToServer("[SCAVENGE] %s", cmd);
}

public Action:ForceCommand(args) {
	PrintToServer("[SCAVENGE] Forcing layout");
	LayoutMap();
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
		sourceGroupID = -1;
	}
	
	if (StrContains(destText, "p_") == 0) {
		destTargetCount = 1;
		destTargets[0] = FindParticleByName(destText);
	} else {
		destGroupID = -1;
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
