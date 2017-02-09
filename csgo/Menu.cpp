#include "Menu.h"
#include "Engine.h"
#include "InputSystem.h"
#include "boneId.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_color_picker.h"
#include <string>

// Radar headers
#include "BaseEntity.h"
#include "ClientEntList.h"
#include "Hooks.h"
#include "Math3.h"

bool Menu::tmpMenuIsOpen = false;
int Menu::menuId = false;
bool Menu::m_menuIsOpen = false;
Config Menu::config = {0};
bool Menu::m_radarIsActive = true;
bool Menu::waitForAimbotKey = false;


bool Menu::save() {
	// In the futur, the settings will be saves in the server
	// For the moment, we save it in C:\csgo\config.dat

	FILE* file = NULL;
	errno_t err = fopen_s(&file, "C:\\csgo\\config.dat", "wb+");
	if (err != 0) {
		return false;
	}

	fwrite(&config, sizeof(config), 1, file);
	fclose(file);
	return true;
}

bool Menu::load() {
	FILE* file = NULL;
	errno_t err = fopen_s(&file, "C:\\csgo\\config.dat", "rb");
	if (err != 0) {
		return false;
	}

	fread(&config, sizeof(config), 1, file);
	fclose(file);
	return true;
}

void Menu::updateMenuState(Loader* loader) {
	Engine* engine = (Engine*) loader->get(ENGINE);

	if (tmpMenuIsOpen != m_menuIsOpen) {
		m_menuIsOpen = tmpMenuIsOpen;
		std::string msg = "cl_mouseenable " + std::to_string(!m_menuIsOpen);
		engine->clientCmd(msg.c_str());
	}
}

bool Menu::menuIsOpen() {
	return m_menuIsOpen;
}

float colors[] = { 0, 0, 0 };
void Menu::showMenu(Loader* loader) {
	updateMenuState(loader);

	ImGui_ImplDX9_NewFrame();

	if (menuIsOpen()) {
		InputSystem* inputSystem = (InputSystem*)loader->get(INPUTSYSTEM);

		int x, y;
		inputSystem->getCursorPosition(&x, &y);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = (float)x;
		io.MousePos.y = (float)y;

		ImGui::Begin("Menu", &tmpMenuIsOpen, ImVec2(620, 300), 0.9f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 115.f);

		if (ImGui::Button("Visual", ImVec2(100.f, 20.f))) menuId = MENU_VISUAL;
		if (ImGui::Button("AimBot##settings", ImVec2(100.f, 20.f))) menuId = MENU_AIMBOT;
		if (ImGui::Button("TriggerBot##settings", ImVec2(100.f, 20.f))) menuId = MENU_TRIGGERBOT;
		if (ImGui::Button("Load", ImVec2(100.f, 20.f))) load();
		if (ImGui::Button("Save", ImVec2(100.f, 20.f))) save();
		ImGui::NextColumn();

		char* bones[] = {
			"Pelvis",
			"Spine 0",
			"Spine 1",
			"Spine 2",
			"Spine 3",
			"Neck",
			"Head"
		};
		int nbBones = 7;
		
		std::string aimKeyText;
		if (waitForAimbotKey) {
			aimKeyText = "<<Press a button>>";
		} else {
			aimKeyText = std::string("AimBot Key: ") + std::string(1, (char)config.aimbotKey).c_str();
		}

		switch (menuId) { 
		case MENU_VISUAL:
			ImGui::Checkbox("ESP Box", &config.espBox);
			ImGui::Checkbox("ESP Skeleton", &config.espSkeleton);
			ImGui::Checkbox("Allies ESP", &config.espAllies);
			ImGui::Checkbox("Show last position", &config.espShowLastPosition);
			ImGui::ColorEdit3("Last position", config.espLastPosition);
			ImGui::Checkbox("Show Radar", &m_radarIsActive);
			break;
		case MENU_AIMBOT:
			ImGui::Checkbox("AimBot always activated", &config.aimbotIsActivated);
			if (ImGui::Button(aimKeyText.c_str(), ImVec2(200.f, 20.f))) waitForAimbotKey = true;
			ImGui::Checkbox("Auto Fire", &config.aimbotAutoFire);
			ImGui::Checkbox("Auto Zoom", &config.aimbotAutoZoom);
			ImGui::Checkbox("Smooth Aim", &config.smoothAimbot);
			ImGui::SliderFloat("Smooth speed", &config.aimbotSmoothSpeed, 0.f, 1.f);
			ImGui::SliderInt("Time between shots (ms)", &config.aimbotWaitingTime, 0, 2000);
			ImGui::Combo("Target", &config.aimbotBoneId, (const char**)bones, nbBones, -1);
			ImGui::Checkbox("Silent Aim", &config.silentAim);
			break;
		case MENU_TRIGGERBOT:
			ImGui::Checkbox("TriggerBot", &config.triggerbotIsActivated);
			ImGui::SliderInt("Time between shots (ms)", &config.triggerbotWaitingTime, 0, 2000);
			break;
		}

		ImGui::Columns(1);
		ImGui::End();
	}

	if (m_radarIsActive) {
		showRadarHack(loader);
	}

	ImGui::Render();
	ImGui::GetIO().MouseDrawCursor = menuIsOpen();
}

