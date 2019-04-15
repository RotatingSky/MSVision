/**
 * Header for light controlling.
 * @filename:	LightControl.h
 * @author:		Sny
 * @version:	1.0
 * @since:		2019-01-17
 */

#pragma once

typedef unsigned char byte;
typedef unsigned short uint16;

#pragma comment(lib, "D:/Learning/VisionDetection/StereoDetection/MSVision/MSVisionQt/Resources/lib/LightDLL.lib")

__declspec(dllimport) byte api_LE_ComportConnect(byte Comport);
__declspec(dllimport) byte api_LE_ComportDisConnect(byte Comport);
__declspec(dllimport) byte api_LE_USBConnect();
__declspec(dllimport) byte api_LE_USBDisconnect();
__declspec(dllimport) byte api_LE_SetChMultipier(byte sCH, byte sMul);
__declspec(dllimport) byte api_LE_SetCHMode(byte sCH, byte sMode);
__declspec(dllimport) byte api_LE_SetCONSTOnOff(byte sCH, byte sOn);
__declspec(dllimport) byte api_LE_SetConstInt(byte sCH, byte sInt);
__declspec(dllimport) byte api_LE_SetStrobeWidth(byte sCH, uint16 sWidth);
__declspec(dllimport) byte api_LE_STROBE(bool CH1, bool CH2, bool CH3, bool CH4);
__declspec(dllimport) byte api_LE_SetAutoStrobeUpTime(byte sCH, uint16 sUpTime);
__declspec(dllimport) byte api_LE_SetAutoStrobeDnTime(byte sCH, uint16 sDnTime);
__declspec(dllimport) byte api_LE_AUTOSTROBE(bool CH1, bool CH2, bool CH3, bool CH4);
__declspec(dllimport) byte api_LE_EEPROM();
__declspec(dllimport) byte api_LE_SelectAdd(byte sAdd);