/*
	This code is based on Jones' tutorial on YouTube: https://www.youtube.com/watch?v=LvpS3ILwQNA
*/
#include <iostream>

namespace rl {
	#include "raylib.h"
}

struct Vector2 {
	float x, y;
};

struct PlayableRectangle {
	const rl::Vector2 origin;
	const rl::Vector2 dimentions;

	PlayableRectangle(int originX, int originY, int width, int height)
		: origin{ static_cast<float>(originX), static_cast<float>(originY) }, dimentions{ static_cast<float>(width), static_cast<float>(height) } {
	}
};

struct GameObject {
	Vector2 position;
	Vector2 speed;

	GameObject(Vector2 position, Vector2 speed)
		: position{ position }, speed{ speed } {}

	virtual void keepInsidePlayableArea(const PlayableRectangle& rect) = 0;
	virtual void positionInPlayableArea(const PlayableRectangle& rect) = 0;
	virtual void updatePosition(float timeDelta) = 0;
};

struct Paddle : GameObject{
	enum PaddleType {
		LEFT = 0,
		RIGHT
	} type;

	float width, height;

	Paddle(int width, int height, PaddleType type, float speed = 1)
		: width{ static_cast<float>(width) }, height{ static_cast<float>(height) }, type{ type }, GameObject { {0.0, 0.0}, { 0.0, speed }} {}

	// Another way we can do this and the ball too, is having a "positioner" class that will take care of the positioning of the objects
	void positionInPlayableArea(const PlayableRectangle& rect) override {
		switch (type) {
		case PaddleType::RIGHT:
			position.x = rect.origin.x + rect.dimentions.x - (0.0625 * rect.dimentions.x) - width/2;
			break;
		case PaddleType::LEFT:
		default:
			position.x = rect.origin.x + (0.0625 * rect.dimentions.x) + width/2;
		}
		position.y = rect.origin.y + rect.dimentions.y / 2;
	}

	void keepInsidePlayableArea(const PlayableRectangle& rect) override {
		// Check y axis only, paddle can't mode in x axis
		// Top edge
		if (position.y - height/2 < rect.origin.y) {
			position.y = rect.origin.y + height/2;
			//speed.y *= -1; // Leaving this creates a funny behavior, try it out. It's not a bug it's a feature
		}
		// Bottom edge
		if (position.y + height/2 > rect.origin.y + rect.dimentions.y) {
			position.y = rect.origin.y + rect.dimentions.y - height/2;
			//speed.y *= - 1; // Leaving this creates a funny behavior, try it out. It's not a bug it's a feature
		}
	}

	void updatePosition(float deltaTime) override {
		position.y += speed.y * deltaTime;
	}
};

struct Ball : GameObject {
	float radius;

	Ball(int radiusInPixels, float speedX = 0.0, float speedY = 0.0)
		: radius{ static_cast<float>(radiusInPixels) }, GameObject{ {0.0, 0.0}, {speedX, speedY} } {}

	void positionInPlayableArea(const PlayableRectangle& rect) override {
		position.x = (rect.origin.x + rect.dimentions.x) / 2.0;
		position.y = (rect.origin.y + rect.dimentions.y) / 2.0;
	}

	void keepInsidePlayableArea(const PlayableRectangle& rect) override {
		// Check xAxis
		if (position.x - radius < rect.origin.x) {
			position.x = rect.origin.x + radius;
			speed.x *= -1;
		}
		if (position.x + radius > rect.origin.x + rect.dimentions.x) {
			position.x = rect.origin.x + rect.dimentions.x - radius;
			speed.x *= -1;
		}
		if (position.y - radius < rect.origin.y) {
			position.y = rect.origin.y + radius;
			speed.y *= -1;
		}
		if (position.y + radius > rect.origin.y + rect.dimentions.y) {
			position.y = rect.origin.y + rect.dimentions.y - radius;
			speed.y *= -1;
		}
	}

	void handleCollisionWithPaddle(const Paddle& paddle) {
		if (paddle.type == Paddle::LEFT) position.x = paddle.position.x + paddle.width / 2 + radius;
		else position.x = paddle.position.x - paddle.width / 2 - radius;

		speed.x *= -1;
		speed.y = 500 * (position.y - paddle.position.y) / paddle.height / 2;
	}

	void updatePosition(float deltaTime) override {
		position.x += speed.x * deltaTime;
		position.y += speed.y * deltaTime;

		// Here I could do bound checking
	}
};

struct PongGame {
	const PlayableRectangle& gameArea;
	Ball& ball;
	Paddle& leftPaddle;
	Paddle& rightPaddle;

	PongGame(const PlayableRectangle& gameArea, Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle)
		: gameArea(gameArea), ball(ball), leftPaddle(leftPaddle), rightPaddle(rightPaddle) {}

	virtual void initialize() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual bool end() = 0;
	virtual void close() = 0;

private:
	virtual void manageCollitions() = 0;
	virtual void constrainGameObjectsToGameArea() = 0;
};

