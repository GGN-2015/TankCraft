struct VS_Out
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(VS_Out vs_out) : SV_TARGET
{
    return vs_out.color;
}