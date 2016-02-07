#ifndef DELTATIMEMONITOR_H_
#define DELTATIMEMONITOR_H_

namespace flat2d
{
	class DeltatimeMonitor
	{
		private:
			float deltaTime = 1.0;
			int currentTime = 0;
			int oldTime = 0;

		public:
			void updateDeltaTime();
			float getDeltaTime() const;
	};
} // namespace flat2d

#endif // DELTATIMEMONITOR_H_