struct PongGameDesktop : PongGame{
	PongGameDesktop(const PlayableRectangle& gameArea, Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle)
		: PongGame{gameArea, ball, leftPaddle, rightPaddle} {}

	void initialize() override {
		rl::InitWindow(gameArea.dimentions.x, gameArea.dimentions.y, "Pong"); // First create an application window
		rl::SetWindowState(rl::FLAG_VSYNC_HINT); // Turn V-sync on

		ball.positionInPlayableArea(gameArea);
		leftPaddle.positionInPlayableArea(gameArea);
		rightPaddle.positionInPlayableArea(gameArea);
	}

	void update() override {
		// Update paddle speeds
		if (rl::IsKeyDown(rl::KEY_W)) leftPaddle.speed.y = (leftPaddle.speed.y > 0) ? -leftPaddle.speed.y : leftPaddle.speed.y;
		if (rl::IsKeyDown(rl::KEY_S)) leftPaddle.speed.y = (leftPaddle.speed.y > 0) ? leftPaddle.speed.y : -leftPaddle.speed.y;
		if (rl::IsKeyDown(rl::KEY_UP)) rightPaddle.speed.y = (rightPaddle.speed.y > 0) ? -rightPaddle.speed.y : rightPaddle.speed.y;
		if (rl::IsKeyDown(rl::KEY_DOWN)) rightPaddle.speed.y = (rightPaddle.speed.y > 0) ? rightPaddle.speed.y : -rightPaddle.speed.y;

		// Update positions
		ball.updatePosition(rl::GetFrameTime());
		if (rl::IsKeyDown(rl::KEY_W) || rl::IsKeyDown(rl::KEY_S)) leftPaddle.updatePosition(rl::GetFrameTime());
		if (rl::IsKeyDown(rl::KEY_UP) || rl::IsKeyDown(rl::KEY_DOWN)) rightPaddle.updatePosition(rl::GetFrameTime());

		manageCollitions(); // ball-paddle collitions

		constrainGameObjectsToGameArea();
	}

	void render() override {
		// Draw on screen
		rl::BeginDrawing();
			rl::ClearBackground(rl::RAYWHITE);
			rl::DrawCircle(ball.position.x, ball.position.y, ball.radius, rl::BLACK);
			rl::DrawRectangle((int)(leftPaddle.position.x - leftPaddle.width / 2), (int)(leftPaddle.position.y - leftPaddle.height / 2), (int)leftPaddle.width, (int)leftPaddle.height, rl::BLACK);
			rl::DrawRectangle((int)(rightPaddle.position.x - rightPaddle.width / 2), (int)(rightPaddle.position.y - rightPaddle.height / 2), (int)rightPaddle.width, (int)rightPaddle.height, rl::BLACK);
			rl::DrawFPS(5, 5);
		rl::EndDrawing();
	}

	bool end() override {
		return rl::WindowShouldClose();
	}

	void close() override {
		rl::CloseWindow(); // It is a C library, so memory has to be released
	}

private:
	void manageCollitions() override {
		if (checkBallCollisionWithPaddle(Paddle::LEFT)) handleBallCollisionWithPaddle(Paddle::LEFT);
		if (checkBallCollisionWithPaddle(Paddle::RIGHT)) handleBallCollisionWithPaddle(Paddle::RIGHT);
	}

	bool checkBallCollisionWithPaddle(Paddle::PaddleType paddleType) {
		Paddle& paddle = (paddleType == Paddle::LEFT) ? leftPaddle : rightPaddle;

		return rl::CheckCollisionCircleRec(rl::Vector2{ ball.position.x, ball. position.y }, ball.radius,
			rl::Rectangle{ paddle.position.x - paddle.width / 2,
						   paddle.position.y - paddle.height / 2,
						   paddle.width,
						   paddle.height });
	}

	void handleBallCollisionWithPaddle(Paddle::PaddleType type) {
		Paddle& paddle = (type == Paddle::LEFT) ? leftPaddle : rightPaddle;
		float xOffset = (type == Paddle::LEFT) ? paddle.width / 2 + ball.radius : -paddle.width / 2 - ball.radius;

		ball.position.x = paddle.position.x + xOffset;
		ball.speed.x *= -1;
		ball.speed.y = 500 * (ball.position.y - paddle.position.y) / paddle.height / 2;
	}

	void constrainGameObjectsToGameArea() override {
		ball.keepInsidePlayableArea(gameArea);
		leftPaddle.keepInsidePlayableArea(gameArea);
		rightPaddle.keepInsidePlayableArea(gameArea);
	}
};

int main() {
	// Window set-up
	const PlayableRectangle gameArea{ 0, 0, 800, 600 };

	// Initialize components
	Ball ball{ 5, -500 };
	Paddle leftPaddle{ 10, 100, Paddle::LEFT, 500 };
	Paddle rightPaddle{ 10, 100, Paddle::RIGHT, 500 };

	PongGameDesktop game{gameArea, ball, leftPaddle, rightPaddle};

	game.initialize();

	while (!game.end()) {
		game.update();

		game.render();
	}

	game.close();
	return 0;
}