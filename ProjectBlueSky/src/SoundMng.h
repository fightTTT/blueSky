#pragma once

#include <unordered_map>

class SoundMng
{
public:
	static SoundMng &GetInstance()
	{
		static SoundMng s_Instance;
		return s_Instance;
	}



private:

	SoundMng();
	~SoundMng();
	SoundMng(const SoundMng&);
	void operator=(const SoundMng&) {}

	std::unordered_map<std::string, int> soundMap;
};

