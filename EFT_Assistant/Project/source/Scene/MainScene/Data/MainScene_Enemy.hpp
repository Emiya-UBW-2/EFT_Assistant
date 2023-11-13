#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"

namespace FPS_n2 {
	class EnemyList : public ListParent<EnemyID> {
	};
	class EnemyData : public DataParent<EnemyList> {
	public:
		EnemyData() noexcept {
			SetDirList("data/enemy/");
		}
		~EnemyData() noexcept {}
	};
};
