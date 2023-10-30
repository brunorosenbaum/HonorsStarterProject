
cbuffer ColourBuffer : register(b0)
{
    float4 colour;
};

struct InputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;

};

float4 main(InputType input) : SV_TARGET
{
    //It just returns the color passed into it. 
    return colour;
}