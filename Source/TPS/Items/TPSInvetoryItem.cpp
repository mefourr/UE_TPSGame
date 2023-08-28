// My game copyright

#include "Items/TPSInvetoryItem.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/TPSInvetoryComponent.h"

ATPSInvetoryItem::ATPSInvetoryItem()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(30.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);
    SetRootComponent(CollisionComponent);
}

void ATPSInvetoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        if (const auto InvetoryComponent = Pawn->FindComponentByClass<UTPSInvetoryComponent>())
        {
            if (InvetoryComponent->TryToAddItem(InvetoryData))
            {
                Destroy();
            }
        }
    }
}
