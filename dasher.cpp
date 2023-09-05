#include "raylib.h"

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float runningTime;
	float updateTime;
};

bool isOnGround(const AnimData& data, int windowHeight)
{
	return data.pos.y >= windowHeight - data.rec.height;
}

void updateAnimData(AnimData& data, float deltaTime, int maxFrameRow, int maxFrameCol, int lastCol)
{
	data.runningTime += deltaTime;
	if (data.runningTime >= data.updateTime)
	{
		int row = data.frame / maxFrameRow;
		int col = data.frame % maxFrameCol;
		if (row == maxFrameRow - 1 && col == lastCol - 1)
		{
			data.frame = 0;
		}
		data.rec.x = col * data.rec.width;
		data.rec.y = row * data.rec.height;
		data.frame++;
		data.runningTime = 0.0;
	}
}

int main()
{
	const int windowDimensions[2]{ 512, 380 };

	InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");
	SetTargetFPS(60);

	// Nebula variables
	Texture2D nebulaTex = LoadTexture("textures/12_nebula_spritesheet.png");
	const int nebNum = 10;
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

	float finishLine{ nebulae[nebNum - 1].pos.x };

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

	// Background Texture Variables
	Texture2D background = LoadTexture("textures/far-buildings.png");
	Texture2D midground = LoadTexture("textures/back-buildings.png");
	Texture2D foreground = LoadTexture("textures/foreground.png");
	float bgX{ };
	float mgX{ };
	float fgX{ };

	// Collision detection boolean
	bool collision{ };

	// Render loop
	while (!WindowShouldClose())
	{
		// Delta Time variable
		const float dT{ GetFrameTime() };

		BeginDrawing();
		ClearBackground(WHITE);

		// Draw background
		bgX -= 20 * dT;

		mgX -= 40 * dT;

		fgX -= 80 * dT;

		Vector2 bg1Pos{ bgX, 0.0 };
		Vector2 bg2Pos{ bgX + background.width*2, 0.0 };

		Vector2 mg1Pos{ mgX, 0.0 };
		Vector2 mg2Pos{ mgX + midground.width * 2, 0.0 };

		Vector2 fg1Pos{ fgX + 0.0 };
		Vector2 fg2Pos{ fgX + foreground.width * 2, 0.0 };

		DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
		DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

		DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
		DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

		DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
		DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

		if (bgX <= -background.width * 2)
		{
			bgX = 0.0;
		}

		if (mgX <= -midground.width * 2)
		{
			mgX = 0.0;
		}

		if (fgX <= -foreground.width * 2)
		{
			fgX = 0.0;
		}

		if (isOnGround(scarfyData, windowDimensions[1]))
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
			updateAnimData(scarfyData, dT, 1, 6, 6);
		}

		// Update finish line position
		finishLine += nebulaVelocity * dT;

		for (const AnimData& nebula : nebulae)
		{
			float pad{ 50 };
			Rectangle nebRec
			{
				nebula.pos.x + pad,
				nebula.pos.y + pad,
				nebula.rec.width - pad * 2,
				nebula.rec.height - pad * 2
			};
			Rectangle scarfyRec
			{
				scarfyData.pos.x,
				scarfyData.pos.y,
				scarfyData.rec.width,
				scarfyData.rec.height
			};

			if (CheckCollisionRecs(nebRec, scarfyRec))
			{
				collision = true;
			}
		}

		// Loop to handle nebula calculations
		for (AnimData& nebula : nebulae)
		{
			nebula.pos.x += nebulaVelocity * dT;
			updateAnimData(nebula, dT, 8, 8, 5);
			nebula.runningTime += dT;
		}

		// Collision detected, game over
		if (collision)
		{
			DrawText("Game Over!", windowDimensions[0] / 3, windowDimensions[1] / 2, 36, RED);
		}
		else if (scarfyData.pos.x > finishLine)
		{
			DrawText("You Win!", windowDimensions[0] / 3, windowDimensions[1] / 2, 36, GREEN);
		}
		else
		{
			for (const AnimData& nebula : nebulae)
			{
				DrawTextureRec(nebulaTex, nebula.rec, nebula.pos, WHITE);
			}
			DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
		}
		EndDrawing();

		scarfyData.runningTime += dT;
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebulaTex);
	UnloadTexture(background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
	CloseWindow();
}