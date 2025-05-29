#version 460 core
 

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;



in vec3 vPos[];
in vec3 vertexColor[]; 

out vec3 gNormal;
out vec3 gFragPos;
out vec3 gVertexColor;


void main() {
    vec3 edge1 = vPos[1] - vPos[0];
    vec3 edge2 = vPos[2] - vPos[0];
    vec3 faceNormal = normalize(cross(edge1, edge2));


    for (int i = 0; i < 3; ++i) 
    {
        gNormal = faceNormal; 
        gFragPos = vPos[i];
        gVertexColor = vertexColor[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}