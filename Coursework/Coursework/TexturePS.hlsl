// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);


cbuffer LightBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 position;
    float3 direction;

};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse, float4 ambient)
{
    float intensity = saturate(dot(normal, -lightDirection));
    float4 colour = saturate(ldiffuse * intensity) + ambient;
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 finalColor = calculateLighting(direction, input.normal, diffuse, ambient);
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    finalColor *= texture0.Sample(Sampler0, input.tex);
    return  finalColor; 
}