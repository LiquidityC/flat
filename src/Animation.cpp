#include <cassert>
#include "Animation.h"

namespace flat2d
{
	void Animation::start()
	{
		animationTimer.start();
	}

	void Animation::stop()
	{
		animationTimer.stop();
	}

	bool Animation::isRunning() const
	{
		return animationTimer.isStarted();
	}

	const SDL_Rect* Animation::run()
	{
		assert (!clips.empty());

		if (!animationTimer.isStarted()) {
			return &clips[0];
		}

		if (animationTimer.getTicks() >= timestep) {
			clipIndex++;
			animationTimer.stop();
			animationTimer.start();
		}

		clipIndex = clipIndex % clips.size();

         // Stay at the last clip, if runs once
        if (runOnce && clipIndex == clips.size() - 1)
        {
            animationTimer.stop();
        }

		return &clips[clipIndex];
	}

    void Animation::setRunOnce(bool once)
    {
        runOnce = once;
    }

    void Animation::reset(bool alsoStart)
    {
        clipIndex = 0;
        animationTimer.stop();
        if (alsoStart)
        {
            animationTimer.start();
        }
    }
} // namespace flat2d