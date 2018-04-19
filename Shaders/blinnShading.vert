#version 330 core

// These two must perfectly match the structure defined in inc/graphics/Vertex.h
layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;

uniform mat4 ModelViewMatrix; // local->world matrix
uniform mat4 ModelViewProjectionMatrix; // local->NDC matrix [no camera support]

out vec4 worldNorm;
out vec4 worldPos;

void main()
{
  // deal with position and normal in world space
  worldPos = ModelViewMatrix * vec4(vVertex, 1);

  // vec4(vNormal, 0) because we don't want to translate a normal;
  // NOTE: this code is wrong if we support non-uniform scaling
  worldNorm = normalize(ModelViewMatrix * vec4(vNormal, 0));

  // compute the final result of passing this vertex through the transformation
  // pipeline and yielding a coordinate in NDC space
  gl_Position = ModelViewProjectionMatrix * vec4(vVertex, 1);
}