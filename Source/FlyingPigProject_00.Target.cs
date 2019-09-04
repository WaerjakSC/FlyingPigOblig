// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class FlyingPigProject_00Target : TargetRules
{
	public FlyingPigProject_00Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "FlyingPigProject_00" } );
	}
}