void rotateVector(float& x, float& y, float angle, float originX = 0, float originY = 0) {
	x -= originX;
	y -= originY;
	float tx = x, ty = y;
	float fCos = cos(angle);
	float fSin = sin(angle);

	tx = x*fCos - y*fSin;
	ty = x*fSin + y*fCos;

	x = tx + originX;
	y = ty + originY;
}

#define MAKE_IMU32(r, g, b, a) ((a << 24) + (b << 16) + (g << 8) + r) 
void Menu::showRadarHack(Loader* loader) {
	ImGui::Begin("Radar", &m_radarIsActive, ImVec2(200, 200), 0.9f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	// Background
	ImVec2 bgPos = ImGui::GetCursorScreenPos();
	ImVec2 bgSize = ImGui::GetContentRegionAvail();
	ImGui::GetWindowDrawList()->AddRectFilled(bgPos, ImVec2(bgPos.x + bgSize.x, bgPos.y + bgSize.y), 0xFF333333);

	// Check if we are in game
	ClientEntList* clientEntList = (ClientEntList*)loader->get(CLIENTENTLIST);
	BaseEntity localPlayer = BaseEntity(loader);
	if (!clientEntList->getLocalPlayer(localPlayer)) {
		ImGui::End();
		return;
	}

	// Radar
	BaseEntity entity = BaseEntity(loader);
	Vector3 pos;
	float x, y;
	ImVec2 midRadar = ImVec2(bgPos.x + (bgSize.x / 2), bgPos.y + (bgSize.y / 2));
	float mapSizeX = -5000;
	float mapSizeY = 5000;
	ImGui::GetWindowDrawList()->AddLine(ImVec2(midRadar.x-bgSize.x/2.f, midRadar.y), ImVec2(midRadar.x+ bgSize.x / 2.f, midRadar.y), MAKE_IMU32(0, 0, 0, 64));
	ImGui::GetWindowDrawList()->AddLine(ImVec2(midRadar.x, midRadar.y - bgSize.y / 2.f), ImVec2(midRadar.x, midRadar.y + bgSize.y / 2.f), MAKE_IMU32(0, 0, 0, 64));
	// @todo: Use the farthest for the map size 

	// Show local player position
	pos = localPlayer.getOrigin();
	x = midRadar.x;
	y = midRadar.y;
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x - 2, y - 2), ImVec2(x + 2, y + 2), 0xFFFFFFFF);

	// Show local camera
	bool radarViewFixed = true;
	ImVec2 camPos;
	Vector3 forward;
	Math3::angleToForwardVector(Hooks::m_viewAngles, forward);
	forward = forward * 1000;

	if (radarViewFixed) {
		camPos = ImVec2(midRadar.x, 0);
	} else {
		camPos = ImVec2(x - forward.x, y + forward.y);
	}

	float fov = DEG2RAD(90.f); // @todo: get fov
	rotateVector(camPos.x, camPos.y, fov/2.f, x, y);
	ImGui::GetWindowDrawList()->AddLine(ImVec2(x, y), camPos, MAKE_IMU32(199, 202, 74, 255));
	rotateVector(camPos.x, camPos.y, -fov, x, y);
	ImGui::GetWindowDrawList()->AddLine(ImVec2(x, y), camPos, MAKE_IMU32(199, 202, 74, 255));

	// Players positions
	ImU32 color = MAKE_IMU32(0, 0, 255, 255);
	int r, g, b;
	for (int i = 0; i < clientEntList->getHighestEntityIndex(); i++) {
		if (!clientEntList->getClientEntity(i, entity)) {
			continue;
		}
		if (entity.getHealth() <= 0) {
			continue;
		}
		if (entity.self() == localPlayer.self()) {
			continue;
		}

		if (Menu::config.espAllies || entity.isEnemy()) {
			if (entity.isDormant() && !Menu::config.espShowLastPosition) {
				continue;
			}

			// Color
			if (entity.isEnemy()) {
				if (entity.isDormant()) {
					r = (int)(Menu::config.espLastPosition[0] * 255);
					g = (int)(Menu::config.espLastPosition[1] * 255);
					b = (int)(Menu::config.espLastPosition[2] * 255);
					color = MAKE_IMU32(r, g, b, 255);
				}
				else if (entity.isVisible(Terrorist::BoneId::head_0)) {
					color = MAKE_IMU32(0, 255, 0, 255);
				} else {
					color = MAKE_IMU32(255, 0, 0, 255);
				}
			} else {
				color = MAKE_IMU32(0, 0, 255, 255);
			}

			// Show 
			pos = entity.getOrigin();
			pos = pos - localPlayer.getOrigin();

			x = midRadar.x + (pos.x / mapSizeX) * bgSize.x;
			y = midRadar.y + (pos.y / mapSizeY) * bgSize.y;

			if (radarViewFixed) {
				rotateVector(x, y, DEG2RAD((Hooks::m_viewAngles.y + 90)), midRadar.x, midRadar.y);
			}

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x-2,y-2), ImVec2(x+2, y+2), color);
		}
	}
	
	ImGui::End();
}