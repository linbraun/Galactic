// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Paintball_StoughtonTarget : TargetRules
{
	public Paintball_StoughtonTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Paintball_Stoughton");
	}
}
