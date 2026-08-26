#pragma once

#include "Core/Object.h"
#include <map>

class MapManager : public Object
{
public:
	MapManager();
	virtual ~MapManager();
	MapManager(const MapManager&) = delete;
	MapManager(MapManager&&) = delete;
	MapManager& operator=(const MapManager&) = delete;
	MapManager& operator=(MapManager&&) = delete;

private:
	Weak<class Level> _OwnerLevel; // 맵 액터의 실제 소유자는 Level

	std::map<std::string, Weak<class Actor>> _Maps; // 생성된 맵 액터를 이름으로 다시 찾기 위한 참조 
													// 생성되고 이동된 맵은 Destroy 되지않고 비활성화 방식으로 기억하면서 다시 찾기 위한 저장소
	Weak<class Actor> _CurrentMap; // 현재 Tick, Collision, Render 가 활성화된 맵 액터

public:
	bool Init(Ptr<class Level> OwnerLevel);

	bool ChangeMap(const std::string& MapName);

	virtual void Destroy() override;
};

