// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class FlyingPigProject_00EditorTarget : TargetRules
{
	public FlyingPigProject_00EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "FlyingPigProject_00" } );
	}
}
