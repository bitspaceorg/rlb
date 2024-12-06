#pragma once
#include <vector>
#include "raylib.h"
#include "rlights.h"
class LightLoader{
	private:
	std::vector<Light> lights;
	Shader shader;
	unsigned int ambientLight;
	unsigned int glsl_version;
	public:
		/*
		 * @param int n - The Number of Lights*/
		LightLoader(int,unsigned int);
		/*
		 * @brief Loads the shader similar to init call before render loop*/
		void ShaderInit();
		/*
		 * @brief Creates the light at the initialized vector
		 *
		 * @param int idx - The index of the light starts at 0
		 * @param Vector3 pos - Position of the light
		 * @param Vector3 direction - Direction of light projection
		 * @param  Color type of raylib color of the light
		 * */
		void InitializeLights(int,Vector3,Vector3,Color);

		/*
		 * @brief Enable the Light Shader*/
		void EnableShader();

		/*
		 * @brief Disables the Light Shader*/
		void DisableShader();
		
		/* @brief  updates light acc to camera ambience
		 * Use inside render loop
		 *
		 * @params Raylib 3d camera
		 * */
		void UpdateLight(Camera3D);

		/*
		 * @brief Enables Debug Mode and draws the light as sphere*/
		void EnableDebug();
};
