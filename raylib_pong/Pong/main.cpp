#include <iostream>

/*
	This code is based on Jones tutorial on YouTube: https://www.youtube.com/watch?v=LvpS3ILwQNA
*/

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

struct Ball {
	double radius;

	rl::Vector2 position;
	rl::Vector2 speed;
	

	Ball(int radiusInPixels, float speedX=0.0, float speedY=0.0)
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
};

struct Paddle {
	enum PaddleType {
		LEFT = 0,
		RIGHT
	};

	float width, height;

	// Position of top left corner
	rl::Vector2 position;
	float speedY;

	Paddle(int width, int height, float speed=1)
		: width{ static_cast<float>(width) }, height{ static_cast<float>(height) }, position{ 0.0, 0.0 }, speedY{ speed } {

	}
	// Another way we can do this and the ball too, is having a "positioner" class that will take care of the positioning of the objects
	void positionSidePlayableArea(const PlayableRectangle& rect, const PaddleType& paddleType) {
		switch (paddleType) {
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
			//speedY *= -1; // Leaving this creates a funny behavior
		}
		if (position.y > rect.origin.y + rect.dimentions.y - height) {
			position.y = rect.origin.y + rect.dimentions.y - height;
			//speedY *= - 1; // Leaving this creates a funny behavior
		}
	}
};

int main() {
	// Window set-up
	const PlayableRectangle gameArea{ 0, 0, 800, 600 };

	rl::InitWindow(gameArea.dimentions.x, gameArea.dimentions.y, "Pong"); // First create an application window
	rl::SetWindowState(rl::FLAG_VSYNC_HINT); // Turn V-sync on
	
	// Initialize components
	Ball ball{5, -500};
	Paddle leftPaddle{10, 100, 500};
	Paddle rightPaddle{10, 100, 500};

	ball.positionMiddlePlayableArea(gameArea);
	leftPaddle.positionSidePlayableArea(gameArea, Paddle::LEFT);
	rightPaddle.positionSidePlayableArea(gameArea, Paddle::RIGHT);

	const char* winnerText = nullptr;

	while (!rl::WindowShouldClose()) {
		// Update
		ball.keepInsidePlayableArea(gameArea);
		leftPaddle.keepInsidePlayableArea(gameArea);
		rightPaddle.keepInsidePlayableArea(gameArea);

		ball.position.x += ball.speed.x * rl::GetFrameTime();
		ball.position.y += ball.speed.y * rl::GetFrameTime();

		if (rl::IsKeyDown(rl::KEY_W)) {
			leftPaddle.position.y -= leftPaddle.speedY * rl::GetFrameTime();
		}
		if (rl::IsKeyDown(rl::KEY_S)) {
			leftPaddle.position.y += leftPaddle.speedY * rl::GetFrameTime();
		}
		if (rl::IsKeyDown(rl::KEY_UP)) {
			rightPaddle.position.y -= rightPaddle.speedY * rl::GetFrameTime();
		}
		if (rl::IsKeyDown(rl::KEY_DOWN)) {
			rightPaddle.position.y += rightPaddle.speedY * rl::GetFrameTime();
		}

		if (rl::CheckCollisionCircleRec(rl::Vector2{ ball.position.x, ball.position.y }, ball.radius,
										rl::Rectangle{ leftPaddle.position.x,
													   leftPaddle.position.y,
													   leftPaddle.width,
													   leftPaddle.height })) {
			ball.position.x = leftPaddle.position.x + leftPaddle.width + ball.radius;
			ball.speed.x *= -1;

			ball.speed.y = 500 * (ball.position.y - (leftPaddle.position.y + leftPaddle.height/2)) / leftPaddle.height / 2;
		}
		if (rl::CheckCollisionCircleRec(rl::Vector2{ ball.position.x, ball.position.y }, ball.radius,
										rl::Rectangle{ rightPaddle.position.x,
													   rightPaddle.position.y,
													   rightPaddle.width,
													   rightPaddle.height })) {
			ball.position.x = rightPaddle.position.x - ball.radius;
			ball.speed.x *= -1;

			ball.speed.y = 500 * (ball.position.y - (rightPaddle.position.y + rightPaddle.height / 2)) / rightPaddle.height / 2;
		}


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