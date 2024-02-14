#include "Object3d.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);


struct Material {
	float32_t4 color;
    
    float32_t4x4 uvTransform;
    
    int32_t enableLighting;
    
    int32_t enableTexture;
    
    int32_t enableHalfLambert;
    
    float32_t discardNum;
    
    int32_t enablePhongReflection;
    
    float32_t shininess;
    
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct Camera
{
    float32_t3 worldPosition;
};

ConstantBuffer<Camera> gCamera : register(b2);


PixelShaderOutput main(VertexShaderOutput input){
    //float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float4 transformedUV = mul(float32_t4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
   
   
    if (gMaterial.enableTexture != 0)
    {
    }
    else
    {
        textureColor = gMaterial.color;
    }
    
	PixelShaderOutput output;
    if (gMaterial.enableLighting != 0)
    {
        float cos;     
       
        //拡散反射を行うか否か
        if (gMaterial.enableHalfLambert) {
        
             //Half Lambert
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            cos = pow(NdotL * 0.5f + 0.5f, 1.0f);
            
              //追加で鏡面反射を適応させるか
            if (gMaterial.enablePhongReflection)
            {
            
                //カメラへの方向算出
                float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
                 //入射光の反射ベクトル
                float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
                   //内積
                float RdotE = dot(reflectLight, toEye);
                //鏡面反射強度
                float specularPow = pow(saturate(RdotE), gMaterial.shininess);
                //拡散反射の色
                float32_t3 diffuce = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
                //鏡面反射の色(反射色は白色を指定中)
                float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
           
                //計算結果を合わせる
                output.color.rgb = diffuce + specular;
                
                 //アルファの処理
                output.color.a = gMaterial.color.a * textureColor.a;
                
                 //textureのα値が0の時Pixelを棄却
                if (output.color.a <= gMaterial.discardNum)
                {
                    discard;
                }
                
                return output;
            }
        }
        else {
        
            //Lambertian Reflectance
            cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
            
           
        }
        //計算
        //output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    
       output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
       //アルファの処理
        output.color.a = gMaterial.color.a * textureColor.a;
      
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
      
    
    //textureのα値が0の時Pixelを棄却
    if (output.color.a <= gMaterial.discardNum)
    {
        discard;
    }
    
	return output;
}