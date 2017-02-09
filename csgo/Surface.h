#pragma once

#include "GameInterface.h"
#include "vmtIndexes.h"

enum FontFlag {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

class Surface : public GameInterface {
public:
	using GameInterface::GameInterface;

	void drawSetColor(int r, int g, int b, int a);
	void drawFilledRect(int x0, int y0, int x1, int y1);
	void drawOutlinedRect(int x0, int y0, int x1, int y1);
	void drawLine(int x0, int y0, int x1, int y1);
	void drawSetTextFont(PVOID font);
	void drawSetTextColor(int r, int g, int b, int a);
	void drawSetTextPos(int x, int y);
	void drawPrintText(const wchar_t* str, int strlen);
	HFONT createFont();
	bool setFontGlyphSet(HFONT font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0);
};