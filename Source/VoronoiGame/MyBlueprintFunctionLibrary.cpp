// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"
#include "VoronoiDiagram.h"


void UMyBlueprintFunctionLibrary::GenerateIsland(FVector2D IslandLocation, int X, int Y, int RandomRange, float DistanceBetweenPoints, float IslandRadius, TArray<FVoronoiMapCell>& CellsOut, int Seed)
{
	#if WITH_EDITOR
		FRandomStream Stream(Seed);
		TSharedPtr<FVoronoiDiagram> VoronoiDiagram(new FVoronoiDiagram(FIntRect(0, 0, 4096, 4096)));
		TArray<FIntPoint> Points;

		for (int i = 0; i < X; i++) {
			for (int j = 0; j < Y; j++) {
				int RandomX = FMath::Clamp((i * DistanceBetweenPoints + Stream.RandRange(-RandomRange, RandomRange)) + 4096 / 2 - X / 2 * DistanceBetweenPoints, 0, 4096);
				int RandomY = FMath::Clamp((j * DistanceBetweenPoints + Stream.RandRange(-RandomRange, RandomRange)) + 4096 / 2 - Y / 2 * DistanceBetweenPoints, 0, 4096);
				Points.AddUnique(FIntPoint(RandomX, RandomY));
			}
		}

		VoronoiDiagram->AddPoints(Points);
		VoronoiDiagram->GenerateSites(2);
		TArray<FVoronoiMapCell> Cells;

		for (int32 i = 0; i < VoronoiDiagram->GeneratedSites.Num(); ++i)
		{
			float dist = FVector2D::Distance(VoronoiDiagram->GeneratedSites[i].Coordinate, IslandLocation);
			if (dist > IslandRadius){
				continue;
			}

			FVoronoiMapCell Cell;

			Cell.Index = VoronoiDiagram->GeneratedSites[i].Index;
			Cell.Coordinate = VoronoiDiagram->GeneratedSites[i].Coordinate;
			Cell.Centroid = VoronoiDiagram->GeneratedSites[i].Centroid;
			Cell.Vertices = VoronoiDiagram->GeneratedSites[i].Vertices;
			Cell.NeighborSites = VoronoiDiagram->GeneratedSites[i].NeighborSites;

			Cells.Add(Cell);
		}
		CellsOut = Cells;
	#endif
}

void UMyBlueprintFunctionLibrary::SortActorsByDistance(FVector Location, TArray<AActor*> inArray, TArray<AActor*>& outArray)
{
	inArray.Sort([Location](const AActor& A, const AActor& B) {
		float DistanceA = FVector::Dist2D(A.GetActorLocation(), Location);
		float DistanceB = FVector::Dist2D(B.GetActorLocation(), Location);
		return DistanceA < DistanceB;
		});
	outArray = inArray;
}

void UMyBlueprintFunctionLibrary::SortVectorArrayByDistance(FVector Location, TArray<FVector> inArray, TArray<FVector>& outArray)
{
	inArray.Sort([Location](const FVector A, const FVector B) {
		float DistanceA = FVector::Dist2D(A, Location);
		float DistanceB = FVector::Dist2D(B, Location);
		return DistanceA < DistanceB;
		});
	outArray = inArray;
}
