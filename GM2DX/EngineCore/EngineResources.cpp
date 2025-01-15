#include "PreCompile.h"
#include "EngineResources.h"
#include "EngineTexture.h"


// EngineCoreDll�� �����Դϴ�.


std::shared_ptr<UEngineResources> UEngineResources::Find(std::string_view _ResName, std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == ResMap[_ResName.data()].contains(UpperString))
	{
		return nullptr;
	}

	return ResMap[_ResName.data()][UpperString];
}

void UEngineResources::PushRes(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	//       �ؽ�ó��                  Player.png�� ����ִ��� Ȯ��.
	if (true == ResMap[_Info.data()].contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� ���ҽ��� �� �ε� �Ϸ��� �߽��ϴ�" + std::string(_Info.data()) + "  " + _Name.data());
		return;
	}

	// �̰� ȣ���� ���� �������� ������ �ȴ�.

	_Res->SetName(UpperName);
	_Res->Path = _Path;
	ResMap[_Info.data()].insert({UpperName, _Res });

	return;
}