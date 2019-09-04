// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingPigFlipbookActor.h"
#include "Brain_Component.h"
#include "GameFramework/Actor.h"
#include "Engine.h"



// Sets default values
AFlyingPigFlipbookActor::AFlyingPigFlipbookActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Brain = CreateDefaultSubobject<UBrain_Component>(TEXT("Brain"));
}

// Called when the game starts or when spawned
void AFlyingPigFlipbookActor::BeginPlay()
{
	Super::BeginPlay();

	paperFlipbookComponent = Cast<UPaperFlipbookComponent>(GetRootComponent());
	paperFlipbookComponent->OnComponentHit.AddDynamic(
		this,
		&AFlyingPigFlipbookActor::OnPigHit
	);



	pigStartPos = paperFlipbookComponent->GetComponentLocation();

}

// Called every frame
void AFlyingPigFlipbookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlyingPigFlipbookActor::OnPigHit(UPrimitiveComponent * hitComp, AActor * otherActor, UPrimitiveComponent * otherComp, FVector NormalImpulse, const FHitResult & hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Obstacle"));

	if (otherActor->GetFName().ToString().Contains("_Dead_Obstacle") == true)
	{
		m_Brain->Dead();
	}
}

FVector AFlyingPigFlipbookActor::getPigPosition()
{
	return paperFlipbookComponent->GetComponentLocation();
}

FTransform AFlyingPigFlipbookActor::getPigTransform()
{
	return paperFlipbookComponent->GetComponentTransform();
}

FVector AFlyingPigFlipbookActor::getPigPhysicsVelocity()
{
	return paperFlipbookComponent->GetComponentVelocity();
}

bool AFlyingPigFlipbookActor::getHitEdge()
{
	return hitEdge;
}

void AFlyingPigFlipbookActor::Move(FVector direction, float DeltaTime)
{
	paperFlipbookComponent->AddForce(
		direction * 10.0f
	);
}

void AFlyingPigFlipbookActor::setHitEdge(bool edge)
{
	hitEdge = edge;
}

void AFlyingPigFlipbookActor::resetPig()
{
	paperFlipbookComponent->SetWorldLocation(pigStartPos);
	paperFlipbookComponent->SetSimulatePhysics(false); 
	paperFlipbookComponent->SetSimulatePhysics(true);
}


