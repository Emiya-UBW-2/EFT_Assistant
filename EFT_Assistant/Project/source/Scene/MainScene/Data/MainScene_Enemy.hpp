#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class EnemyList : public ListParent<EnemyID> {
	};
	class EnemyData : public SingletonBase<EnemyData>, public DataParent<EnemyID, EnemyList> {
	private:
		friend class SingletonBase<EnemyData>;
	private:
		EnemyData() noexcept {
			SetDirList("data/enemy/");
		}
		~EnemyData() noexcept {}
	};
};