// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSTypes.h"
#include "TPSCharacter.generated.h"

class UTPSInvetoryComponent;
class UDamageType;
class AController;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;

UCLASS(config = Game)
class ATPSCharacter : public ACharacter
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

public:
    ATPSCharacter();

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseLookUpRate;

protected:
    /** Resets HMD orientation in VR. */
    void OnResetVR();

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of
     * desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of
     * desired turn rate
     */
    void LookUpAtRate(float Rate);

    /** Handler for when a touch input begins. */
    void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

    /** Handler for when a touch input stops. */
    void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    // End of APawn interface

public:
    /** Returns CameraBoom subobject **/
    FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UTPSInvetoryComponent* InvetoryComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    FHealthData HealthData;

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const;

private:
    float Health{0.0f};
    FTimerHandle HealTImeHandle;

    UFUNCTION()
    void OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    void OnHealing();
    void OnDeath();
};
