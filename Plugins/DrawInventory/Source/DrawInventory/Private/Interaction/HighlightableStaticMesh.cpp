// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/HighlightableStaticMesh.h"

void UHighlightableStaticMesh::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void UHighlightableStaticMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
