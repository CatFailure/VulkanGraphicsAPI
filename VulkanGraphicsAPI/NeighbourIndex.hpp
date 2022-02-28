#pragma once

namespace SolEngine::Enumeration
{
    /*
    * Represents the bit-index of neighbouring nodes in a grid:
    * 
    *	     	    Top
    *	     	 	 |  / Front
    *	     	 	 | /
    *	     	 	 |/
    *	Left --------o-------- Right
    *	     	    /|
    *	     	   / |
    *	     Back /  |
    *              Bottom
    */
    enum class NeighbourIndex : unsigned char
    {
        TOP	   = 1,
        BOTTOM = TOP << 1,
        LEFT   = BOTTOM << 1,
        RIGHT  = LEFT << 1,
        FRONT  = RIGHT << 1,
        BACK   = FRONT << 1
    };
}
