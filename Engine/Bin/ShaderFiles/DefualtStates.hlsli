float PI = 3.14159265359f;
vector g_vMtrlDiffuse = 1.f;
vector g_vMtrlAmbient = 1.f;
vector g_vMtrlSpecular = 1.f;
sampler DefaultSampler = sampler_state
{
    filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

RasterizerState RS_Default
{
    FillMode = Solid;
    CullMode = Back;
};

RasterizerState RS_Front
{
    FillMode = Solid;
    CullMode = Front;
};

RasterizerState RS_Wireframe
{
    FillMode = Wireframe;
};

RasterizerState RS_Cull_Front
{
    CullMode = Front;
};

DepthStencilState DSS_Default
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = less_equal;
};

DepthStencilState DSS_None
{
    DepthEnable = false;
    DepthWriteMask = zero;
};

BlendState BS_None
{
    BlendEnable[0] = false;
};

BlendState BS_Blend
{
    BlendEnable[0] = true;
    BlendEnable[1] = true;

    SrcBlend = ONE;
    DestBlend = ONE;
    BlendOp = Add;
};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = true;
    BlendEnable[1] = true;

    SrcBlend = Src_Alpha;
    DestBlend = Inv_Src_Alpha;
    BlendOp = Add;
};