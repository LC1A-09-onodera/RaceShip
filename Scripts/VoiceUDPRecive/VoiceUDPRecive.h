#pragma once
#include "../UDP/udp.h"
#include "../BaseDirectX/BaseDirectX.h"

class VoiceReciver
{
	static bool isShot;
	static bool isFront;
	static bool isBack;
	static bool isRight;
	static bool isLeft;
	static bool isPouse;
	static bool isWall;
public:
	static UDPClass udp;

	//�o�g���Ƃ��Ɏg������
	static bool GetIsShot() { return isShot; }
	static void SetIsShot(bool shot) { VoiceReciver::isShot = shot; }
	static bool GetWall() { return isWall; }
	static void SetWall(bool wall) { VoiceReciver::isWall = wall; }

	//�����֌W
	static bool GetFront() { return isFront; }
	static bool GetBack() { return isBack; }
	static bool GetRight() { return isRight; }
	static bool GetLeft() { return isLeft; }
	static void SetFront(bool front) { VoiceReciver::isFront = front; }
	static void SetBack(bool back) { VoiceReciver::isBack = back; }
	static void SetRight(bool right) { VoiceReciver::isRight = right; }
	static void SetLeft(bool left) { VoiceReciver::isLeft = left; }

	//
	static bool GetPouse() { return isPouse; }
	static void SetPouse(bool pouse) { VoiceReciver::isPouse = pouse; }

	static void VoiceUDPUpdate();
	static void StartUp();
	static void ReciveData();
	static void EndRecive();
	static void LoadText();
};
