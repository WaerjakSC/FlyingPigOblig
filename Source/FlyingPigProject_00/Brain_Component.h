// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArtificialNN/ArtificialNN.h"
#include "Components/ActorComponent.h"
#include "Brain_Component.generated.h"

class Replay {
public:
	TArray<double> states;
	double reward;

	Replay(double pigz, double pigvx, double r) {
		states.Add(pigz);
		states.Add(pigvx);
		reward = r;
	}
};
class AFlyingPigFlipbookActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLYINGPIGPROJECT_00_API UBrain_Component : public UActorComponent
{
	GENERATED_BODY()
private:
	ArtificialNN* ann;
	AFlyingPigFlipbookActor* piggy;

	float reward = 0.0f; // reward to associate with actions
	TArray<Replay*> replayMemory; // memory - list of past actions and rewards
	int mCapacity = 10000; // memory capacity

	float discount = 0.99f; // how much future states affect rewards
	float exploreRate = 100.0f; // chance of picking random action
	float maxExploreRate = 100.0f; // max chance value
	float minExploreRate = 0.01f; // min chance value
	float exploreDecay = 0.0001f; // chance decay amount for each update

	int failCount = 0; // Count how many times the pig hits the top or bottom
	float timer = 0; // timer to keep track of balancing
	float maxBalanceTime = 0; // record time pig balances in the air

	TArray<double> SoftMax(TArray<double>values);


public:	
	// Sets default values for this component's properties
	UBrain_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Dead();
	// CorrectionForce?

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Q-Learning_NNet")
		float flySpeed = 1.5f;

		
};
