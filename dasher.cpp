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
	const int wWidth{ 800 };
	const int wHeight{ 600 };

	InitWindow(wWidth, wHeight, "Dapper Dasher");
	SetTargetFPS(60);

	//Nebula variables
	Texture2D nebulaTex = LoadTexture("textures/12_nebula_spritesheet.png");

	AnimData nebData
	{
		{ 0.0, 0.0, nebulaTex.width / 8, nebulaTex.height / 8},
		{ wWidth, wHeight - nebulaTex.height / 8 },
		0,
		0.0,
		(1.0 / 12.0)
	};

	AnimData neb2Data
	{
		{ 0.0, 0.0, nebulaTex.width / 8, nebulaTex.height / 8 },
		{ wWidth + 300, wHeight - nebulaTex.height / 8 },
		0,
		0.0,
		(1.0 / 16.0)
	};


	// Scarfy variables
	Texture2D scarfy = LoadTexture("textures/scarfy.png");

	AnimData scarfyData
	{
		{ 0.0, 0.0, scarfy.width / 6, scarfy.height },
		{ wWidth / 2 - scarfy.width / 12, wHeight - scarfy.height },
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

		if (scarfyData.pos.y > wHeight - scarfyData.rec.height)
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

		// Update nebula position
		nebData.pos.x += nebulaVelocity * dT;
		neb2Data.pos.x += nebulaVelocity * dT;

		// Update scarfy position
		scarfyData.pos.y += velocity * dT;

		if (scarfyData.runningTime >= scarfyData.updateTime && !isInAir)
		{
			scarfyData.rec.x = (scarfyData.frame % 6) * scarfyData.rec.width;
			scarfyData.frame++;
			scarfyData.runningTime = 0;
		}

		if (nebData.runningTime >= nebData.updateTime)
		{
			int row = nebData.frame / 8;
			int col = nebData.frame % 8;
			if (row == 7 && col == 4)
			{
				nebData.frame = 0;
			}
			nebData.rec.x = col * nebData.rec.width;
			nebData.rec.y = row * nebData.rec.height;
			nebData.frame++;
			nebData.runningTime = 0;
		}

		if (neb2Data.runningTime >= neb2Data.updateTime)
		{
			int row = neb2Data.frame / 8;
			int col = neb2Data.frame % 8;
			if (row == 7 && col == 4)
			{
				neb2Data.frame = 0;
			}
			neb2Data.rec.x = col * neb2Data.rec.width;
			neb2Data.rec.y = row * neb2Data.rec.height;
			neb2Data.frame++;
			neb2Data.runningTime = 0;
		}
		DrawTextureRec(nebulaTex, nebData.rec, nebData.pos, WHITE);
		DrawTextureRec(nebulaTex, neb2Data.rec, neb2Data.pos, RED);
		DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

		EndDrawing();

		scarfyData.runningTime += dT;
		nebData.runningTime += dT;
		neb2Data.runningTime += dT;
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebulaTex);
	CloseWindow();
}