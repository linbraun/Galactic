// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Paintball_StoughtonEditorTarget : TargetRules
{
	public Paintball_StoughtonEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Paintball_Stoughton");
	}
}
