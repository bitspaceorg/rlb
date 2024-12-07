#include "vignette.hpp"
#include "raylib.h"

Vignette::Vignette(int width, int height) {
    this -> width = width;
    this -> height = height;

    shader = LoadShader(0, TextFormat("../shader/vignette.fs"));

    rLoc = GetShaderLocation(shader, "radius");
    blurLoc = GetShaderLocation(shader, "blur");
    colLoc = GetShaderLocation(shader, "color");
    vTexture = LoadRenderTexture(width, height);

    float radius = 0.3f;
    float blur = 0.0f;
    Vector3 vColor = {0.0f, 0.0f, 0.0f};

    SetShaderValue(shader, rLoc, &radius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, blurLoc, &blur, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, colLoc, &vColor, SHADER_UNIFORM_VEC3);

};

void Vignette::EnableShader() {
    BeginShaderMode(shader);
};

void Vignette::DisableShader() {
    EndShaderMode();
};

void Vignette::Draw(bool isOpen) {
    if (isOpen) {
        DrawTextureRec(vTexture.texture,
                       (Rectangle){0, 0, vTexture.texture.width * 1.0f,
                                   -vTexture.texture.height * 1.0f},
                       (Vector2){0, 0}, BLANK);
        Vector2 centerScreen = {width / 2.0f, height / 2.0f};
        DrawCircleV(centerScreen, 2.0f, RED);
    }
}
