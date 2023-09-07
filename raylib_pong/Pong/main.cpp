/*
	This code is based on Jones' tutorial on YouTube: https://www.youtube.com/watch?v=LvpS3ILwQNA
*/
#include <iostream>

namespace rl {
	#include "raylib.h"
}

struct PlayableRectangle {
	const rl::Vector2 origin;
	const rl::Vector2 dimentions;

	PlayableRectangle(int originX, int originY, int width, int height)
		: origin{ static_cast<float>(originX), static_cast<float>(originY) }, dimentions{ static_cast<float>(width), static_cast<float>(height) } {
	}
};


struct Paddle {
	enum PaddleType {
		LEFT = 0,
		RIGHT
	};

	float width, height;
	PaddleType type;

	// Position of top left corner
	rl::Vector2 position;
	float speedY;

	Paddle(int width, int height, PaddleType type, float speed = 1)
		: width{ static_cast<float>(width) }, height{ static_cast<float>(height) }, position{ 0.0, 0.0 }, type{ type }, speedY{ speed } {

	}
	// Another way we can do this and the ball too, is having a "positioner" class that will take care of the positioning of the objects
	void positionSidePlayableArea(const PlayableRectangle& rect) {
		switch (type) {
		case PaddleType::RIGHT:
			position.x = rect.origin.x + rect.dimentions.x - 0.0625 * rect.dimentions.x - width;
			break;
		case PaddleType::LEFT:
		default:
			position.x = rect.origin.x + 0.0625 * (rect.dimentions.x);
		}
		position.y = rect.origin.y + rect.dimentions.y / 2 - height / 2;
	}

	void keepInsidePlayableArea(const PlayableRectangle& rect) {
		// Check yAxis only
		if (position.y < rect.origin.y) {
			position.y = rect.origin.y;
			//speedY *= -1; // Leaving this creates a funny behavior, try it out. It's not a bug it's a feature
		}
		if (position.y > rect.origin.y + rect.dimentions.y - height) {
			position.y = rect.origin.y + rect.dimentions.y - height;
			//speedY *= - 1; // Leaving this creates a funny behavior, try it out. It's not a bug it's a feature
		}
	}

	void updatePosition(float deltaTime) {
		position.y += speedY * deltaTime;
	}
};

struct Ball {
	double radius;

	rl::Vector2 position;
	rl::Vector2 speed;


	Ball(int radiusInPixels, float speedX = 0.0, float speedY = 0.0)
		: radius{ static_cast<float>(radiusInPixels) } {
		position = { 0.0, 0.0 };
		speed = { speedX, speedY };
	}

	void positionMiddlePlayableArea(const PlayableRectangle& rect) {
		position.x = (rect.origin.x + rect.dimentions.x) / 2.0;
		position.y = (rect.origin.y + rect.dimentions.y) / 2.0;
	}

	void keepInsidePlayableArea(const PlayableRectangle& rect) {
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

	bool checkCollisionWithPaddle(const Paddle& paddle) {
		return rl::CheckCollisionCircleRec(rl::Vector2{ position.x, position.y }, radius,
			rl::Rectangle{ paddle.position.x,
						   paddle.position.y,
						   paddle.width,
						   paddle.height });
	}

	void handleCollisionWithPaddle(const Paddle& paddle) {
		if (paddle.type == Paddle::LEFT) position.x = paddle.position.x + paddle.width + radius;
		else position.x = paddle.position.x - radius;

		speed.x *= -1;
		speed.y = 500 * (position.y - (paddle.position.y + paddle.height / 2)) / paddle.height / 2;
	}

	void updatePosition(float deltaTime) {
		position.x += speed.x * deltaTime;
		position.y += speed.y * deltaTime;

		// Here I could do bound checking
	}
};

int main() {
	// Window set-up
	const PlayableRectangle gameArea{ 0, 0, 800, 600 };

	rl::InitWindow(gameArea.dimentions.x, gameArea.dimentions.y, "Pong"); // First create an application window
	rl::SetWindowState(rl::FLAG_VSYNC_HINT); // Turn V-sync on

	// Initialize components
	Ball ball{ 5, -500 };
	Paddle leftPaddle{ 10, 100, Paddle::LEFT, 500 };
	Paddle rightPaddle{ 10, 100, Paddle::RIGHT, 500 };

	ball.positionMiddlePlayableArea(gameArea);
	leftPaddle.positionSidePlayableArea(gameArea);
	rightPaddle.positionSidePlayableArea(gameArea);

	const char* winnerText = nullptr;

	while (!rl::WindowShouldClose()) {
		// Update paddle speeds
		if (rl::IsKeyDown(rl::KEY_W)) leftPaddle.speedY = (leftPaddle.speedY > 0) ? -leftPaddle.speedY : leftPaddle.speedY;
		if (rl::IsKeyDown(rl::KEY_S)) leftPaddle.speedY = (leftPaddle.speedY > 0) ? leftPaddle.speedY : -leftPaddle.speedY;
		if (rl::IsKeyDown(rl::KEY_UP)) rightPaddle.speedY = (rightPaddle.speedY > 0) ? -rightPaddle.speedY : rightPaddle.speedY;
		if (rl::IsKeyDown(rl::KEY_DOWN)) rightPaddle.speedY = (rightPaddle.speedY > 0) ? rightPaddle.speedY : -rightPaddle.speedY;

		// Update positions
		ball.updatePosition(rl::GetFrameTime());
		if (rl::IsKeyDown(rl::KEY_W) || rl::IsKeyDown(rl::KEY_S)) leftPaddle.updatePosition(rl::GetFrameTime());
		if (rl::IsKeyDown(rl::KEY_UP) || rl::IsKeyDown(rl::KEY_DOWN)) rightPaddle.updatePosition(rl::GetFrameTime());

		// Handle ball-paddle collisions if any
		if (ball.checkCollisionWithPaddle(leftPaddle)) ball.handleCollisionWithPaddle(leftPaddle);
		if (ball.checkCollisionWithPaddle(rightPaddle)) ball.handleCollisionWithPaddle(rightPaddle);

		// Constraint game objects to playable area
		ball.keepInsidePlayableArea(gameArea);
		leftPaddle.keepInsidePlayableArea(gameArea);
		rightPaddle.keepInsidePlayableArea(gameArea);

		// Draw on screen
		rl::BeginDrawing();

			rl::ClearBackground(rl::RAYWHITE);
			rl::DrawCircle(ball.position.x, ball.position.y, ball.radius, rl::BLACK);
			rl::DrawRectangle((int)leftPaddle.position.x, (int)leftPaddle.position.y, (int)leftPaddle.width, (int)leftPaddle.height, rl::BLACK);
			rl::DrawRectangle((int)rightPaddle.position.x, (int)rightPaddle.position.y, (int)rightPaddle.width, (int)rightPaddle.height, rl::BLACK);
			rl::DrawFPS(10, 10);

		rl::EndDrawing();
	}

	rl::CloseWindow(); // It is a C library, so memory has to be released
	return 0;
}