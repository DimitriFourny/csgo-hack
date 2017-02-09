#include "Surface.h"

void Surface::drawSetColor(int r, int g, int b, int a) {
	typedef void(__thiscall *DrawSetColorFn)(PVOID pThis, int, int, int, int);
	DrawSetColorFn oDrawSetColor = (DrawSetColorFn) m_vtable->GetMethodAddress(VMT_DRAWSETCOLOR);
	oDrawSetColor(self(), r, g, b, a);
}

void Surface::drawFilledRect(int x0, int y0, int x1, int y1) {
	typedef void(__thiscall *DrawFilledRectFn)(PVOID pThis, int, int, int, int);
	DrawFilledRectFn oDrawFilledRect = (DrawFilledRectFn)m_vtable->GetMethodAddress(VMT_DRAWFILLEDRECT);
	oDrawFilledRect(self(), x0, y0, x1, y1);
}

void Surface::drawOutlinedRect(int x0, int y0, int x1, int y1) {
	typedef void(__thiscall *DrawOutlinedRectFn)(PVOID pThis, int, int, int, int);
	DrawOutlinedRectFn oDrawOutlinedRect = (DrawOutlinedRectFn)m_vtable->GetMethodAddress(VMT_DRAWOUTLINEDRECT);
	oDrawOutlinedRect(self(), x0, y0, x1, y1);
}

void Surface::drawLine(int x0, int y0, int x1, int y1) {
	typedef void(__thiscall *DrawLineFn)(PVOID pThis, int, int, int, int);
	DrawLineFn oDrawLine = (DrawLineFn)m_vtable->GetMethodAddress(VMT_DRAWLINE);
	oDrawLine(self(), x0, y0, x1, y1);
}

void Surface::drawSetTextFont(PVOID font) {
	typedef void(__thiscall *DrawSetTextFontFn)(PVOID pThis, PVOID font);
	DrawSetTextFontFn oDrawSetTextFont = (DrawSetTextFontFn)m_vtable->GetMethodAddress(VMT_DRAWSETTEXTFONT);
	oDrawSetTextFont(self(), font);
}

void Surface::drawSetTextColor(int r, int g, int b, int a) {
	typedef void(__thiscall *DrawSetTextColorFn)(PVOID pThis, int r, int g, int b, int a);
	DrawSetTextColorFn oDrawSetTextColor = (DrawSetTextColorFn)m_vtable->GetMethodAddress(VMT_DRAWSETTEXTCOLOR);
	oDrawSetTextColor(self(), r, g, b, a);
}

void Surface::drawSetTextPos(int x, int y) {
	typedef void(__thiscall *DrawSetTextPosFn)(PVOID pThis, int x, int y);
	DrawSetTextPosFn oDrawSetTextPos = (DrawSetTextPosFn)m_vtable->GetMethodAddress(VMT_DRAWSETTEXTPOS);
	oDrawSetTextPos(self(), x, y);
}

void Surface::drawPrintText(const wchar_t* str, int strlen) {
	typedef void(__thiscall *DrawPrintTextFn)(PVOID pThis, const wchar_t* str, int strlen, int drawType);
	DrawPrintTextFn oDrawPrintText = (DrawPrintTextFn)m_vtable->GetMethodAddress(VMT_DRAWPRINTTEXT);
	oDrawPrintText(self(), str, strlen, 0);
}

HFONT Surface::createFont() {
	typedef HFONT(__thiscall *CreateFontFn)(PVOID pThis);
	CreateFontFn oCreateFont = (CreateFontFn)m_vtable->GetMethodAddress(VMT_CREATEFONT);
	return oCreateFont(self());
}

bool Surface::setFontGlyphSet(HFONT font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax) {
	typedef bool(__thiscall *SetFontGlyphSetFn)(PVOID pThis, HFONT, const char*, int, int, int, int, int, int, int);
	SetFontGlyphSetFn oSetFontGlyphSet = (SetFontGlyphSetFn)m_vtable->GetMethodAddress(VMT_SETFONTGLYPHSET);
	return oSetFontGlyphSet(self(), font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
}
