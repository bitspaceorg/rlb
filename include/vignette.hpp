#pragma once
#include "raylib.h"

class Vignette{
	private:
	    Shader shader;
        int height, width, rLoc, blurLoc, colLoc;
        RenderTexture2D vTexture;

	public:
		Vignette(int width, int height);
		void ShaderInit();
		void EnableShader();
		void DisableShader();
        void Draw(bool isOpen);
};
