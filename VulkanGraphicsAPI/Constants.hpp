#pragma once
#include <glm/glm.hpp>

typedef uint32_t Index_t;

namespace SolEngine::Data
{
	static constexpr glm::vec3 VEC3_AXIS_X{ 1.f, 0.f, 0.f };
	static constexpr glm::vec3 VEC3_AXIS_Y{ 0.f, -1.f, 0.f };
	static constexpr glm::vec3 VEC3_AXIS_Z{ 0.f, 0.f, 1.f };

	static constexpr size_t MAX_GRID_NODES{ 1 << 8 };

    static constexpr uint32_t CUBE_VERTEX_COUNT{ 8U };
    static constexpr uint32_t CUBE_INDEX_COUNT{ 36U };

    static constexpr Vertex CUBE_VERTICES[CUBE_VERTEX_COUNT]
    {
        { { -.5f, .5f, .5f }, { .9f, .9f, .9f } },      // 0
        { { .5f, .5f, .5f }, { .8f, .8f, .1f } },       // 1
        { { .5f, .5f, -.5f }, { 1.f, .6f, .1f } },      // 2
        { { -.5f, .5f, -.5f }, { .8f, .1f, .1f } },     // 3
        { { -.5f, -.5f, .5f }, { .1f, .1f, .8f } },     // 4
        { { .5f, -.5f, .5f }, { .98f, .27f, .41f } },   // 5
        { { .5f, -.5f, -.5f }, { .24f, .36f, .98f } },  // 6
        { { -.5f, -.5f, -.5f }, { .13f, .02f, .3f } },  // 7
    };

	static constexpr Index_t CUBE_INDICES[CUBE_INDEX_COUNT]
	{
        // Bottom-Face
        0, 1, 2,
        0, 2, 3,

        // Back-Face
        0, 4, 5,
        0, 5, 1,

        // Right-Face
        1, 5, 6,
        1, 6, 2,

        // Front-Face
        2, 6, 7,
        2, 7, 3,

        // Left-Face
        3, 7, 4,
        3, 4, 0,

        // Top-Face
        4, 7, 6,
        4, 6, 5,
	};
}