
cbuffer LightBuffer : register(b0)
{
    float4 ambient; 
    float4 diffuse[2]; 
    float4 position[2]; 
    float3 direction; 

};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;

};

float4 calculateDirectionalLighting(float3 lightDirection, float3 normal, float4 ldiffuse, float4 ambient)
{
    float intensity = saturate(dot(normal, -lightDirection));
    float4 colour = saturate(ldiffuse * intensity) + ambient;
    return colour;
}

float4 calculatePositionLighting(float3 worldPos, float3 lightPos, float3 normal, float4 diffuse, float4 ambient)
{
    //Includes attenuation calcs.
    //float attenuationFactors[3] = { 0.5f, 0.125f, 0.0f };
    float distance = length(lightPos - worldPos);
    //float attenuation = calcAttenuation(attenuationFactors[0], attenuationFactors[1], attenuationFactors[2], distance);

    //Light vector is just the light's position - the world's. 
    float3 lightVector = normalize(lightPos - worldPos);
    return calculateDirectionalLighting(-lightVector, normal, diffuse, ambient);
}

float4 main(InputType input) : SV_TARGET
{
    float4 directionalColor = calculateDirectionalLighting(direction, input.normal, diffuse[0], ambient);
    float4 pointColor = calculatePositionLighting(input.worldPosition, position[1], input.normal, diffuse[1], ambient); 
    float4 finalColor = directionalColor + pointColor; 
  return  finalColor; 
}