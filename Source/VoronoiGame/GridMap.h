// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector> 
#include "GridMap.generated.h"

class AStaticMeshActor;

USTRUCT()
struct FGrid
{
	GENERATED_BODY()
	TArray<BYTE> GridCell;
};

UCLASS()
class VORONOIGAME_API AGridMap : public AActor
{
	GENERATED_BODY()

	TArray<FGrid> Map;
	
public:	
	// Sets default values for this actor's properties
	AGridMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "GRID")
	void GenerateHeight();
	UFUNCTION(BlueprintCallable, Category = "GRID")
	void GenerateMap(int x, int y);
	UFUNCTION(BlueprintCallable, Category = "GRID")
	void PrintMap();
	UFUNCTION(BlueprintCallable, Category = "GRID")
	void GetBorderTransforms(int Team, TArray<FTransform>& outBorderArray, TArray<FTransform>& outTerritoryArray);
	UFUNCTION(BlueprintCallable, Category = "GRID")
	void PaintOnMap(int x, int y, int BrushRadius, int Team);
	UFUNCTION(BlueprintCallable, Category = "GRID")
	bool CanBeBuild(TArray<FVector> BuildLocations, int Team);
	UFUNCTION(BlueprintCallable, Category = "GRID")
	bool GetClosestTeamLocation(FVector Location, int Team, FVector& outLocation);
	UFUNCTION(BlueprintCallable, Category = "GRID")
	int GetTeamAtLocation(FVector Location);

	TArray<FVector2D> Directions = { FVector2D(0, -1), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(1, 0) };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FVector, float> LocationHeight;

	UPROPERTY(EditAnywhere)
	float Scale = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AStaticMeshActor* Terrain;
};
