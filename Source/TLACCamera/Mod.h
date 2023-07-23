#include <algorithm>
#include <Windows.h>
#include "Utilities/Math.h"

using namespace Utilities;

struct Camera
{
	Vec3 Position, Focus;
	float Rotation, HorizontalFov, VerticalFov;
};

constexpr uint64_t _Camera = 0x14CC2B590;

const float fastSpeed = 0.1f;
const float slowSpeed = 0.0005f;
const float normalSpeed = 0.005f;

const float defaultRotation = 0.0f;
const float defaultFov = 70.0f;
const float sensitivity = 0.25f;

float verticalRotation;
float horizontalRotation;

bool isEnabled = false;
Camera* camera;