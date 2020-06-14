// NEWBLIT.CPP
//

#include <imgui.h>
#include "FUNCTION.H"
#include "Image.h"

extern byte backbuffer_palette[768];
extern uint8_t g_ColorXlat[16];

void GL_RenderImage(Image_t* image, int x, int y, int width, int height) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);
	ImGui::GetForegroundDrawList()->AddImage((ImTextureID)image->image, mi, ma);
}

void GL_FillRect(int color, int x, int y, int width, int height) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);
	float r = backbuffer_palette[(color * 3) + 0] / 255.0f;
	float g = backbuffer_palette[(color * 3) + 1] / 255.0f;
	float b = backbuffer_palette[(color * 3) + 2] / 255.0f;
	ImGui::GetForegroundDrawList()->AddRectFilled(mi, ma, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)));
}

void GL_DrawText(int color, int x, int y, char* text) {
	ImVec2 pos(x, y);
	color = g_ColorXlat[color % 15];
	float r = backbuffer_palette[(color * 3) + 0] / 255.0f;
	float g = backbuffer_palette[(color * 3) + 1] / 255.0f;
	float b = backbuffer_palette[(color * 3) + 2] / 255.0f;
	ImGui::GetForegroundDrawList()->AddText(pos, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)), text);
}

void GL_DrawLine(int color, int x, int y, int dx, int dy) {
	ImVec2 pos(x, y);
	ImVec2 pos2(dx, dy);
	float r = backbuffer_palette[(color * 3) + 0] / 255.0f;
	float g = backbuffer_palette[(color * 3) + 1] / 255.0f;
	float b = backbuffer_palette[(color * 3) + 2] / 255.0f;
	ImGui::GetForegroundDrawList()->AddLine(pos, pos2, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)));
}