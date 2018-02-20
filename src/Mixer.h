#ifndef MIXER_H_
#define MIXER_H_

#include <SDL_mixer.h>
#include <map>
#include <string>

namespace flat2d
{
	/**
	 * This is the game mixer. You can load effects and music and then play them
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Mixer
	{
		private:
			std::map<int, Mix_Chunk*> effects;
			std::map<int, Mix_Music*> music;

		public:
			~Mixer();

			/**
			 * Remove all loaded sounds and music
			 */
			void clearAllSound();

			/**
			 * Load a sound effect
			 * @param id The id you want to give the sound
			 * @param path The path to the resource
			 * @return success or fail
			 */
			bool loadEffect(int, std::string);

			/**
			 * Play a sound effect
			 * @param effect The id that was given when the sound was loaded
			 */
			void playEffect(int effect);

			/**
			 * Load music
			 * @param id The id you want to give the sound
			 * @param path The path to the resource
			 * @return success or fail
			 */
			bool loadMusic(int, std::string);

			/**
			 * Play music
			 * @param music The id that was given when the sound was loaded
			 */
			void playMusic(int music);

			/**
			 * Check if we are playing music
			 * @return true or false
			 */
			bool playingMusic();

			/**
			 * Pause the music
			 */
			void pauseMusic();

			/**
			 * Check if the music is paused
			 * @return true or false
			 */
			bool pausedMusic();

			/**
			 * Resume music
			 */
			void unpauseMusic();

			/**
			 * Stop music
			 */
			void stopMusic();
	};
} // namespace flat2d

#endif // MIXER_H_
