#pragma once
#include"../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageBase.hpp"

namespace FPS_n2 {
	class EnemyList : public ListParent<EnemyID> {
	};
	class EnemyData : public DataParent<EnemyID, EnemyList> {
	public:
		EnemyData() noexcept {
			SetDirList("data/enemy/");
		}
		~EnemyData() noexcept {}
	};
};
