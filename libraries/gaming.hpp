#pragma once
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

namespace Gamification{
	constexpr int Streak_Max_Hours = 24;
	using std::string, std::vector, std::domain_error, std::exception;
	class ProgressTracker{
	private:
		int 			Streak;
		int 			level;
		vector<string> 	Achievements;

	public:
		ProgressTracker() {
			Streak = 0;
			level = 1;
		};

		void LevelUp (int amount = 1){
			if(amount > 10){
				throw domain_error("We cannot accept values above 10 for level ups");
			}

			level += amount;
		}

		bool StreakCheck(){
			throw exception(); // NOT IMPLEMENTED
			int previous_hour_since_epoch;
			int current_hour_since_epoch;

			if(current_hour_since_epoch - previous_hour_since_epoch > Streak_Max_Hours)
			{
				return false;
			}

			return true;
		}

		vector<string> getAchivements()
		{
			return Achievements;
		}

		void addAchievement(string Achievement)
		{
			Achievements.push_back(Achievement);
		}
	};
	
	class StatTracker{
		public:
			int health;
			int buffness;
			int stamina;
			int morale;
			int hunger;
		
		StatTracker(int h, int b, int s, int m, int hunger) : health(h), buffness(b), stamina(s), morale(m), hunger(hunger) {};
		StatTracker() : health(100), buffness(0), stamina(2), morale(10), hunger(0) {};
	};
};