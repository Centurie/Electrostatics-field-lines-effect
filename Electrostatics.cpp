#define MAIN
#ifdef MAIN

#include "raylib.h"
#include <cmath>
#include <algorithm>
#include <cassert>
#include <array>
#include <iostream>

constexpr int screenWidth = 1000;
constexpr int screenHeight = 1000;
constexpr float gravity = 1.8f;

namespace ballSettings
{
	int ballRad{ 50 };
	float ballX{ static_cast<float>(screenWidth) / 2 };
	float ballY{ static_cast<float>(screenHeight) / 2 };
	Color ballColor = RED;
}

int main(void)
{
	SetConfigFlags(FLAG_WINDOW_TOPMOST);
	InitWindow(screenWidth, screenHeight, "Studying Electrostatics with SIM");

	Vector2 ballPos{ ballSettings::ballX, ballSettings::ballY };

	int ballCounter{};
	constexpr int xUpdate{ (screenWidth) / 10 };
	constexpr int yUpdate{ (screenHeight) / 10 };
	std::array<int, 100> chargeBalls{};
	std::array<float, 2> chargeBallPos{ 0,0 };
	chargeBalls.fill(1);

	SetTargetFPS(64);
	HideCursor();
	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_H))
		{
			if (IsCursorHidden()) { ShowCursor(); }
			else HideCursor();
		}
		if (!IsCursorHidden()) { ballPos = GetMousePosition(); }
		if (IsKeyDown(KEY_RIGHT)) { ballPos.x += 3.0f; };
		if (IsKeyDown(KEY_LEFT)) { ballPos.x -= 3.0f; };
		if (IsKeyDown(KEY_UP)) { ballPos.y -= 3.0f; };
		if (IsKeyDown(KEY_DOWN)) { ballPos.y += 3.0f; };

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawCircleV(ballPos, (float)ballSettings::ballRad, ballSettings::ballColor);

		chargeBallPos[0] = 50;
		chargeBallPos[1] = 50;
		ballCounter = 0;
		while (ballCounter < chargeBalls.size())
		{
			if (chargeBallPos[1] > screenHeight)
			{
				std::cout << "Charge ball drawing exceeds max height limit. (Drawn)\n";
			}

			float dirX = chargeBallPos[0] - ballPos.x;
			float dirY = chargeBallPos[1] - ballPos.y;
			float distance = std::sqrt(dirX * dirX + dirY * dirY);

			if (distance > 0.1f)
			{
				float normX = dirX / distance;
				float normY = dirY / distance;

				// Mr. Coloumb's Geni
				constexpr float epsilon0 = 8.854e-12f;     // vaccum permittivity, not Em not Er
				constexpr float k_e = 1.0f / (4.0f * PI * epsilon0);     // Coulomb constant (~8.988e9)
				constexpr float Q = 1.5e-6f;                             // Point charge in Coulombs (1.5 microcoulombs)

				float electricFieldE = k_e * (Q / (distance * distance));

				// Scale down the massive physics value to pixels for visual arrow lengths
				float vectorLength = std::clamp(electricFieldE * 0.003f, 40.0f, 95.0f);
				// --------------------------------------------

				Vector2 vectorStart{ chargeBallPos[0], chargeBallPos[1] };
				Vector2 vectorEnd{
					chargeBallPos[0] + normX * vectorLength,
					chargeBallPos[1] + normY * vectorLength
				};

				Color vectorColor = BLUE;

				// Rendering line vector shaft
				DrawLineEx(vectorStart, vectorEnd, 2.0f, vectorColor);

				float arrowSize = std::clamp(vectorLength * 0.25f, 4.0f, 10.0f); // Arrowhead sizes scale with intensity!
				float perpX = -normY;
				float perpY = normX;

				Vector2 tip = vectorEnd;
				Vector2 baseLeft{
					vectorEnd.x - normX * arrowSize - perpX * (arrowSize * 0.45f),
					vectorEnd.y - normY * arrowSize - perpY * (arrowSize * 0.45f)
				};
				Vector2 baseRight{
					vectorEnd.x - normX * arrowSize + perpX * (arrowSize * 0.45f),
					vectorEnd.y - normY * arrowSize + perpY * (arrowSize * 0.45f)
				};

				DrawTriangle(tip, baseLeft, baseRight, vectorColor);
				DrawCircle((int)chargeBallPos[0], (int)chargeBallPos[1], 3, DARKBLUE);
			}
			else
			{
				DrawCircle((int)chargeBallPos[0], (int)chargeBallPos[1], 3, BLUE);
			}

			chargeBallPos[0] += xUpdate;
			if ((chargeBallPos[0]) >= screenWidth)
			{
				chargeBallPos[1] += yUpdate;
				chargeBallPos[0] = 50;
			}

			ballCounter++;
		}
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

#endif
