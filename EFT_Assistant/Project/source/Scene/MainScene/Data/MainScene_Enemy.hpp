#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int EnemyID;
	class EnemyList : public ListParent<EnemyID> {
		//í«â¡ê›íË
		void			Set_Sub(const std::string&, const std::string&, const std::vector<std::string>&) noexcept override {}
		void	Load_Sub() noexcept override {
		}
		void	WhenAfterLoad_Sub() noexcept override {}
	};
	class EnemyData : public SingletonBase<EnemyData>, public DataParent<EnemyID, EnemyList> {
	private:
		friend class SingletonBase<EnemyData>;
	private:
		EnemyData() noexcept {
			SetList("data/enemy/");
		}
		~EnemyData() noexcept {}
	};
};