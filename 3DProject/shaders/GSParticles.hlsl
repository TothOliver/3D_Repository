cbuffer CameraPosition : register(b0)
{
    float4 camPosition;
};

cbuffer Camera : register(b1)
{
    float4x4 vp;
};

struct GSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

[maxvertexcount(6)]
void main(point float3 input[1] : POSITION, inout TriangleStream<GSOutput> output)
{
    // Calculate front vector using input and camera position
    // Use front vector along with defaulted up vector to calculate right vector
    // Use front vector along with right vector to calculate actual up vector
    // Scale right and up at end based on preference
    
    float3 front = normalize(camPosition.xyz - input[0]);
    
    float3 right = normalize(cross(front, float3(0, 1, 0))) * 0.01;
    
    float3 up = normalize(cross(front, right)) * 0.01;

    GSOutput vertex;
    
    //Right is relative to vertex looking at the camera
    vertex.pos = mul(float4(input[0] + right + up, 1.0f), vp); //Top left
    vertex.uv = float2(0, 0);
    output.Append(vertex);
    
    vertex.pos = mul(float4(input[0] - right - up, 1.0f), vp); //Bottom right
    vertex.uv = float2(1, 1);
    output.Append(vertex);
    
    vertex.pos = mul(float4(input[0] + right - up, 1.0f), vp); //Bottom left
    vertex.uv = float2(0, 1);
    output.Append(vertex);
    
    output.RestartStrip(); //Start second triangle
    
    vertex.pos = mul(float4(input[0] + right + up, 1.0f), vp); //Top left
    vertex.uv = float2(0, 0);
    output.Append(vertex);
    
    vertex.pos = mul(float4(input[0] - right + up, 1.0f), vp); //Top right
    vertex.uv = float2(1, 0);
    output.Append(vertex);
    
    vertex.pos = mul(float4(input[0] - right - up, 1.0f), vp); //Bottom right
    vertex.uv = float2(1, 1);
    output.Append(vertex);
    
}