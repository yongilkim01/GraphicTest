#include "PreCompile.h"
#include "HUD.h"
#include "Widget.h"

AHUD::AHUD()
{

}

AHUD::~AHUD()
{
}

void AHUD::UIRender(UEngineCamera* _Camera, float _DeltaTime)
{
	for (std::pair<const int, std::list<std::shared_ptr<class UWidget>>> WidgetPair : Widgets)
	{
		std::list<std::shared_ptr<class UWidget>>& WidgetList = WidgetPair.second;

		for (std::shared_ptr<class UWidget> Widget : WidgetList)
		{
			if (false == Widget->IsActive())
			{
				continue;
			}

			Widget->Tick(_DeltaTime);
		}
	}



	for (std::pair<const int, std::list<std::shared_ptr<class UWidget>>> WidgetPair : Widgets)
	{
		std::list<std::shared_ptr<class UWidget>>& WidgetList = WidgetPair.second;

		for (std::shared_ptr<class UWidget> Widget : WidgetList)
		{
			if (false == Widget->IsActive())
			{
				continue;
			}

			Widget->Render(_Camera, _DeltaTime);
		}
	}

}