#pragma once

#include <unordered_map>

#define SOUND_ID(X) (SoundMng::GetInstance().GetID(X))

class SoundMng
{
public:
	static SoundMng &GetInstance()
	{
		static SoundMng s_Instance;
		return s_Instance;
	}

	const int GetID(std::string filePath);

private:

	SoundMng();
	~SoundMng();
	SoundMng(const SoundMng&);
	void operator=(const SoundMng&) {}

	std::unordered_map<std::string, int> soundMap;
};

