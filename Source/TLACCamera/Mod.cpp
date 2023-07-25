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

void cameraSetup(bool enabled, bool all)
{
	if (!enabled)
		return;

	// set initial camera angle
	Vec2 camXz = Vec2(camera->Position.X, camera->Position.Z);
	Vec2 focusXz = Vec2(camera->Focus.X, camera->Focus.Z);
	verticalRotation = AngleFromPoints(camXz, focusXz);

	horizontalRotation = 0;
	camera->Rotation = defaultRotation;

	if (all)
	{
		speedFactor = 1.0f;
		camera->HorizontalFov = defaultFov;
	}
}

bool backWasPressed = false;
bool aWasPressed = false;
bool bWasPressed = false;
bool xWasPressed = false;
bool yWasPressed = false;
bool rsWasPressed = false;

extern "C"
{
	void __declspec(dllexport) Init()
	{
		INSTALL_HOOK(_CameraPosition);
		INSTALL_HOOK(_CameraINTR);
		INSTALL_HOOK(_CameraRotation);
		INSTALL_HOOK(_CameraPerspective);

		camera = (Camera*)0x14CC2B590;
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

				{
					bool backPressed = (Buttons & 0x20) != 0;

					if (backPressed && !backWasPressed)
					{
						isEnabled = !isEnabled;
						cameraSetup(isEnabled, true);
					}

					backWasPressed = backPressed;
				}

				if (isEnabled)
				{
					{
						bool aPressed = (Buttons & 0x1000) != 0;

						if (aPressed && !aWasPressed)
							speedFactor += 0.05f;

						aWasPressed = aPressed;
					}

					{
						bool bPressed = (Buttons & 0x2000) != 0;

						if (bPressed && !bWasPressed)
							speedFactor -= 0.05f;

						bWasPressed = bPressed;
					}

					{
						bool xPressed = (Buttons & 0x4000) != 0;

						if (xPressed && !xWasPressed)
							speedFactor = 1.0f;

						xWasPressed = xPressed;
					}

					speedFactor = max(0.05f, speedFactor);

					{
						bool yPressed = (Buttons & 0x8000) != 0;

						if (yPressed && !yWasPressed)
							camera->HorizontalFov = defaultFov;

						yWasPressed = yPressed;
					}

					{
						bool rsPressed = (Buttons & 0x80) != 0;

						if (rsPressed && !rsWasPressed)
							cameraSetup(isEnabled, false);

						rsWasPressed = rsPressed;
					}

					float LX = state.Gamepad.sThumbLX;
					float LY = state.Gamepad.sThumbLY;

					//determine how far the controller is pushed
					float magnitudeL = sqrt(LX * LX + LY * LY);

					float normalizedLX = 0.0f;
					float normalizedLY = 0.0f;
					float normalizedMagnitudeL = 0.0f;

					//check if the controller is outside a circular dead zone
					if (magnitudeL > INPUT_DEADZONE)
					{
						//clip the magnitude at its expected maximum value
						if (magnitudeL > 32767) magnitudeL = 32767;

						//adjust magnitude relative to the end of the dead zone
						magnitudeL -= INPUT_DEADZONE;

						//determine the direction the controller is pushed
						normalizedLX = LX / magnitudeL;
						normalizedLY = LY / magnitudeL;

						//optionally normalize the magnitude with respect to its expected range
						//giving a magnitude value of 0.0 to 1.0
						normalizedMagnitudeL = magnitudeL / (32767 - INPUT_DEADZONE);
					}
					else //if the controller is in the deadzone zero out the magnitude
					{
						magnitudeL = 0.0f;
						normalizedMagnitudeL = 0.0f;
					}

					bool up = (Buttons & 0x200) != 0;
					bool down = (Buttons & 0x100) != 0;

					bool fast = state.Gamepad.bRightTrigger > 0.0f;
					bool slow = state.Gamepad.bLeftTrigger > 0.0f;

					bool counterclockwise = (Buttons & 0x4) != 0;
					bool clockwise = (Buttons & 0x8) != 0;

					bool zoomin = (Buttons & 0x1) != 0;
					bool zoomout = (Buttons & 0x2) != 0;

					float elapsed = (1000.0f / 60.0f);
					float speed = max(0.005f, elapsed * (fast ? fastSpeed : slow ? slowSpeed : normalSpeed) * speedFactor);

					if (normalizedLY != 0.0f)
						camera->Position += PointFromAngle(verticalRotation, (speed * normalizedLY) * normalizedMagnitudeL);

					if (normalizedLX != 0.0f)
						camera->Position += PointFromAngle(verticalRotation + 90.0f, (speed * normalizedLX) * normalizedMagnitudeL);

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

					float normalizedRX = 0.0f;
					float normalizedRY = 0.0f;
					float normalizedMagnitudeR = 0.0f;

					//check if the controller is outside a circular dead zone
					if (magnitudeR > INPUT_DEADZONE)
					{
						//clip the magnitude at its expected maximum value
						if (magnitudeR > 32767) magnitudeR = 32767;

						//adjust magnitude relative to the end of the dead zone
						magnitudeR -= INPUT_DEADZONE;

						//determine the direction the controller is pushed
						normalizedRX = RX / magnitudeR;
						normalizedRY = RY / magnitudeR;

						//optionally normalize the magnitude with respect to its expected range
						//giving a magnitude value of 0.0 to 1.0
						normalizedMagnitudeR = magnitudeR / (32767 - INPUT_DEADZONE);
					}
					else //if the controller is in the deadzone zero out the magnitude
					{
						magnitudeR = 0.0;
						normalizedMagnitudeR = 0.0;
					}

					if (normalizedMagnitudeR != 0.0f)
					{
						float vertDelta = (normalizedRX * 45.0f) * (speed * normalizedMagnitudeR);
						float horzDelta = (-normalizedRY * 45.0f) * (speed * normalizedMagnitudeR);

						verticalRotation += vertDelta * sensitivity;
						horizontalRotation -= horzDelta * (sensitivity / 5.0f);

						horizontalRotation = std::clamp(horizontalRotation, -75.0f, 75.0f);
					}

					Vec2 focus = PointFromAngle(verticalRotation, 1.0f);
					camera->Focus.X = camera->Position.X + focus.X;
					camera->Focus.Z = camera->Position.Z + focus.Y;

					camera->Focus.Y = camera->Position.Y + PointFromAngle(horizontalRotation, 5.0f).X;
				}
			}
		}
	}
}
