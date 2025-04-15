// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMap.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
AGridMap::AGridMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridMap::GenerateHeight()
{
	if (!Terrain) return;
	LocationHeight.Empty();
	for (int i = 0; i < Map.Num(); i++) {
		for (int j = 0; j < Map[i].GridCell.Num(); j++) {
			for (int x = 0; x < Directions.Num(); x++) {
				FVector Point = FVector(i * Scale + Directions[x].X * (Scale / 2), j * Scale + Directions[x].Y * (Scale / 2), 0);
				FHitResult Hit(1.f);

				FVector TraceStart = Point + FVector(0,0, 2000);
				FVector TraceEnd = Point + FVector(0, 0, -2000);
				FCollisionQueryParams QueryParams;
				QueryParams.bTraceComplex = false;

				Terrain->GetStaticMeshComponent()->LineTraceComponent(Hit, TraceStart, TraceEnd, QueryParams);
				//UE_LOG(LogTemp, Warning, TEXT("%f"), Hit.Location.Z);
				if(Hit.Location.Z != 0)
				LocationHeight.Add(Point, Hit.Location.Z);
			}
		}
	}
}

void AGridMap::GenerateMap(int x, int y)
{
	Map.Empty();
	for (int i = 0; i < x; i++) {
		FGrid MapRow;
		for (int j = 0; j < y; j++) {
			MapRow.GridCell.Add(255);
		}
		Map.Add(MapRow);
	}
}

void AGridMap::PrintMap()
{
	for (int i = 0; i < Map.Num(); i++) {
		FString MapRow;
		for (int j = 0; j < Map[i].GridCell.Num(); j++) {
			MapRow.Append(FString::FromInt(Map[i].GridCell[j]));
		}
		UE_LOG(LogTemp, Warning, TEXT("%s "), *MapRow);
	}
}

void AGridMap::GetBorderTransforms(int Team, TArray<FTransform>& outBorderArray, TArray<FTransform>& outTerritoryArray)
{
	for (int i = 0; i < Map.Num(); i++) {
		for (int j = 0; j < Map[i].GridCell.Num(); j++) {
			if (Map[i].GridCell[j] == Team) {
				outTerritoryArray.Add(FTransform(FVector(i * Scale, j * Scale, 0)));
				for (int x = 0; x < Directions.Num(); x++) {
					if (Map[i + Directions[x].X].GridCell.IsValidIndex(j + Directions[x].Y)) {
						if (Map[i + Directions[x].X].GridCell[j + Directions[x].Y] != Team) {
							FVector Point =  FVector(i * Scale + Directions[x].X * (Scale / 2), j * Scale + Directions[x].Y * (Scale / 2), 0);
							float Height = 0;
							if (LocationHeight.Find(Point)) {
								Height = *LocationHeight.Find(Point);
							}
							Point.Z = Height;
							outBorderArray.Add(FTransform(Point));
						}
					}
				}
			}
		}
	}
}

void AGridMap::PaintOnMap(int x, int y, int BrushRadius, int Team)
{
	if (Map.IsValidIndex(x)) {
		if (Map[x].GridCell.IsValidIndex(y)) {
			for (int i = -BrushRadius; i <= BrushRadius; i++) {
				if (Map.IsValidIndex(i + x)) {
					for (int j = -BrushRadius; j <= BrushRadius; j++) {
						if (Map[i + x].GridCell.IsValidIndex(j + y)) {
							float dx = x - (i + x);
							float dy = y - (j + y);
							float distance = sqrt(dx * dx + dy * dy);
							if (distance <= BrushRadius) {
								Map[i + x].GridCell[j + y] = Team;
							}
						}
					}
				}
			}
		}
	}
}

bool AGridMap::CanBeBuild(TArray<FVector> BuildLocations, int Team)
{
	for (int i = 0; i < BuildLocations.Num(); i++) {
		FVector Location = BuildLocations[i];
		if (!Map.IsValidIndex((int)Location.X / Scale))
			return false;
		if (!Map[(int)Location.X/Scale].GridCell.IsValidIndex((int)Location.Y / Scale))
			return false;
		if (Map[(int)Location.X / Scale].GridCell[(int)Location.Y / Scale] != Team) {
			UE_LOG(LogTemp, Warning, TEXT("%d, %d"), (int)Location.X / Scale, (int)Location.Y / Scale);
			return false;
		}
	}
	return true;
}

bool AGridMap::GetClosestTeamLocation(FVector Location, int Team, FVector& outLocation)
{
	float minDistance = -1;
	for (int i = 0; i < Map.Num(); i++) {
		for (int j = 0; j < Map[i].GridCell.Num(); j++) {
			if (Map[i].GridCell[j] == Team) {
				FVector CurrentLocation = FVector(i * Scale, j * Scale, 0);
				float distance = FVector::Dist2D(Location, CurrentLocation);
				if (distance < minDistance || minDistance == -1) {
					minDistance = distance;
					outLocation = CurrentLocation;
				}
			}
		}
	}
	return minDistance != -1;
}

int AGridMap::GetTeamAtLocation(FVector Location)
{
	int x = Location.X / Scale;
	int y = Location.Y / Scale;

	if (!Map.IsValidIndex(x)) {
		return -1;
	}
	if (!Map[x].GridCell.IsValidIndex(y)) {
		return -1;
	}

	return Map[x].GridCell[y];
}

