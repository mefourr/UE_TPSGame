// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS/TPSTypes.h"
#include "TPSInvetoryItem.generated.h"

class USphereComponent;

UCLASS()
class TPS_API ATPSInvetoryItem : public AActor
{
    GENERATED_BODY()

public:
    ATPSInvetoryItem();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInvetoryData InvetoryData;
};
