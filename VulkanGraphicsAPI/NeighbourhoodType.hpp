#pragma once
namespace SolEngine::Enumeration
{
	enum class NeighbourhoodType
	{
		UKNOWN = -1,
		MOORE_NEIGHBOURHOOD,		// 14-Cell Neighbourhood (Include Diagonals)
		VON_NEUMANN_NEIGHBOURHOOD,	// 6-Cell Neighbourhood (Exclude Diagonals)
		COUNT
	};
}
