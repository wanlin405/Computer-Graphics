#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
	//这是因为天空盒会在之后覆盖所有的场景中其他物体。我们需要耍个花招让深度缓冲相信天空盒的深度缓冲有着最大深度值1.0，如此只要有个物体存在深度测试就会失败，看似物体就在它前面了
	//透视除法（perspective division）是在顶点着色器运行之后执行的，把gl_Position的xyz坐标除以w元素。我们从深度测试教程了解到除法结果的z元素等于顶点的深度值。利用这个信息，我们可以把输出位置的z元素设置为它的w元素，这样就会导致z元素等于1.0了，因为，当透视除法应用后，它的z元素转换为w/w = 1.0：
    gl_Position = pos.xyww;
}  