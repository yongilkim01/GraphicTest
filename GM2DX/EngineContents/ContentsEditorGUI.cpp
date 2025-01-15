#include "PreCompile.h"
#include "ContentsEditorGUI.h"
#include "EngineCore/EngineCore.h"
#include "EngineCore/EngineGUI.h"

UContentsEditorGUI::UContentsEditorGUI()
{
}

UContentsEditorGUI::~UContentsEditorGUI()
{
}

void UContentsEditorGUI::OnGUI()
{
	std::map<std::string, std::shared_ptr<class ULevel>> Map = UEngineCore::GetAllLevelMap();

	for (std::pair<const std::string, std::shared_ptr<class ULevel>>& Pair : Map)
	{
		// ??����־���µ� why ???????????????
		if (ImGui::Button(Pair.first.c_str()))
		{
			UEngineCore::OpenLevel(Pair.first);
		}
	}

	std::string Txt = UEngineString::AnsiToUTF8("�ȳ��ϼ��� �������� ȣȣȣ");

	ImGui::Text(Txt.c_str());
}




