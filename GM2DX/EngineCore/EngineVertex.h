#pragma once
#include <EngineBase/EngineMath.h>
#include "EngineInputLayOutInfo.h"

struct FEngineVertex
{
	// ��� ���ؽ��� �ڽ��� ���̾ƿ� ������ ������ �־�� �Ѵ�.
	friend class EngineVertexInit;
	static UEngineInputLayOutInfo Info;

	float4 POSITION;
	float4 TEXCOORD; // UV���̶�� �Ҹ��� ����� �ؽ�ó�� ���εǴ� ������ �������ݴϴ�.
	float4 COLOR;
};


