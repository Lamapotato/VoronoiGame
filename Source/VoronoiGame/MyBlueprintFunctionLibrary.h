// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FVoronoiMapCell
{
		GENERATED_BODY()
	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Index;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor Color;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D Coordinate;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D Centroid;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector2D> Vertices;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> NeighborSites;
};

UCLASS()
class VORONOIGAME_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "VORONOI")
	static void GenerateIsland(FVector2D IslandLocation, int X, int Y, int RandomRange, float DistanceBetweenPoints, float IslandRadius, TArray<FVoronoiMapCell> &CellsOut, int Seed);

	UFUNCTION(BlueprintCallable, Category = "VORONOI")
	static void SortActorsByDistance(FVector Location, TArray<AActor*> inArray, TArray<AActor*>& outArray);

	UFUNCTION(BlueprintCallable, Category = "VORONOI")
	static void SortVectorArrayByDistance(FVector Location, TArray<FVector> inArray, TArray<FVector>& outArray);
};
