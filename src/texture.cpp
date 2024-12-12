#include "texture.hpp"
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position,
                        float width, float height, float length, Color color) {
  float x = position.x;
  float y = position.y;
  float z = position.z;
  float texWidth = (float)texture.width;
  float texHeight = (float)texture.height;
  rlSetTexture(texture.id);
  rlBegin(RL_QUADS);
  rlColor4ub(color.r, color.g, color.b, color.a);

  rlNormal3f(0.0f, 0.0f, 1.0f);
  rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
  rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth,
               (source.y + source.height) / texHeight);
  rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
  rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
  rlTexCoord2f(source.x / texWidth, source.y / texHeight);
  rlVertex3f(x - width / 2, y + height / 2, z + length / 2);

  rlNormal3f(0.0f, 0.0f, -1.0f);
  rlTexCoord2f((source.x + source.width) / texWidth,
               (source.y + source.height) / texHeight);
  rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
  rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
  rlTexCoord2f(source.x / texWidth, source.y / texHeight);
  rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
  rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
  rlVertex3f(x + width / 2, y - height / 2, z - length / 2);

  rlNormal3f(0.0f, 1.0f, 0.0f);
  rlTexCoord2f(source.x / texWidth, source.y / texHeight);
  rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
  rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
  rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth,
               (source.y + source.height) / texHeight);
  rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
  rlVertex3f(x + width / 2, y + height / 2, z - length / 2);

  rlNormal3f(0.0f, -1.0f, 0.0f);
  rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
  rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
  rlTexCoord2f(source.x / texWidth, source.y / texHeight);
  rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
  rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
  rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth,
               (source.y + source.height) / texHeight);
  rlVertex3f(x - width / 2, y - height / 2, z + length / 2);

  rlNormal3f(1.0f, 0.0f, 0.0f);
  rlTexCoord2f((source.x + source.width) / texWidth,
               (source.y + source.height) / texHeight);
  rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
  rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
  rlTexCoord2f(source.x / texWidth, source.y / texHeight);
  rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
  rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
  rlVertex3f(x + width / 2, y - height / 2, z + length / 2);

  rlNormal3f(-1.0f, 0.0f, 0.0f);
  rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
  rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth,
               (source.y + source.height) / texHeight);
  rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
  rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
  rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
  rlTexCoord2f(source.x / texWidth, source.y / texHeight);
  rlVertex3f(x - width / 2, y + height / 2, z - length / 2);

  rlEnd();

  rlSetTexture(0);
}

TextureSingleton* TextureSingleton::instance = nullptr;

TextureSingleton::TextureSingleton() {
    texture = LoadTexture("../resource/window_texture.jpg"); 
}

TextureSingleton* TextureSingleton::GetInstance() {
    if (instance == nullptr) {
        instance = new TextureSingleton();
    }
    return instance;
}

Texture2D TextureSingleton::GetTexture() const {
    return texture;
}

TextureSingleton::~TextureSingleton() {
    UnloadTexture(texture);
}
