#pragma once

#include "../sqlite/DatabaseManager.h"
#include "../game_objects/Platform.h"
#include "../game_objects/Player.h"
#include "../game_objects/Cannon.h"
#include "../game_objects/EnemySpawner.h"
#include "../game_objects/Goal.h"
#include <vector>
#include <string>


class DBLevel
{
public:
	DBLevel(void);
	~DBLevel(void);

	void FillData(int level_, std::vector<Platform>& platform_);
	void SaveData(std::vector<Platform>& platform_, std::string levelName_);

private:
	DatabaseManager dm;
};

