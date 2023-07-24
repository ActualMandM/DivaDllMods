#include <algorithm>
#include <XInput.h>
#include "Utilities/Math.h"

#define INPUT_DEADZONE ( 0.24f * FLOAT(0x7FFF) )

using namespace Utilities;

struct Camera
{
	Vec3 Position, Focus;
	float Rotation, HorizontalFov, VerticalFov;
};

const float fastSpeed = 0.05f;
const float slowSpeed = 0.005f;
const float normalSpeed = 0.01f;

const float defaultRotation = 0.0f;
const float defaultFov = 45.0f;
const float sensitivity = 0.25f;

float verticalRotation;
float horizontalRotation;

bool isEnabled = false;
Camera* camera;