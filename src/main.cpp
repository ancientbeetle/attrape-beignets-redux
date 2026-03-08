#include "raylib.h"

#include <vector>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

namespace GameData {
    // Declare values
    static int iGD_FrameTick = 59;

    static int iGD_GameScene = 1;
    static int iGD_GameDifficulty = 1;

    static int iGD_BeignetCooldown = 60;
    static float fGD_BeignetSpeed = 4.0f;

    static float fGD_PlayerX = SCREEN_WIDTH / 2 - 40;
    static float fGD_PlayerY = SCREEN_HEIGHT - 80;
    static bool bGD_PlayerFacingRight = true;

    static int iGD_PlayerScore = 0;
    static int iGD_LostScore = 0;
    static int iGD_BestScore = 0;
};

namespace GameAssets {
    // Declare assets
    static Texture2D tGA_PlayerL = {};
    static Texture2D tGA_PlayerR = {};
    static Texture2D tGA_Player = {};

    static Texture2D tGA_Beignet = {};

    static Texture2D tGA_Difficulty_Easy = {};
    static Texture2D tGA_Difficulty_Med = {};
    static Texture2D tGA_Difficulty_Hard = {};

    static Image iGA_PlayerL = {};
    static Image iGA_PlayerR = {};

    static Image iGA_Beignet = {};

    static Image iGA_Difficulty_Easy = {};
    static Image iGA_Difficulty_Med = {};
    static Image iGA_Difficulty_Hard = {};

    static Sound sGA_Coin = {};
    static Sound sGA_Die = {};
    static Sound sGA_Menu = {};
    static Sound sGA_Menu2 = {};

    static Sound mGA_Menu = {};
    static Sound mGA_InGame = {};
    
    static void Init() {
        // Load Images
        iGA_PlayerL = LoadImage("assets/sprites/sprite_player_l.png");
        iGA_PlayerR = LoadImage("assets/sprites/sprite_player_r.png");
        iGA_Beignet = LoadImage("assets/sprites/sprite_beignet.png");
        iGA_Difficulty_Easy = LoadImage("assets/sprites/sprite_diff_easy.png");
        iGA_Difficulty_Med = LoadImage("assets/sprites/sprite_diff_med.png");
        iGA_Difficulty_Hard = LoadImage("assets/sprites/sprite_diff_hard.png");

        // Load Textures
        tGA_PlayerL = LoadTextureFromImage(iGA_PlayerL);
        tGA_PlayerR = LoadTextureFromImage(iGA_PlayerR);
        tGA_Beignet = LoadTextureFromImage(iGA_Beignet);
        tGA_Difficulty_Easy = LoadTextureFromImage(iGA_Difficulty_Easy);
        tGA_Difficulty_Med = LoadTextureFromImage(iGA_Difficulty_Med);
        tGA_Difficulty_Hard = LoadTextureFromImage(iGA_Difficulty_Hard);

        // And also do ts
        tGA_Player = tGA_PlayerR;

        // Load Sounds
        sGA_Coin = LoadSound("assets/sounds/sound_coin_pickup.wav");
        sGA_Die = LoadSound("assets/sounds/sound_player_die.wav");
        sGA_Menu = LoadSound("assets/sounds/sound_ui_navigate.wav");
        sGA_Menu2 = LoadSound("assets/sounds/sound_ui_navigate2.wav");

        mGA_Menu = LoadSound("assets/music/music_menu.ogg");
        mGA_InGame = LoadSound("assets/music/music_ingame.ogg");
    }

    static void Unload() {
        UnloadTexture(tGA_Player);

        UnloadTexture(tGA_PlayerL);
        UnloadTexture(tGA_PlayerR);

        UnloadTexture(tGA_Beignet);

        UnloadTexture(tGA_Difficulty_Easy);
        UnloadTexture(tGA_Difficulty_Med);
        UnloadTexture(tGA_Difficulty_Hard);

        UnloadImage(iGA_Beignet);
        UnloadImage(iGA_Difficulty_Easy);
        UnloadImage(iGA_Difficulty_Hard);
        UnloadImage(iGA_Difficulty_Med);
        UnloadImage(iGA_PlayerL);
        UnloadImage(iGA_PlayerR);

        UnloadSound(sGA_Coin);
        UnloadSound(sGA_Die);
        UnloadSound(sGA_Menu);
        UnloadSound(sGA_Menu2);
    }
};

