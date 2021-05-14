//
// Created by Administrator on 2021/5/14.
//

#include <ImageDef.h>
#include "../util/LogUtil.h"
#include "RenderContext.h"




jfieldID RenderContext::s_ContextHandle = 0L;

RenderContext::RenderContext(int renderType) :
        m_pByteFlowRender(NULL)
{
    switch (renderType)
    {
        case GL_RENDER_TYPE:
            m_pByteFlowRender = new GLRender();
            break;
        case CL_RENDER_TYPE:
            break;
        default:
            m_pByteFlowRender = new GLRender();
    }

}

RenderContext::~RenderContext()
{
    if (m_pByteFlowRender != NULL)
    {
        delete m_pByteFlowRender;
        m_pByteFlowRender = NULL;
    }

}

void RenderContext::CreateRenderContext(JNIEnv *env, jobject instance, jint renderType)
{
    LOGCATE("ByteFlowRenderContext::CreateRenderContext renderType = %d", renderType);
    RenderContext *pContext = new RenderContext(renderType);
    StoreRenderContext(env, instance, pContext);
}

void RenderContext::StoreRenderContext(JNIEnv *env, jobject instance, RenderContext *pContext)
{
    LOGCATE("ByteFlowRenderContext::StoreRenderContext");
    jclass cls = env->GetObjectClass(instance);
    if (cls == NULL)
    {
        LOGCATE("ByteFlowRenderContext::StoreRenderContext cls == NULL");
        return;
    }

    s_ContextHandle = env->GetFieldID(cls, "mNativeContextHandle", "J");
    if (s_ContextHandle == NULL)
    {
        LOGCATE("ByteFlowRenderContext::StoreRenderContext s_ContextHandle == NULL");
        return;
    }

    env->SetLongField(instance, s_ContextHandle, reinterpret_cast<jlong>(pContext));

}


void RenderContext::DeleteRenderContext(JNIEnv *env, jobject instance)
{
    LOGCATE("ByteFlowRenderContext::DeleteRenderContext");
    if (s_ContextHandle == NULL)
    {
        LOGCATE("ByteFlowRenderContext::DeleteRenderContext Could not find render context.");
        return;
    }

    RenderContext *pContext = reinterpret_cast<RenderContext *>(env->GetLongField(
            instance, s_ContextHandle));
    if (pContext)
    {
        delete pContext;
    }
    env->SetLongField(instance, s_ContextHandle, 0L);
}

RenderContext *RenderContext::GetRenderContext(JNIEnv *env, jobject instance)
{
    LOGCATE("ByteFlowRenderContext::GetRenderContext");

    if (s_ContextHandle == NULL)
    {
        LOGCATE("ByteFlowRenderContext::GetRenderContext Could not find render context.");
        return NULL;
    }

    RenderContext *pContext = reinterpret_cast<RenderContext *>(env->GetLongField(
            instance, s_ContextHandle));
    return pContext;
}

int RenderContext::Init(int initType)
{
    return m_pByteFlowRender->Init(initType);
}

int RenderContext::UnInit()
{
    return m_pByteFlowRender->UnInit();
}

void RenderContext::UpdateFrame(int format, uint8_t *pBuffer, int width, int height)
{
    LOGCATE("ByteFlowRenderContext::UpdateFrame format=%d, width=%d, height=%d, pData=%p",
            format, width, height, pBuffer);
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pBuffer;

    switch (format)
    {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }
    m_pByteFlowRender->UpdateFrame(&nativeImage);
}

void RenderContext::SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror)
{
    m_pByteFlowRender->SetTransformMatrix(translateX, translateY, scaleX, scaleY, degree, mirror);
}

void RenderContext::SetParamsInt(int paramType, int param)
{
    LOGCATE("ByteFlowRenderContext::SetParamsInt paramType = %d, param = %d", paramType, param);
    switch (paramType)
    {
        case PARAM_TYPE_SET_SHADER_INDEX:
            m_pByteFlowRender->SetShaderIndex(param);
            break;
        default:
            break;
    }

}

int RenderContext::GetParamsInt(int paramType)
{
    LOGCATE("ByteFlowRenderContext::GetParamsInt paramType = %d", paramType);
    switch (paramType)
    {
        case PARAM_TYPE_SET_SHADER_INDEX:
            return m_pByteFlowRender->GetShaderIndex();
        default:
            break;
    }
    return -1;
}

void RenderContext::OnSurfaceCreated()
{
    m_pByteFlowRender->OnSurfaceCreated();
}

void RenderContext::OnSurfaceChanged(int width, int height)
{
    m_pByteFlowRender->OnSurfaceChanged(width, height);
}

void RenderContext::OnDrawFrame()
{
    m_pByteFlowRender->OnDrawFrame();
}

void RenderContext::LoadLutImageData(int index, int format, int width, int height, uint8_t *pData)
{
    LOGCATE("ByteFlowRenderContext::LoadFilterImageData index=%d, format=%d, width=%d, height=%d, pData=%p",
            index, format, width, height, pData);
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;

    switch (format)
    {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }

    m_pByteFlowRender->LoadFilterImageData(index, &nativeImage);
}

void RenderContext::LoadFragShaderScript(int shaderIndex, char *pShaderStr, int strLen)
{
    LOGCATE("ByteFlowRenderContext::LoadFragShaderScript shaderIndex = %d, pShaderStr = %s, strLen = %d", shaderIndex, pShaderStr, strLen);
    m_pByteFlowRender->LoadFragShaderScript(shaderIndex, pShaderStr, strLen);

}