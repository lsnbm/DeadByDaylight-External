#include "overlay.h"
#include <imgui.h>
#include<windows.h>
#include <thread>


// �����ַ��ַ���ת��Ϊ UTF-8 �� const char*
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

// �������� std::wstring תΪ UTF-8 const char*
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
    // --------- �Զ�����ʽ ---------
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

    // --------- �������� ---------
    ImGui::SetNextWindowSize(ImVec2(320, 440), ImGuiCond_FirstUseEver);
    ImGui::Begin(WideToUtf8(L"DBD Overlay"), nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // ��������
    ImGui::TextColored(ImVec4(0.90f, 0.60f, 0.10f, 1.00f), WideToUtf8(L"����ɱ�� �ⲿ����"));
    ImGui::Separator();

    // ��ʾ��Ϣ
    ImGui::TextWrapped(WideToUtf8(L"��ʾ���� [INSERT] ����/��ʾ�˵���ʵ����괩͸"));
    ImGui::TextWrapped(WideToUtf8(L"ע�⣺��Ϸ�뿪��ȫ�����ڻ�ģʽ"));
    ImGui::Spacing();

    // --------- ������ť ---------
    ImGui::BeginGroup();
    // ��ʼ����ť �� �Զ�����ɫ
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.40f, 0.70f, 1.00f));
    if (ImGui::Button(WideToUtf8(L"��ʼ��"), ImVec2(130, 36))) {
        gui::��ʼ�� = !gui::��ʼ��;
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();
    // �˳���ť �� �Զ�����ɫ
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.25f, 0.25f, 1.00f));
    if (ImGui::Button(WideToUtf8(L"�˳�"), ImVec2(130, 36))) {
        exit(0);
    }
    ImGui::PopStyleColor();
    ImGui::EndGroup();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --------- �۵���壺�������� ---------
    if (ImGui::CollapsingHeader(WideToUtf8(L"��������"), ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox(WideToUtf8(L"�������"), &gui::����);
        ImGui::Checkbox(WideToUtf8(L"֡����ʾ"), &gui::֡����ʾ);
        ImGui::Checkbox(WideToUtf8(L"��������"), &gui::��������);
    }

    // --------- �۵���壺��ɫ��Ϣ ---------
    if (ImGui::CollapsingHeader(WideToUtf8(L"��ɫ��Ϣ"))) {
        ImGui::Checkbox(WideToUtf8(L"�鿴����"), &gui::������ʾ);
        ImGui::Checkbox(WideToUtf8(L"ɱ��λ��"), &gui::ɱ����ʾ);
        ImGui::Checkbox(WideToUtf8(L"���λ��"), &gui::λ����ʾ);
        ImGui::Checkbox(WideToUtf8(L"������ʾ"), &gui::������ʾ);
    }

    // --------- �۵���壺������� ---------
    if (ImGui::CollapsingHeader(WideToUtf8(L"�������"))) {
        ImGui::Columns(2, nullptr, false);
        ImGui::Checkbox(WideToUtf8(L"ͼ��"), &gui::ͼ����ʾ);
        ImGui::Checkbox(WideToUtf8(L"�����"), &gui::�������ʾ);
        ImGui::NextColumn();
        ImGui::Checkbox(WideToUtf8(L"ľ��"), &gui::������ʾ);
        ImGui::Checkbox(WideToUtf8(L"�ؽ�"), &gui::�ؽ���ʾ);
        ImGui::Columns(1);
    }

    ImGui::End();
}