class ObjBeignet {
    public:
        float x = 0.0f;
        float y = 0.0f;
        int phase = 1;
};

std::vector<ObjBeignet> vBeignetManager;

namespace PlayerFunctions {
    static void TurnLeft() {
        if (GameData::bGD_PlayerFacingRight) {
            GameAssets::tGA_Player = GameAssets::tGA_PlayerL;
            GameData::bGD_PlayerFacingRight = !GameData::bGD_PlayerFacingRight;
        }
    }

    static void TurnRight() {
        if (!GameData::bGD_PlayerFacingRight) {
            GameAssets::tGA_Player = GameAssets::tGA_PlayerR;
            GameData::bGD_PlayerFacingRight = !GameData::bGD_PlayerFacingRight;
        }
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Attrape-Beignets");
    InitAudioDevice();

    SetTargetFPS(60);

    GameAssets::Init();

    while (!WindowShouldClose())
    {
        if (GameData::iGD_GameScene == 1) {
            BeginDrawing();
            ClearBackground(SKYBLUE);

            if (!IsSoundPlaying(GameAssets::mGA_Menu)) {
                PlaySound(GameAssets::mGA_Menu);
            }

            DrawText("Attrape Beignets", SCREEN_WIDTH / 2 - MeasureText("Attrape Beignets", 40) / 2, SCREEN_HEIGHT / 2 - 80, 40, WHITE);
            DrawText("Appuie sur Espace pour jouer", SCREEN_WIDTH / 2 - MeasureText("Appuie sur Espace pour jouer", 20) / 2, SCREEN_HEIGHT / 2 - 25, 20, WHITE);
            DrawText("Appuie sur Shift pour voir les touches", SCREEN_WIDTH / 2 - MeasureText("Appuie sur Shift pour voir les touches", 20) / 2, SCREEN_HEIGHT / 2, 20, WHITE);
            DrawTextureEx(GameAssets::tGA_Beignet, Vector2{ SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 160 }, 0.0f, 5.0f, WHITE);
            switch (GameData::iGD_GameDifficulty) {
            case 0:
                DrawTextureEx(GameAssets::tGA_Difficulty_Easy, Vector2{ SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT - 130 }, 0.0f, 5.0f, WHITE);
                DrawText("Difficulté: Facile", SCREEN_WIDTH / 2 - MeasureText("Difficulté: Facile", 20) / 2, SCREEN_HEIGHT - 60, 20, WHITE);
                break;
            case 1:
                DrawTextureEx(GameAssets::tGA_Difficulty_Med, Vector2{ SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT - 130 }, 0.0f, 5.0f, WHITE);
                DrawText("Difficulté: Normal", SCREEN_WIDTH / 2 - MeasureText("Difficulté: Normal", 20) / 2, SCREEN_HEIGHT - 60, 20, WHITE);
                break;
            case 2:
                DrawTextureEx(GameAssets::tGA_Difficulty_Hard, Vector2{ SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT - 130 }, 0.0f, 5.0f, WHITE);
                DrawText("Difficulté: Difficile", SCREEN_WIDTH / 2 - MeasureText("Difficulté: Difficile", 20) / 2, SCREEN_HEIGHT - 60, 20, WHITE);
                break;
            }

            if (IsKeyDown(KEY_SPACE)) {
                switch (GameData::iGD_GameDifficulty) {
                case 0:
                    GameData::iGD_BeignetCooldown = 60;
                    GameData::fGD_BeignetSpeed = 4.0f;
                    break;
                case 1:
                    GameData::iGD_BeignetCooldown = 40;
                    GameData::fGD_BeignetSpeed = 6.0f;
                    break;
                case 2:
                    GameData::iGD_BeignetCooldown = 40;
                    GameData::fGD_BeignetSpeed = 8.0f;
                    break;
                }
                GameData::iGD_FrameTick = 59;
                GameData::fGD_PlayerX = SCREEN_WIDTH / 2 - 40;
                GameData::bGD_PlayerFacingRight = true;
                GameAssets::tGA_Player = GameAssets::tGA_PlayerR;
                PlaySound(GameAssets::sGA_Menu2);
                GameData::iGD_GameScene = 2;
                StopSound(GameAssets::mGA_Menu);
            }

            if (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) {
                PlaySound(GameAssets::sGA_Menu2);
                GameData::iGD_GameScene = 4;
            }

            if (IsKeyPressed(KEY_LEFT)) {
                if (GameData::iGD_GameDifficulty > 0) {
                    GameData::iGD_GameDifficulty -= 1;
                    PlaySound(GameAssets::sGA_Menu);
                }
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                if (GameData::iGD_GameDifficulty < 2) {
                    GameData::iGD_GameDifficulty += 1;
                    PlaySound(GameAssets::sGA_Menu);
                }
            }

            EndDrawing();

        }
        else if (GameData::iGD_GameScene == 2) {
            BeginDrawing();
            ClearBackground(SKYBLUE);

            if (!IsSoundPlaying(GameAssets::mGA_InGame)) {
                PlaySound(GameAssets::mGA_InGame);
            }

            if (GameData::iGD_FrameTick < GameData::iGD_BeignetCooldown) {
                GameData::iGD_FrameTick += 1;
            }
            else {
                GameData::iGD_FrameTick = 0;
                vBeignetManager.push_back(ObjBeignet());
            }

            if (IsKeyDown(KEY_LEFT) && GameData::fGD_PlayerX > 0) {
                GameData::fGD_PlayerX -= 8.0f;
                PlayerFunctions::TurnLeft();
            }

            if (IsKeyDown(KEY_RIGHT) && GameData::fGD_PlayerX < SCREEN_WIDTH - 80) {
                GameData::fGD_PlayerX += 8.0f;
                PlayerFunctions::TurnRight();
            }

            for (int i = vBeignetManager.size() - 1; i >= 0; i--) {
                ObjBeignet& BEIGNET = vBeignetManager[i];

                int POSRAND = GetRandomValue(0, 2);

                switch (BEIGNET.phase) {
                case 1:
                    if (POSRAND == 0) {
                        BEIGNET.x = SCREEN_WIDTH / 2 - 240;
                    }
                    else if (POSRAND == 1) {
                        BEIGNET.x = SCREEN_WIDTH / 2 - 40;
                    }
                    else if (POSRAND == 2) {
                        BEIGNET.x = SCREEN_WIDTH / 2 + 170;
                    }
                    BEIGNET.phase = 2;
                    break;
                case 2:
                    DrawTextureEx(GameAssets::tGA_Beignet, Vector2{ BEIGNET.x, BEIGNET.y }, 0.0f, 5.0f, WHITE);
                    BEIGNET.y += GameData::fGD_BeignetSpeed;
                    if (BEIGNET.y >= SCREEN_HEIGHT - 80) {
                        GameData::iGD_LostScore = GameData::iGD_PlayerScore;
                        if (GameData::iGD_PlayerScore > GameData::iGD_BestScore) { GameData::iGD_BestScore = GameData::iGD_PlayerScore; }
                        GameData::iGD_PlayerScore = 0;
                        GameData::iGD_GameScene = 3;
                        PlaySound(GameAssets::sGA_Die);
                        vBeignetManager.clear();
                        StopSound(GameAssets::mGA_InGame);
                    }
                    break;
                }
                if (
                    GameData::fGD_PlayerX < BEIGNET.x + 80 &&
                    GameData::fGD_PlayerX + 80 > BEIGNET.x &&
                    GameData::fGD_PlayerY < BEIGNET.y + 80 &&
                    GameData::fGD_PlayerY + 80 > BEIGNET.y
                    ) {
                    PlaySound(GameAssets::sGA_Coin);
                    GameData::iGD_PlayerScore += 1;
                    vBeignetManager.erase(vBeignetManager.begin() + i);
                }
            }

            DrawTextureEx(GameAssets::tGA_Player, Vector2{ GameData::fGD_PlayerX, GameData::fGD_PlayerY }, 0.0f, 5.0f, WHITE);

            DrawText(TextFormat("Score: %d", GameData::iGD_PlayerScore), 20, 20, 30, WHITE);

            EndDrawing();
        }
        else if (GameData::iGD_GameScene == 3) {
            BeginDrawing();
            ClearBackground(SKYBLUE);

            if (!IsSoundPlaying(GameAssets::mGA_Menu)) {
                PlaySound(GameAssets::mGA_Menu);
            }

            DrawText("Tu as perdu!", SCREEN_WIDTH / 2 - MeasureText("Tu as perdu!", 40) / 2, SCREEN_HEIGHT / 2 - 60, 40, WHITE);
            const char* TEMP_TEXT = TextFormat("Score: %d | Meilleur Score: %d", GameData::iGD_LostScore, GameData::iGD_BestScore);
            DrawText(TEMP_TEXT, SCREEN_WIDTH / 2 - MeasureText(TEMP_TEXT, 20) / 2, SCREEN_HEIGHT / 2 - 10, 20, WHITE);
            DrawText("Appuie sur Espace pour rejouer", SCREEN_WIDTH / 2 - MeasureText("Appuie sur Espace pour rejouer", 20) / 2, SCREEN_HEIGHT / 2 + 20, 20, WHITE);
            DrawTextureEx(GameAssets::tGA_Beignet, Vector2{ SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 40 }, 0.0f, 5.0f, WHITE);
            switch (GameData::iGD_GameDifficulty) {
            case 0:
                DrawTextureEx(GameAssets::tGA_Difficulty_Easy, Vector2{ SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT - 130 }, 0.0f, 5.0f, WHITE);
                DrawText("Difficulté: Facile", SCREEN_WIDTH / 2 - MeasureText("Difficulté: Facile", 20) / 2, SCREEN_HEIGHT - 60, 20, WHITE);
                break;
            case 1:
                DrawTextureEx(GameAssets::tGA_Difficulty_Med, Vector2{ SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT - 130 }, 0.0f, 5.0f, WHITE);
                DrawText("Difficulté: Normal", SCREEN_WIDTH / 2 - MeasureText("Difficulté: Normal", 20) / 2, SCREEN_HEIGHT - 60, 20, WHITE);
                break;
            case 2:
                DrawTextureEx(GameAssets::tGA_Difficulty_Hard, Vector2{ SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT - 130 }, 0.0f, 5.0f, WHITE);
                DrawText("Difficulté: Difficile", SCREEN_WIDTH / 2 - MeasureText("Difficulté: Difficile", 20) / 2, SCREEN_HEIGHT - 60, 20, WHITE);
                break;
            }

            if (IsKeyDown(KEY_SPACE)) {
                switch (GameData::iGD_GameDifficulty) {
                case 0:
                    GameData::iGD_BeignetCooldown = 60;
                    GameData::fGD_BeignetSpeed = 4.0f;
                    break;
                case 1:
                    GameData::iGD_BeignetCooldown = 40;
                    GameData::fGD_BeignetSpeed = 6.0f;
                    break;
                case 2:
                    GameData::iGD_BeignetCooldown = 40;
                    GameData::fGD_BeignetSpeed = 8.0f;
                    break;
                }
                GameData::iGD_FrameTick = 59;
                GameData::fGD_PlayerX = SCREEN_WIDTH / 2 - 40;
                GameData::bGD_PlayerFacingRight = true;
                GameAssets::tGA_Player = GameAssets::tGA_PlayerR;
                PlaySound(GameAssets::sGA_Menu2);
                GameData::iGD_GameScene = 2;
                StopSound(GameAssets::mGA_Menu);
            }

            if (IsKeyPressed(KEY_LEFT)) {
                if (GameData::iGD_GameDifficulty > 0) {
                    GameData::iGD_GameDifficulty -= 1;
                    PlaySound(GameAssets::sGA_Menu);
                }
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                if (GameData::iGD_GameDifficulty < 2) {
                    GameData::iGD_GameDifficulty += 1;
                    PlaySound(GameAssets::sGA_Menu);
                }
            }

            EndDrawing();
        }
        else if (GameData::iGD_GameScene == 4) {
            BeginDrawing();
            ClearBackground(SKYBLUE);

            DrawText("Touches", 32, 32, 40, WHITE);
            DrawText("MENU:\nFlèches: Changer de difficulté\nShift: Ouvrir/fermer le menu de touches\nEspace: Commencer à jouer\n\nJEU:\nFlèches: Bouger", 32, 88, 20, WHITE);

            if (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) {
                GameData::iGD_GameScene = 1;
                PlaySound(GameAssets::sGA_Menu2);
            }

            EndDrawing();
        }
    }

    CloseWindow();

    GameAssets::Unload();

    return 0;
}