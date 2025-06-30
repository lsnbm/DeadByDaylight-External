#pragma once
#include <iostream>
#include "imgui.h"


// 基础颜色
const ImU32 COLOR_RED = IM_COL32(255, 0, 0, 255);      // 红色
const ImU32 COLOR_GREEN = IM_COL32(0, 255, 0, 255);    // 绿色
const ImU32 COLOR_BLUE = IM_COL32(0, 0, 255, 255);     // 蓝色
const ImU32 COLOR_WHITE = IM_COL32(255, 255, 255, 255);// 白色
const ImU32 COLOR_BLACK = IM_COL32(0, 0, 0, 255);      // 黑色
const ImU32 COLOR_YELLOW = IM_COL32(255, 255, 0, 255); // 黄色
const ImU32 COLOR_PURPLE = IM_COL32(255, 0, 255, 255); // 紫色
const ImU32 COLOR_CYAN = IM_COL32(0, 255, 255, 255);   // 青色

// 半透明颜色
const ImU32 COLOR_RED_ALPHA = IM_COL32(255, 0, 0, 128);    // 半透明红色
const ImU32 COLOR_BLACK_ALPHA = IM_COL32(0, 0, 0, 128);    // 半透明黑色

namespace gui
{
	// imgui控件状态
	inline bool 初始化 = false;


	inline bool	绘制 = false;
	inline bool 连体婴儿子 = true;

	inline bool 技能显示 = false;
	inline bool 杀手显示 = false;
	inline bool 位置显示 = false;
	inline bool 距离显示 = false;
	inline bool 图腾显示 = false;
	inline bool 发电机显示 = false;
	inline bool 板子显示 = false;
	inline bool 窗口显示 = false;
	inline bool 地窖显示 = false;

	inline bool 类名调试 = false;
	inline bool 帧率显示 = true;
	inline bool 性能查看 = false;


}
