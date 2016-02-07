#ifndef VIRTUALGAMESTATECONTROLLER_H_
#define	VIRTUALGAMESTATECONTROLLER_H_

namespace flat2d
{
	class GameData;

	class VirtualGameStateController
	{
		public:
			virtual ~VirtualGameStateController() { }
			virtual void handle(const SDL_Event& event) = 0;
			virtual bool gameStateCheck(GameData *gameData) = 0;
	};
} // namespace flat2d

#endif // VIRTUALGAMESTATECONTROLLER_H_
