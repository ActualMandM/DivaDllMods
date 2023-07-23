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

HOOK(void, __fastcall, _CameraPerspective, 0x1402FB7D0, int64_t a1)
{
	if (isEnabled)
		return;
	else
		return original_CameraPerspective(a1);
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
		INSTALL_HOOK(_CameraPerspective);

		camera = (Camera*)_Camera;
	}

	void __declspec(dllexport) OnFrame()
	{
		DWORD dwResult;

		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			// Simply get the state of the controller from XInput.
			dwResult = XInputGetState(i, &state);

			if (dwResult == ERROR_SUCCESS)
			{
				WORD Buttons = state.Gamepad.wButtons;

				bool enabledPressed = (Buttons & 0x20) != 0;

				if (enabledPressed && !enabledWasPressed)
				{
					isEnabled = !isEnabled;
					cameraSetup(isEnabled);
				}

				enabledWasPressed = enabledPressed;

				if (isEnabled)
				{
					if ((Buttons & 0x80) != 0)
						cameraSetup(true);

					float LX = state.Gamepad.sThumbLX;
					float LY = state.Gamepad.sThumbLY;

					//determine how far the controller is pushed
					float magnitudeL = sqrt(LX * LX + LY * LY);

					//determine the direction the controller is pushed
					float normalizedLX = LX / magnitudeL;
					float normalizedLY = LY / magnitudeL;

					float normalizedMagnitudeL = 0;

					//check if the controller is outside a circular dead zone
					if (magnitudeL > INPUT_DEADZONE)
					{
						//clip the magnitude at its expected maximum value
						if (magnitudeL > 32767) magnitudeL = 32767;

						//adjust magnitude relative to the end of the dead zone
						magnitudeL -= INPUT_DEADZONE;

						//optionally normalize the magnitude with respect to its expected range
						//giving a magnitude value of 0.0 to 1.0
						normalizedMagnitudeL = magnitudeL / (32767 - INPUT_DEADZONE);
					}
					else //if the controller is in the deadzone zero out the magnitude
					{
						magnitudeL = 0.0;
						normalizedMagnitudeL = 0.0;
					}

					bool forward = normalizedLY > 0;
					bool backward = normalizedLY < 0;
					bool left = normalizedLX < 0;
					bool right = normalizedLX > 0;

					bool up = (Buttons & 0x200) != 0;
					bool down = (Buttons & 0x100) != 0;

					bool fast = state.Gamepad.bRightTrigger > 0;
					bool slow = state.Gamepad.bLeftTrigger > 0;

					bool counterclockwise = (Buttons & 0x4) != 0;
					bool clockwise = (Buttons & 0x8) != 0;

					bool zoomin = (Buttons & 0x1) != 0;
					bool zoomout = (Buttons & 0x2) != 0;

					float speed = (1000.0f / 60.0f) * (fast ? fastSpeed : slow ? slowSpeed : normalSpeed);

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

					float RX = state.Gamepad.sThumbRX;
					float RY = state.Gamepad.sThumbRY;

					//determine how far the controller is pushed
					float magnitudeR = sqrt(RX * RX + RY * RY);

					//determine the direction the controller is pushed
					float normalizedRX = RX / magnitudeR;
					float normalizedRY = RY / magnitudeR;

					float normalizedMagnitudeR = 0;

					//check if the controller is outside a circular dead zone
					if (magnitudeR > INPUT_DEADZONE)
					{
						//clip the magnitude at its expected maximum value
						if (magnitudeR > 32767) magnitudeR = 32767;

						//adjust magnitude relative to the end of the dead zone
						magnitudeR -= INPUT_DEADZONE;

						//optionally normalize the magnitude with respect to its expected range
						//giving a magnitude value of 0.0 to 1.0
						normalizedMagnitudeR = magnitudeR / (32767 - INPUT_DEADZONE);
					}
					else //if the controller is in the deadzone zero out the magnitude
					{
						magnitudeR = 0.0;
						normalizedMagnitudeR = 0.0;
					}

					bool camup = normalizedRY > 0;
					bool camdown = normalizedRY < 0;
					bool camleft = normalizedRX < 0;
					bool camright = normalizedRX > 0;

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
	}
}
