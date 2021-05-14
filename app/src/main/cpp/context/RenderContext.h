//
// Created by Administrator on 2021/5/14.
//

#ifndef MYOPENGLCAMERA_RENDERCONTEXT_H
#define MYOPENGLCAMERA_RENDERCONTEXT_H

#include <cstdint>
#include <jni.h>
#include "../render/GLRender.h"

#define GL_RENDER_TYPE   0
#define CL_RENDER_TYPE   1

#define PARAM_TYPE_SET_SHADER_INDEX   201

class RenderContext {
public:
    RenderContext(int renderType);

    ~RenderContext();

    static void CreateRenderContext(JNIEnv *env, jobject instance, jint renderType);

    static void StoreRenderContext(JNIEnv *env, jobject instance, RenderContext *pContext);

    static void DeleteRenderContext(JNIEnv *env, jobject instance);

    static RenderContext* GetRenderContext(JNIEnv *env, jobject instance);

    int Init(int initType);

    int UnInit();

    void UpdateFrame(int format, uint8_t *pBuffer, int width, int height);

    void LoadLutImageData(int index, int format, int width, int height, uint8_t *pData);

    void LoadFragShaderScript(int shaderIndex, char *pShaderStr, int strLen);

    void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

    void SetParamsInt(int paramType, int param);

    int GetParamsInt(int paramType);

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

private:
    static jfieldID s_ContextHandle;

    GLRender *m_pByteFlowRender;
};


#endif //MYOPENGLCAMERA_RENDERCONTEXT_H
