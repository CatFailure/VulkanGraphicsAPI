#pragma once
namespace SolEngine::Enumeration
{
	enum class NeighbourhoodType
	{
		UKNOWN = -1,
		MOORE,			// 26-Cell Neighbourhood (Include Diagonals)
		VON_NEUMANN,	// 6-Cell Neighbourhood (Exclude Diagonals)
		COUNT
	};
}
