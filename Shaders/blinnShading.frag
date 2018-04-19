#version 330 core

const int MaxLights = 8; // maximum possible lights this shader supports

// only support directional lights for now
struct Light
{
  vec4 direction; // direction the light is directed
  vec4 position;
  vec4 ambient; // ambient light cast onto objects
  vec4 diffuse; // diffuse light cast onto objects
  vec4 specular;
  int type;
};

uniform struct
{
  vec4 ambient;
  float spotInner;
  float spotOuter;
  int spotFalloff;
  float distC1;
  float distC2;
  float distC3;
  vec4 fogColor;
  float fogNear;
  float fogFar;
} Global;

uniform Light Lights[MaxLights]; // support UP TO 8 lights
uniform int LightCount; // number of lights enabled THIS ROUND

// represents material properties of the surface passed by the application
uniform struct
{
  vec4 ambient; // ambient color of the surface/how much ambient light to absorb
  vec4 diffuse; // diffuse color of the surface/how much diffuse light to absorb
  vec4 specular;
  vec4 emissive;
  int shininess;
} Material;

in vec4 worldNorm;
in vec4 worldPos;

layout(location = 0) out vec4 vFragColor;

vec4 computeLightingTerm(in int lightIdx, in vec4 worldNormal)
{
  // grab light
  Light light = Lights[lightIdx];

  // light vector points from the surface toward the light (opposite light dir.)
  vec4 lightVec = vec4(0);

  if(light.type > 0)
    lightVec = light.position - worldPos;
  else
    lightVec = -light.direction;
    
  float dl = length(lightVec);
  float spotlightEffect = 1;
  vec4 viewVec = vec4(0, 0, -2, 1) - worldPos;
  vec4 H = normalize(viewVec + lightVec);
  viewVec = normalize(viewVec);
  lightVec = normalize(lightVec);
  // ambient contribution from the light is always constant
  vec4 ambient = light.ambient * Material.ambient;
  
  vec4 diffuse = light.diffuse * Material.diffuse * max(dot(worldNormal, lightVec), 0);
  
  vec4 reflection = normalize(2 * dot(worldNormal, lightVec) * worldNormal - lightVec);
  
  vec4 specular = vec4(0);
  specular = light.specular * Material.specular * pow(max(dot(worldNormal, H), 0), Material.shininess);
  
  float att = min(1f / (Global.distC1 + dl * Global.distC2 + dl * dl * Global.distC3), 1.0);
  
  if(light.type == 2)
  {
    spotlightEffect = (dot(-lightVec, normalize(light.direction)) - cos(Global.spotOuter)) 
                        / (cos(Global.spotInner) - cos(Global.spotOuter));
    spotlightEffect = pow(spotlightEffect, Global.spotFalloff);
    spotlightEffect = clamp(spotlightEffect, 0, 1);
  }
  return att * ambient + att * spotlightEffect * (diffuse + specular); // total contribution from this light
}

vec4 computeSurfaceColor(in vec4 worldNormal)
{
  // Phong: total contribution of light is sum of all individual light contribs.
  vec4 color = Material.emissive + Global.ambient * Material.ambient; // no light = black
  for (int i = 0; i < LightCount; ++i)
    color += computeLightingTerm(i, worldNormal); // contribution of light i
  
  vec4 viewVec = normalize(vec4(0, 0, 0, 1) - worldPos);
  float S = (Global.fogFar - length(viewVec)) / (Global.fogFar - Global.fogNear);
  S = clamp(S, 0, 1);
  return S * color + (1 - S) * Global.fogColor; // contribution from all lights onto surface
}

void main()
{
  // compute the contribution of lights onto this vertex and interpolate that
  // color value across the surface of the polygon
  vFragColor = computeSurfaceColor(normalize(worldNorm));
}