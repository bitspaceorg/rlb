#pragma once
#include "raylib.h"
#include "rlgl.h"
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position,
                        float width, float height, float length, Color color);
;
class TextureSingleton {
private:
  static TextureSingleton *instance;
  Texture2D texture;
	Texture2D brickTexture;

  TextureSingleton();

public:
  TextureSingleton(const TextureSingleton &) = delete;
  TextureSingleton &operator=(const TextureSingleton &) = delete;

  static TextureSingleton *GetInstance();

  Texture2D GetTexture() const;
	
  Texture2D GetTextureWall() const;


  ~TextureSingleton();
};
