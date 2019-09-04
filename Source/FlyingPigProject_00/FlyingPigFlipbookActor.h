// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PaperFlipbookComponent.h"
#include "Classes/Components/ChildActorComponent.h"


//with the "class Brain" declare here
//we do not need to include "Brain.h" here, but we can do that in cpp instead
class UBrain_Component;	


#include "FlyingPigFlipbookActor.generated.h"

UCLASS()
class FLYINGPIGPROJECT_00_API AFlyingPigFlipbookActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPaperFlipbookComponent *paperFlipbookComponent;

	UPROPERTY(VisibleAnywhere, Category = "Q-Learning_NNet")
	UBrain_Component* m_Brain;
	bool hitEdge{ false };
	FVector pigStartPos;

public:	
	// Sets default values for this actor's properties
	AFlyingPigFlipbookActor();

	UPaperFlipbookComponent* getFlipbookComponent() { return paperFlipbookComponent; }
	UBrain_Component* getBrain() { return m_Brain; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnPigHit(
			UPrimitiveComponent *hitComp,
			AActor *otherActor,
			UPrimitiveComponent *otherComp,
			FVector NormalImpulse,
			const FHitResult& hit
		);
	FVector getPigPosition();
	FTransform getPigTransform();
	FVector getPigPhysicsVelocity();

	bool getHitEdge();
	void Move(FVector direction, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Q-Learning_NNet")
		void setHitEdge(bool edge);
	UFUNCTION(BlueprintCallable, Category = "Q-Learning_NNet")
		void resetPig();

};
