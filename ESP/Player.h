#pragma once
#include <iostream>
#include "imgui.h"


// ������ɫ
const ImU32 COLOR_RED = IM_COL32(255, 0, 0, 255);      // ��ɫ
const ImU32 COLOR_GREEN = IM_COL32(0, 255, 0, 255);    // ��ɫ
const ImU32 COLOR_BLUE = IM_COL32(0, 0, 255, 255);     // ��ɫ
const ImU32 COLOR_WHITE = IM_COL32(255, 255, 255, 255);// ��ɫ
const ImU32 COLOR_BLACK = IM_COL32(0, 0, 0, 255);      // ��ɫ
const ImU32 COLOR_YELLOW = IM_COL32(255, 255, 0, 255); // ��ɫ
const ImU32 COLOR_PURPLE = IM_COL32(255, 0, 255, 255); // ��ɫ
const ImU32 COLOR_CYAN = IM_COL32(0, 255, 255, 255);   // ��ɫ

// ��͸����ɫ
const ImU32 COLOR_RED_ALPHA = IM_COL32(255, 0, 0, 128);    // ��͸����ɫ
const ImU32 COLOR_BLACK_ALPHA = IM_COL32(0, 0, 0, 128);    // ��͸����ɫ

namespace gui
{
	// imgui�ؼ�״̬
	inline bool ��ʼ�� = false;


	inline bool	���� = false;
	inline bool ����Ӥ���� = true;

	inline bool ������ʾ = false;
	inline bool ɱ����ʾ = false;
	inline bool λ����ʾ = false;
	inline bool ������ʾ = false;
	inline bool ͼ����ʾ = false;
	inline bool �������ʾ = false;
	inline bool ������ʾ = false;
	inline bool ������ʾ = false;
	inline bool �ؽ���ʾ = false;

	inline bool �������� = false;
	inline bool ֡����ʾ = true;
	inline bool ���ܲ鿴 = false;


}
