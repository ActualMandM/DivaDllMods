#include "Mod.h"

HOOK(void, __fastcall, _CameraPosition, 0x1402FB760, int64_t a1)
{
	if (isEnabled)
		return;
	else
		return original_CameraPosition(a1);
}

HOOK(void, __fastcall, _CameraINTR, 0x1402FB780, int64_t a1)
{
	if (isEnabled)
		return;
	else
		return original_CameraINTR(a1);
}

HOOK(void, __fastcall, _CameraRotation, 0x1402FB7A0, int64_t a1)
{
	if (isEnabled)
		return;
	else
		return original_CameraRotation(a1);
}

HOOK(void, __fastcall, _CameraPERS, 0x1402FB7D0, int64_t a1)
{
	if (isEnabled)
		return;
	else
		return original_CameraPERS(a1);
}

void cameraSetup(bool enabled)
{
	if (!enabled)
		return;

	// set initial camera angle
	Vec2 camXz = Vec2(camera->Position.X, camera->Position.Z);
	Vec2 focusXz = Vec2(camera->Focus.X, camera->Focus.Z);
	verticalRotation = AngleFromPoints(camXz, focusXz);

	horizontalRotation = 0;
	camera->Rotation = defaultRotation;
	camera->HorizontalFov = defaultFov;
}

bool enabledWasPressed = false;

extern "C"
{
	void __declspec(dllexport) Init()
	{
		INSTALL_HOOK(_CameraPosition);
		INSTALL_HOOK(_CameraINTR);
		INSTALL_HOOK(_CameraRotation);
		INSTALL_HOOK(_CameraPERS);

		camera = (Camera*)_Camera;
	}

	void __declspec(dllexport) OnFrame()
	{
		bool enabledPressed = GetAsyncKeyState(VK_F3) != 0;

		if (enabledPressed && !enabledWasPressed)
		{
			isEnabled = !isEnabled;
			cameraSetup(isEnabled);
		}

		enabledWasPressed = enabledPressed;

		if (isEnabled)
		{
			bool forward = GetAsyncKeyState(0x57) != 0;				// W
			bool backward = GetAsyncKeyState(0x53) != 0;			// S
			bool left = GetAsyncKeyState(0x41) != 0;				// A
			bool right = GetAsyncKeyState(0x44) != 0;				// D

			bool up = GetAsyncKeyState(VK_SPACE) != 0;				// Space
			bool down = GetAsyncKeyState(VK_CONTROL) != 0;			// Control

			bool fast = GetAsyncKeyState(VK_SHIFT) != 0;			// Shift
			bool slow = GetAsyncKeyState(VK_MENU) != 0;				// Menu

			bool counterclockwise = GetAsyncKeyState(0x51) != 0;	// Q
			bool clockwise = GetAsyncKeyState(0x45) != 0;			// E

			bool zoomin = GetAsyncKeyState(0x52) != 0;				// R
			bool zoomout = GetAsyncKeyState(0x46) != 0;				// F

			float speed = (1000.0 / 60.0) * (fast ? fastSpeed : slow ? slowSpeed : normalSpeed);

			if (forward ^ backward)
				camera->Position += PointFromAngle(verticalRotation + (forward ? +0.0f : -180.0f), speed);

			if (left ^ right)
				camera->Position += PointFromAngle(verticalRotation + (right ? +90.0f : -90.0f), speed);

			if (up ^ down)
				camera->Position.Y += speed * (up ? +0.25f : -0.25f);

			if (clockwise ^ counterclockwise)
				camera->Rotation += speed * (clockwise ? -1.0f : +1.0f);

			if (zoomin ^ zoomout)
			{
				camera->HorizontalFov += speed * (zoomin ? -1.0f : +1.0f);
				camera->HorizontalFov = std::clamp(camera->HorizontalFov, +1.0f, +170.0f);
			}

			bool camup = GetAsyncKeyState(0x49) != 0;				// I
			bool camdown = GetAsyncKeyState(0x4B) != 0;				// K
			bool camleft = GetAsyncKeyState(0x4A) != 0;				// J
			bool camright = GetAsyncKeyState(0x4C) != 0;			// L

			float vertDelta = (camright ? 50 : camleft ? -50 : 0) * speed;
			float horzDelta = (camdown ? 50 : camup ? -50 : 0) * speed;

			verticalRotation += vertDelta * sensitivity;
			horizontalRotation -= horzDelta * (sensitivity / 5.0f);

			horizontalRotation = std::clamp(horizontalRotation, -75.0f, +75.0f);

			Vec2 focus = PointFromAngle(verticalRotation, 1.0f);
			camera->Focus.X = camera->Position.X + focus.X;
			camera->Focus.Z = camera->Position.Z + focus.Y;

			camera->Focus.Y = camera->Position.Y + PointFromAngle(horizontalRotation, 5.0f).X;
		}
	}
}
