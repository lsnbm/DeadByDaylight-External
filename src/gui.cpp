#include "overlay.h"
#include <imgui.h>
#include<windows.h>
#include <thread>


// 将宽字符字符串转换为 UTF-8 的 const char*
static inline __forceinline const char* WideToUtf8(const wchar_t* wstr) {
	static thread_local char buffer[2048];
	if (!wstr || wstr[0] == L'\0') {
		buffer[0] = '\0';
		return buffer;
	}

	int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	if (size <= 0 || size > (int)sizeof(buffer)) {
		buffer[0] = '\0';
		return buffer;
	}
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, size, nullptr, nullptr);
	return buffer;
}

// 辅助：将 std::wstring 转为 UTF-8 const char*
static inline __forceinline const char* WideToUtf8(const std::wstring& wstr) {
	return WideToUtf8(wstr.c_str());
}


void overlay::draw_gui()
{
	ImGui::SetNextWindowSize(ImVec2(600, 400));

	ImGui::Begin("gui", nullptr, ImGuiWindowFlags_NoDecoration);
	{
		ImGui::Text("external overlay by noahswtf");
	}
	ImGui::End();
}
void overlay::DBD_draw_gui() {
    // --------- 自定义样式 ---------
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(8, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.14f, 0.17f, 0.95f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.18f, 0.20f, 0.23f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.17f, 0.20f, 0.90f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.22f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.18f, 0.21f, 0.90f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.27f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.15f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.16f, 0.18f, 0.21f, 0.90f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.26f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.25f, 1.00f);

    // --------- 窗口设置 ---------
    ImGui::SetNextWindowSize(ImVec2(320, 440), ImGuiCond_FirstUseEver);
    ImGui::Begin(WideToUtf8(L"DBD Overlay"), nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // 标题文字
    ImGui::TextColored(ImVec4(0.90f, 0.60f, 0.10f, 1.00f), WideToUtf8(L"黎明杀机 外部绘制"));
    ImGui::Separator();

    // 提示信息
    ImGui::TextWrapped(WideToUtf8(L"提示：按 [INSERT] 隐藏/显示菜单，实现鼠标穿透"));
    ImGui::TextWrapped(WideToUtf8(L"注意：游戏请开启全屏窗口化模式"));
    ImGui::Spacing();

    // --------- 操作按钮 ---------
    ImGui::BeginGroup();
    // 初始化按钮 — 自定义颜色
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.40f, 0.70f, 1.00f));
    if (ImGui::Button(WideToUtf8(L"初始化"), ImVec2(130, 36))) {
        gui::初始化 = !gui::初始化;
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();
    // 退出按钮 — 自定义颜色
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.25f, 0.25f, 1.00f));
    if (ImGui::Button(WideToUtf8(L"退出"), ImVec2(130, 36))) {
        exit(0);
    }
    ImGui::PopStyleColor();
    ImGui::EndGroup();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --------- 折叠面板：基础设置 ---------
    if (ImGui::CollapsingHeader(WideToUtf8(L"基础设置"), ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox(WideToUtf8(L"整体绘制"), &gui::绘制);
        ImGui::Checkbox(WideToUtf8(L"帧率显示"), &gui::帧率显示);
        ImGui::Checkbox(WideToUtf8(L"类名调试"), &gui::类名调试);
    }

    // --------- 折叠面板：角色信息 ---------
    if (ImGui::CollapsingHeader(WideToUtf8(L"角色信息"))) {
        ImGui::Checkbox(WideToUtf8(L"查看技能"), &gui::技能显示);
        ImGui::Checkbox(WideToUtf8(L"杀手位置"), &gui::杀手显示);
        ImGui::Checkbox(WideToUtf8(L"玩家位置"), &gui::位置显示);
        ImGui::Checkbox(WideToUtf8(L"距离提示"), &gui::距离显示);
    }

    // --------- 折叠面板：环境物件 ---------
    if (ImGui::CollapsingHeader(WideToUtf8(L"环境物件"))) {
        ImGui::Columns(2, nullptr, false);
        ImGui::Checkbox(WideToUtf8(L"图腾"), &gui::图腾显示);
        ImGui::Checkbox(WideToUtf8(L"发电机"), &gui::发电机显示);
        ImGui::NextColumn();
        ImGui::Checkbox(WideToUtf8(L"木板"), &gui::板子显示);
        ImGui::Checkbox(WideToUtf8(L"地窖"), &gui::地窖显示);
        ImGui::Columns(1);
    }

    ImGui::End();
}
