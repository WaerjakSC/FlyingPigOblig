// Fill out your copyright notice in the Description page of Project Settings.


#include "Brain_Component.h"
#include "FlyingPigFlipbookActor.h"
#include "Engine.h"

// Sets default values for this component's properties
UBrain_Component::UBrain_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrain_Component::BeginPlay()
{
	Super::BeginPlay();
	ann = new ArtificialNN(2, 2, 1, 6, 0.2f);
	piggy = (AFlyingPigFlipbookActor*)GetOwner();
	// ...
	
}


// Called every frame
void UBrain_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (piggy == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(
			0, 60.0f, FColor::Cyan,
			FString("No Pig Attached To The Brain!!!")
		);
		return;
	}
	timer += DeltaTime;

	FString message = FString::Printf(
		TEXT("\n\n\n\
			Stats\n\
			Fails: %d\n\
			Decay Rate: %f\n\
			Last Best Balance: %f\n\
			This Balance: %f"),
		failCount, exploreRate, maxBalanceTime, timer);

	GEngine->AddOnScreenDebugMessage(
		0, 0.5f, FColor::Cyan,
		message
	);
	TArray<double> states;
	TArray<double> qs;

	states.Add(piggy->getPigPosition().Z);
	states.Add(piggy->getPigPhysicsVelocity().Z);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), piggy->getPigPhysicsVelocity().Z);

	qs = SoftMax(ann->CalcOutput(states));
	double maxQ = FMath::Max(qs);
	int maxQIndex = qs.Find(maxQ);
	exploreRate = FMath::Clamp(exploreRate - exploreDecay, minExploreRate, maxExploreRate);
	UE_LOG(LogTemp, Warning, TEXT("maxQIndex = %i"), maxQIndex);
	if (maxQIndex == 0)
	{
		piggy->Move(
			FVector(
				0.0f, 0.0f, 1.0f * flySpeed * (float)qs[maxQIndex]
			), 
			DeltaTime
		);
	}
	else if (maxQIndex == 1)
	{
		piggy->Move(
			FVector(
				0.0f, 0.0f, -1.0f * flySpeed * (float)qs[maxQIndex]
			),
			DeltaTime
		);
	}
	if (piggy->getHitEdge() == true)
	{
		reward = -1.0f;
	}
	else
	{
		reward = 0.1f;
	}

	Replay* lastMemory = new Replay(
		piggy->getPigPosition().Z,
		piggy->getPigPhysicsVelocity().Z,
		reward);
	if (replayMemory.Num() > mCapacity)
		replayMemory.RemoveAt(0);
	replayMemory.Add(lastMemory);

	if (piggy->getHitEdge() == true)
	{
		for (int i = replayMemory.Num() - 1; i >= 0; i--)
		{
			TArray<double> t_outputsOld;
			TArray<double> t_outputsNew;
			t_outputsOld = SoftMax(ann->CalcOutput(replayMemory[i]->states));

			double maxQOld = FMath::Max(t_outputsOld);
			int action = t_outputsOld.Find(maxQOld);

			double feedback;
			if (i == replayMemory.Num() - 1 || replayMemory[i]->reward == -1)
			{
				feedback = replayMemory[i]->reward;
			}
			else
			{
				t_outputsNew = SoftMax(ann->CalcOutput(replayMemory[i + 1]->states));
				maxQ = FMath::Max(t_outputsNew);
				feedback = (replayMemory[i]->reward + discount * maxQ);
			}

			t_outputsOld[action] = feedback;
			ann->Train(replayMemory[i]->states, t_outputsOld);
		}

		//Here, we need to keep the best time and reset the pig
		if (timer > maxBalanceTime)
		{
			maxBalanceTime = timer;
		}

		timer = 0;

		piggy->resetPig();
		piggy->setHitEdge(false);
		replayMemory.Empty();
		failCount++;
	}

	// ...
}
TArray<double> UBrain_Component::SoftMax(TArray<double> values)
{
	double max = FMath::Max<double>(values);

	float scale = 0.0f;
	for (int i = 0; i < values.Num(); ++i)
		scale += FMath::Exp((float)(values[i] - max));

	TArray<double>result;
	for (int i = 0; i < values.Num(); ++i)
		result.Add(FMath::Exp((float)(values[i] - max)) / scale);
	return result;
}

void UBrain_Component::Dead()
{
	piggy->setHitEdge(true);
}

