#define RLIGHTS_IMPLEMENTATION
#include "lights.hpp"

LightLoader::LightLoader(int n, unsigned int glsl_version) {
  this->lights.resize(n);
  this->glsl_version=glsl_version;
}

void LightLoader::ShaderInit() {
  this->shader = LoadShader(TextFormat("../shader/lighting.vs", this->glsl_version),
                            TextFormat("../shader/lighting.fs", this->glsl_version));
  this->shader.locs[SHADER_LOC_VECTOR_VIEW] =
      GetShaderLocation(this->shader, "viewPos");

  // sets ambient light
  this->ambientLight = GetShaderLocation(this->shader, "ambient");
  SetShaderValue(this->shader, this->ambientLight,
                 (float[4]){1.0f, 1.0f, 1.0f, 1.0f}, SHADER_UNIFORM_VEC4);
}

void LightLoader::UpdateLight(Camera3D camera) {
  float cameraPos[3] = {camera.position.x, camera.position.y,
                        camera.position.z};
  SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos,
                 SHADER_UNIFORM_VEC3);
}

void LightLoader::EnableShader() { BeginShaderMode(this->shader); }

void LightLoader::DisableShader() { UnloadShader(this->shader); }

void LightLoader::EnableDebug() {
  for (int i = 0; i < this->lights.size(); i++) {
    if (this->lights[i].enabled)
      DrawSphereEx(this->lights[i].position, 0.2f, 8, 8, this->lights[i].color);
    else
      DrawSphereWires(this->lights[i].position, 0.2f, 8, 8,
                      ColorAlpha(this->lights[i].color, 0.3f));
  }
}

void LightLoader::InitializeLights(int idx, Vector3 pos, Vector3 projection,
                                   Color color) {
  this->lights[idx] =
      CreateLight(LIGHT_POINT, pos, projection, color, this->shader);
}
