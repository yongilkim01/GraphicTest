#pragma once
#include <vector>
// �ֽ� std����� �̿��غ��̴ϴ�.
#include "EnginePath.h"

// ���а� ���°� �����е��� ���幮��.

// ���� : 
class UEngineDirectory : public UEnginePath
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineDirectory();
	ENGINEAPI UEngineDirectory(std::string_view _Path);
	ENGINEAPI UEngineDirectory(std::filesystem::path _Path);
	ENGINEAPI ~UEngineDirectory();

	//// delete Function
	//UEngineDirectory(const UEngineDirectory& _Other) = delete;
	//UEngineDirectory(UEngineDirectory&& _Other) noexcept = delete;
	//UEngineDirectory& operator=(const UEngineDirectory& _Other) = delete;
	//UEngineDirectory& operator=(UEngineDirectory&& _Other) noexcept = delete;

	// �ð��� �ʹ� ���ٰ� �ȸ���. ���߿� ��������.
	ENGINEAPI std::vector<class UEngineFile> GetAllFile(bool _IsRecursive, const std::vector<std::string>& _Exts);

	ENGINEAPI std::vector<class UEngineDirectory> GetAllDirectory();

	ENGINEAPI UEngineFile GetFile(std::string_view _FileName);

protected:

private:
	void GetAllFileRecursive(std::filesystem::path _Path, std::vector<class UEngineFile>& _Result, const std::vector<std::string>& _Exts);

};

