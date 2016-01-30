#include "SoundManager.h"
#include "Debugger.h"

namespace rgl
{
	SoundManager* SoundManager::m_pInstance = 0;

	SoundManager* SoundManager::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new SoundManager();

		return m_pInstance;
	}

	void SoundManager::init()
	{
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	}

	bool SoundManager::load(std::string fileName, std::string id, SoundManager::SoundType soundType)
	{
		if (soundType == MUSIC)
		{
			Mix_Music* pMusic = Mix_LoadMUS(fileName.c_str());

			if (pMusic == 0)
			{
				Debugger::get()->log("Could not load music from file \"" + fileName + "\".\n" + Mix_GetError(), Debugger::WARNING);
				return false;
			}

			m_music[id] = pMusic;
		}
		else if (soundType == SFX)
		{
			Mix_Chunk* pChunk = Mix_LoadWAV(fileName.c_str());

			if (pChunk == 0)
			{
				Debugger::get()->log("Could not load SFX from file \"" + fileName + "\".\n" + Mix_GetError(), Debugger::WARNING);
				return false;
			}

			m_sfxs[id] = pChunk;
		}

		return true;
	}

	void SoundManager::clean()
	{
		for (auto music : m_music)
			Mix_FreeMusic(music.second);

		m_music.clear();

		for (auto sfx : m_sfxs)
			Mix_FreeChunk(sfx.second);

		m_sfxs.clear();

		Mix_CloseAudio();
	}

	void SoundManager::playMusic(std::string id, int loops)
	{
		Mix_PlayMusic(m_music[id], loops);
	}

	void SoundManager::playSound(std::string id, int loops)
	{
		Mix_PlayChannel(-1, m_sfxs[id], loops);
	}
}