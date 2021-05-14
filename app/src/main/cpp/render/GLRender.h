//
// Created by Administrator on 2021/5/14.
//

#ifndef MYOPENGLCAMERA_GLRENDER_H
#define MYOPENGLCAMERA_GLRENDER_H

#include <GLES3/gl3.h>
#include <vector>
#include "BaseRender.h"
#include "../util/ImageDef.h"
#include "../glm/detail/type_mat.hpp"
#include "../glm/detail/type_mat4x4.hpp"


#define SPLIT_SCREEN_SHADER_INDEX     13
#define COLOR_SHIFT_SHADER_INDEX      14
#define ANTI_WIHITE_SHADER_INDEX      15
#define SCALE_CIRCLE_SHADER_INDEX     16
#define ROTATE_CIRCLE_SHADER_INDEX    17
#define DYNAMIC_GLITCH_SHADER_INDEX   18
#define LUT_A_SHADER_INDEX            19
#define LUT_B_SHADER_INDEX            20
#define LUT_C_SHADER_INDEX            21
#define LUT_D_SHADER_INDEX            22
#define DOUYIN_SHADER_INDEX           23

#define GHOST_SHADER_INDEX            26

#define ASCII_SHADER_INDEX            29
#define ASCII_SHADER_MATTE_BLUR       30
#define ASCII_SHADER_POLAR_MOSAIC     31

#define MAX_FRAME_NUM  30
#define SKIP_FRAME_NUM 5

class GLRender : public BaseRender{
public:
    GLRender();
    virtual ~GLRender();

    virtual int Init(int initType);

    virtual int UnInit();

    virtual void UpdateFrame(NativeImage *pImage);

    virtual void LoadFilterImageData(int index, NativeImage *pImage);

    virtual void LoadFragShaderScript(int shaderIndex, char *pShaderStr, int strLen);

    virtual void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

    virtual void SetShaderIndex(int shaderIndex);

    virtual int GetShaderIndex();

    virtual bool CreateTextures();

    virtual bool UpdateTextures();

    virtual bool DeleteTextures();

    virtual void OnSurfaceCreated();

    virtual void OnSurfaceChanged(int width, int height);

    virtual void OnDrawFrame();

    int CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource);

    void UpdateMVPMatrix(glm::mat4 &mat4Matrix, TransformMatrix &transformMatrix);

    float GetFrameProgress();

    void SetShaderProgramDynamicAttrib(int shaderIndex);

    GLuint UseProgram();

private:
    glm::mat4 m_MVPMatrix;

    GLuint m_YTextureId;
    GLuint m_UTextureId;
    GLuint m_VTextureId;
    GLuint m_ExtRgbaTextureId;

    GLuint m_Program;
    GLuint m_VertexShader;
    GLuint m_FragShader;

    GLuint m_VertexCoorHandle;
    GLuint m_TextureCoorHandle;

    GLint m_YTextureHandle;
    GLint m_UTextureHandle;
    GLint m_VTextureHandle;
    GLint m_LutTextureHandle;

    GLint m_ColorDriftHandle;
    GLint m_ScanLineJitterHandle;

    GLint m_TextureSizeHandle;
    GLint m_OffsetHandle;
    GLint m_TimeHandle;
    GLint m_MVPHandle;

//	std::vector<const char *> m_FragShaders;
    int m_ShaderIndex;
    volatile bool m_IsShaderChanged;

    int m_PeriodicFrameIndex;
    int m_FrameIndex;
    NativeImage m_ExtRgbaImage;
    SyncLock m_SynLock;

    volatile bool m_IsUpdateExtTexture;

    char *m_pFragShaderBuf;
    SyncLock m_ShaderBufLock;
};


#endif //MYOPENGLCAMERA_GLRENDER_H
