#pragma once

#include "GameInterface.h"
#include "vmtIndexes.h"
#include "Vmatrix.h"
#include "Vector3.h"
#include "Vector2.h"
#include "QAngle.h"

struct PlayerInfo;

class Engine : public GameInterface {
public:
	using GameInterface::GameInterface;

	bool screenTransform(const Vector3& worldPos, Vector2& screenPos);
	bool worldToScreen(const Vector3& origin, Vector2& screen);

	void getScreenSize(int& width, int& height);
	bool getPlayerInfo(int entNum, PlayerInfo* playerInfo);
	int getLocalPlayer();
	struct Vmatrix& worldToScreenMatrix();
	void setViewAngles(const QAngle& angle);
	void clientCmd(const char* cmd);
};