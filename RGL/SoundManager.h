#pragma once

#include <map>
#include <string>

#include <SDL_mixer.h>

namespace rgl
{
	class SoundManager
	{
	private:

		static SoundManager* m_pInstance;

		std::map<std::string, Mix_Chunk*> m_sfxs;
		std::map<std::string, Mix_Music*> m_music;

		SoundManager() { }
		~SoundManager() { }

	public:

		enum SoundType
		{
			MUSIC = 0,
			SFX = 1
		};

		static SoundManager* get();

		void init();
		bool load(std::string filename, std::string id, SoundType soundType);
		void clean();

		void playSound(std::string id, int loops);
		void playMusic(std::string id, int loops);

	};
}