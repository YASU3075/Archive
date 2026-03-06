#pragma once

#include <xaudio2.h>
#include "component/Component.h"


class AUDIO : public COMPONENT
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice{};
	BYTE*					m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};


public:
	static void InitMaster();
	static void UninitMaster();

	void Uninit();

	void Load(const char *FileName);
	void Play(bool Loop = false);
	
	//Stop;
	//Fade;
	//3D Sound;

	void SetVolume(float volume);
};

