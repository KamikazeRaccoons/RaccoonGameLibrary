#pragma once

#ifdef RGL_EXPORTS
#define RGL_SOUNDMANAGER_API __declspec(dllexport)
#else
#define RGL_SOUNDMANAGER_API __declspec(dllimport)
#endif

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

		static RGL_SOUNDMANAGER_API SoundManager* get();

		RGL_SOUNDMANAGER_API void init();
		RGL_SOUNDMANAGER_API bool load(std::string filename, std::string id, SoundType soundType);
		RGL_SOUNDMANAGER_API void clean();

		RGL_SOUNDMANAGER_API void playSound(std::string id, int loops);
		RGL_SOUNDMANAGER_API void playMusic(std::string id, int loops);

	};
}