#include "Sound.h"

Sound::Sound()
   : hSound(NULL), hChannel(NULL), bLoop(false)
   , bLoopRange(false), range_from(0), range_to(0)
{
}
Sound::~Sound()
{
   if (hChannel)
      hChannel->stop();

   //if (hSound)
   //{
   //   hSound->release();
   //}
}
Sound* Sound::clone()
{
	Sound* pSound = new Sound(*this);
	return pSound;
}
void Sound::Load(LPCSTR lpszFileName, bool bFixed/*=true*/)
{
   //FMOD_RESULT F_API createSound (const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);
   if (bFixed)
   {
      SndDepot->createSound(lpszFileName, FMOD_SOFTWARE, 0, &hSound);
   }
   else
   {
      SndDepot->createStream(lpszFileName, FMOD_SOFTWARE, 0, &hSound);
   }
}
void Sound::Play()
{
   if (!hSound) return;

   for (int i = 0; i < maxchannel; i++)
   {
      FMOD::Channel *pCh;
      SndDepot->getChannel(i, &pCh);
      bool play = false;
      pCh->isPlaying(&play);
      if (!play)
      {
         hChannel = pCh;

         if (bLoopRange)
         {
            SndDepot->playSound(FMOD_CHANNEL_REUSE, hSound, true, &hChannel);
            hChannel->setPosition(range_from, FMOD_TIMEUNIT_MS);
            hChannel->setPaused(false);
         }
         else
         {
            SndDepot->playSound(FMOD_CHANNEL_REUSE, hSound, false, &hChannel);
         }

         break;
      }

   }
   //SndDepot->playSound(FMOD_CHANNEL_FREE, hSound, false, &hChannel);

   FMOD_RESULT ret;
   if (bLoop)
   {
      if (hChannel)
      {
         FMOD_MODE mode;
         hChannel->getMode(&mode);
         mode &= ~FMOD_LOOP_OFF;
         mode |= FMOD_LOOP_NORMAL;
         hChannel->setMode(mode);
         ret = hChannel->setLoopCount(-1);

         if (bLoopRange)
         {
            ret = hChannel->setLoopPoints(range_from, FMOD_TIMEUNIT_MS, range_to, FMOD_TIMEUNIT_MS);
         }
      }
   }

}
void Sound::Stop()
{
   if (hChannel)
      hChannel->stop();
}
void Sound::Pause(bool pause/*=true*/)
{
   if (hChannel)
   {
      hChannel->setPaused(pause);
   }
}
//void Sound::PadeOut()
//{
//   if (hChannel)
//   {
//      hChannel->setVolume(0.5);
//   }
//}
bool Sound::IsPaused()
{
   if (hChannel)
   {
      bool pause;
      hChannel->getPaused(&pause);
      return pause;
   }
   return false;
}
bool Sound::IsPlaying()
{
   if (hChannel)
   {
      bool isplay;
      hChannel->isPlaying(&isplay);
      return isplay;
   }
   return false;
}
void Sound::SetLoop(bool loop/*=true*/)
{
   bLoop = loop;
}
void Sound::SetLoopRange(unsigned int from, unsigned int to)
{
   bLoopRange = true;
   range_from = from;
   range_to = to;
}

SoundManager::SoundManager()
   : system(NULL)
{
   FMOD_RESULT ret = FMOD::System_Create(&system);
   if (ret != FMOD_OK)
   {
      ::MessageBox(NULL, _T("FMOD System Create Failed"), _T("Error"), MB_OK);
   }

   int numdriver;
   ret = system->getNumDrivers(&numdriver);
   if (ret != FMOD_OK)
   {
      ::MessageBox(NULL, _T("FMOD getNumDrivers Failed"), _T("Error"), MB_OK);
   }

   if (numdriver != 0)
   {
      FMOD_CAPS caps;
      FMOD_SPEAKERMODE speakermode;
      system->getDriverCaps(0, &caps, 0, &speakermode);

      system->setSpeakerMode(speakermode);
   }

   ret = system->init(maxchannel, FMOD_INIT_NORMAL, 0);
   if (ret == FMOD_ERR_OUTPUT_CREATEBUFFER)
   {
      system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);

      system->init(maxchannel, FMOD_INIT_NORMAL, 0);
   }
}

SoundManager::~SoundManager()
{
   system->release();
   system->close();
}

FMOD::System* SoundManager::getSystem() const
{
   return system;
}

FMOD::System* SoundManager::operator -> () const
{
   return system;
}
