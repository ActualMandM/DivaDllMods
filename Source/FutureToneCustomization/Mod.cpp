#include <Windows.h>

bool* visualSetting;
int32_t* style;

SIG_SCAN
(
	sigCustomizationStyle,
	0x140681E70,
	"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x88\x41\x2C",
	"xxxxx????xxx"
);

SIG_SCAN
(
	sigNPRArchive,
	0x14040B2B0,
	"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x33\xED\x48\xC7\x41\x00\x00\x00\x00\x00\x48\x8B\xF9\x48\x89\x29\x48\x89\x69\x08\x48\x8D\x99\x00\x00\x00\x00\x40\x88\x69\x10\x8D\x75\x04\x48\x89\x69\x14\x66\x89\x69\x1C\x89\x69\x28\xC7\x41\x00\x00\x00\x00\x00",
	"xxxx?xxxx?xxxx?xxxxxxxxxx?????xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxx?????"
);

SIG_SCAN
(
	sigStyle,
	0x14040CD00,
	"\x48\x89\x6C\x24\x00\x56\x48\x83\xEC\x20\x49\x8B\xF0\x48\x8B\xEA\x48\x3B\xCA\x74\x7D\x48\x89\x5C\x24\x00\x48\x8D\x59\x18\x48\x89\x7C\x24\x00\x49\x8D\x78\x28\x66\x0F\x1F\x84\x00\x00\x00\x00\x00",
	"xxxx?xxxxxxxxxxxxxxxxxxxx?xxxxxxxx?xxxxxxxxx????"
);

SIG_SCAN
(
	sigVisualSetting,
	0x1401D64B0,
	"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x84\xC0\x74\x28",
	"xxxxx????xxxx"
);

bool prevVisualSetting = (bool)-1;

extern "C"
{
	__declspec(dllexport) void Init()
	{
		if (!sigValid)
		{
			versionWarning(TEXT("Future Tone Customization"));
			return;
		}

		{
			uint8_t* instrAddr = (uint8_t*)sigVisualSetting() + 0x40;
			visualSetting = (bool*)(instrAddr + readUnalignedU32(instrAddr + 0x3) + 0x7);
			printf("[Future Tone Customization2] visualSetting: 0x%llx\n", visualSetting);
		}

		{
			uint8_t* instrAddr = (uint8_t*)sigStyle() + 0xA0;
			style = (int32_t*)(instrAddr + readUnalignedU32(instrAddr + 0x3) + 0x7);
			style -= 0x2;
			printf("[Future Tone Customization2] style: 0x%llx\n", style);
		}
	}

	__declspec(dllexport) void OnFrame()
	{

		if (!sigValid)
			return;

		static bool f12KeyWasPressed = false;
		bool f12KeyPressed = (GetAsyncKeyState(VK_F8) & 0x8000) != 0;

		if (!sigValid)
			return;

		if (f12KeyPressed && !f12KeyWasPressed) // Toggle visualSetting when F12 key is pressed
		{
			
			*visualSetting = !*visualSetting;
			prevVisualSetting != *visualSetting;
			printf
			(
				"[Future Tone Customization2] visualSetting: %s\n",
				*visualSetting ? "true" : "false"
			);
			printf
			(
				"[Future Tone Customization2] prevVisualSetting: %s\n",
				prevVisualSetting ? "true" : "false"
			);
		}

		f12KeyWasPressed = f12KeyPressed;

		if (prevVisualSetting != *visualSetting)
		{
			if (*visualSetting)
			{
				WRITE_MEMORY((char*)sigCustomizationStyle() + 0x49, uint8_t, 0xB2, 0x01);
				WRITE_MEMORY((char*)sigCustomizationStyle() + 0x55, uint8_t, 0x33, 0xD2);
				WRITE_MEMORY((char*)sigCustomizationStyle() + 0x5C, uint8_t, 0xB2, 0x01);

				WRITE_MEMORY((char*)sigNPRArchive() + 0x17A, int32_t, -1);

				WRITE_MEMORY(style, int32_t, -1);
			}
			else
			{
				WRITE_MEMORY((char*)sigCustomizationStyle() + 0x49, uint8_t, 0x33, 0xD2);
				WRITE_MEMORY((char*)sigCustomizationStyle() + 0x55, uint8_t, 0xB2, 0x01);
				WRITE_MEMORY((char*)sigCustomizationStyle() + 0x5C, uint8_t, 0x33, 0xD2);

				WRITE_MEMORY((char*)sigNPRArchive() + 0x17A, int32_t, 0);

				WRITE_MEMORY(style, int32_t, 0);
			}

			prevVisualSetting = *visualSetting;
		}
	}
}
