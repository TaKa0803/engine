#include"Particle.hlsli"

struct Particle4GPU {
    float32_t4x4 WVP;
    float32_t4x4 World;
    float32_t4 color;
};

StructuredBuffer<Particle4GPU> gParticle4GPUs : register(t0);

struct VertexShaderInput {
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input,uint32_t instanced : SV_InstanceID) {
    VertexShaderOutput output;
    output.position = mul(input.position, gParticle4GPUs[instanced].WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gParticle4GPUs[instanced].World));
    output.color = gParticle4GPUs[instanced].color;
    return output;
}