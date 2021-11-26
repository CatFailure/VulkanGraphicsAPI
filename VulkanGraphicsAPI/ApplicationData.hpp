#pragma once
#include "Vector.hpp"

using namespace SolEngine::Math;

struct ApplicationData
{
	const LPCWSTR windowTitle;
	const LPCWSTR windowClassName;
	const LPCWSTR engineName;
	const char *appName;

	Vector2<uint32_t> windowDimensions;
};

