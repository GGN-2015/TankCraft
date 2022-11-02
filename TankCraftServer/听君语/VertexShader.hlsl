struct VS_Out
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VS_Out main(float2 pos : POSITION)
{
    VS_Out vs_out;
    vs_out.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    vs_out.color = float4(1, 1, 0, 1);
    return vs_out;
}