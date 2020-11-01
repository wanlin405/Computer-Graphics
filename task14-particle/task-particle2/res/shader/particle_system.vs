#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // 粒子中心位置的坐标 和 粒子大小
layout(location = 2) in vec4 color; // 颜色

out vec2 UV;
out vec4 particlecolor;

uniform vec3 CameraRight_worldspace; //摄像头的右方向
uniform vec3 CameraUp_worldspace; //摄像头的上方向
uniform mat4 VP; // View-Projection 矩阵

void main()
{
	// 粒子大小
	float particleSize = xyzs.w;
	// 粒子中心位置的坐标
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	// 采用公告板技术 让粒子始终朝向摄像头
	// 根据摄像头的右方向、上方向，粒子中心位置的坐标以及粒子的大小，计算出粒子在世界空间顶点的位置
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);

	UV = squareVertices.xy + vec2(0.5, 0.5);
	particlecolor = color;
}

