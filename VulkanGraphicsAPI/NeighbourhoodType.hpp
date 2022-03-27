#pragma once
namespace SolEngine::Enumeration
{
	enum class NeighbourhoodType
	{
		UKNOWN = -1,
		MOORE_NEIGHBOURHOOD,		// 8-Cell Neighbourhood (Include Diagonals)
		VON_NEUMANN_NEIGHBOURHOOD	// 4-Cell Neighbourhood (Exclude Diagonals)
	};
}
