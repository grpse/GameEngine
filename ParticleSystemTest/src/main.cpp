#include <Engine.h>
#include <iostream>

class TestScene : public Scene
{
public:
	virtual void start()
	{
		std::cout << "Scene Started!" << std::endl;
	}

	virtual void update(float dt)
	{
		std::cout << "Scene Update " << dt << " ms" << std::endl;
	}
};

int main() {
	GameLoop loop;
	loop.setScene(new TestScene);
	loop.start();
	return 0;
}