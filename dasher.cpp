#include "raylib.h"

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float runningTime;
	float updateTime;
};

int main()
{
	const int windowDimensions[2]{ 512, 380 };

	InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");
	SetTargetFPS(60);

	//Nebula variables
	Texture2D nebulaTex = LoadTexture("textures/12_nebula_spritesheet.png");

	const int nebNum = 6;
	AnimData nebulae[nebNum]{ };

	nebulae[0].pos.x = windowDimensions[0];
	for (int i = 0; i < nebNum; i++)
	{
		nebulae[i].rec.x = 0.0;
		nebulae[i].rec.y = 0.0;
		nebulae[i].rec.width = nebulaTex.width / 8.0;
		nebulae[i].rec.height = nebulaTex.height / 8.0;
		nebulae[i].pos.y = windowDimensions[1] - nebulaTex.height / 8.0;
		nebulae[i].frame = 0;
		nebulae[i].runningTime = 0.0;
		nebulae[i].updateTime = { 1.0 / 16.0 };
		nebulae[i].pos.x = windowDimensions[0] + i * 300;
	}

	// Scarfy variables
	Texture2D scarfy = LoadTexture("textures/scarfy.png");

	AnimData scarfyData
	{
		{ 0.0, 0.0, scarfy.width / 6, scarfy.height },
		{ windowDimensions[0] / 2 - scarfy.width / 12, windowDimensions[1] - scarfy.height },
		0,
		0.0,
		(1.0 / 12.0)
	};

	// Physics variables
	int nebulaVelocity{ -200 };
	int velocity{ 0 };
	bool isInAir{ false };
	const int jumpHeight{ -600 };
	const int gravity{ 1000 };

	while (!WindowShouldClose())
	{
		// Delta Time variable
		const float dT{ GetFrameTime() };

		BeginDrawing();

		ClearBackground(WHITE);

		if (scarfyData.pos.y > windowDimensions[1] - scarfyData.rec.height)
		{
			isInAir = false;
			velocity = 0;
		}
		else
		{
			isInAir = true;
			velocity += gravity * dT;

		}

		if (IsKeyPressed(KEY_SPACE) && !isInAir)
		{
			velocity += jumpHeight;
		}

		// Update scarfy position
		scarfyData.pos.y += velocity * dT;

		if (scarfyData.runningTime >= scarfyData.updateTime && !isInAir)
		{
			scarfyData.rec.x = (scarfyData.frame % 6) * scarfyData.rec.width;
			scarfyData.frame++;
			scarfyData.runningTime = 0;
		}

		// Loop to handle nebula calculations
		for (AnimData& nebula : nebulae)
		{
			nebula.pos.x += nebulaVelocity * dT;
			if (nebula.runningTime >= nebula.updateTime)
			{
				int row = nebula.frame / 8;
				int col = nebula.frame % 8;
				if (row == 7 && col == 4)
				{
					nebula.frame = 0;
				}
				nebula.rec.x = col * nebula.rec.width;
				nebula.rec.y = row * nebula.rec.height;
				nebula.frame++;
				nebula.runningTime = 0;
			}
			DrawTextureRec(nebulaTex, nebula.rec, nebula.pos, RED);
			nebula.runningTime += dT;
		}
		DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

		EndDrawing();

		scarfyData.runningTime += dT;
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebulaTex);
	CloseWindow();
}