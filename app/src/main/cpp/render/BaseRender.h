//
// Created by Administrator on 2021/5/14.
//

#ifndef MYOPENGLCAMERA_BASERENDER_H
#define MYOPENGLCAMERA_BASERENDER_H

#include "../util/SyncLock.h"
#include "../util/ImageDef.h"
#include "ByteFlowDef.h"

class BaseRender
{
public:
    BaseRender():
            m_ViewportHeight(0),
            m_ViewportWidth(0),
            m_IsProgramChanged(false),
            m_IsTextureDirty(false)
    {
        memset(&m_RenderFrame, 0, sizeof(NativeImage));
    }

    virtual ~BaseRender()
    {

    }

    virtual int Init(int initType) = 0;
    virtual int UnInit() = 0;

    virtual void UpdateFrame(NativeImage *pImage) = 0;

    virtual void LoadFilterImageData(int index, NativeImage *pImage) = 0;

    virtual void LoadFragShaderScript(int shaderIndex, char *pShaderStr, int strLen) = 0;

    virtual void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror) = 0;

    virtual void SetShaderIndex(int shaderIndex) = 0;

    virtual int GetShaderIndex() = 0;

    virtual bool CreateTextures() = 0;

    virtual bool UpdateTextures() = 0;

    virtual bool DeleteTextures() = 0;

    virtual void OnSurfaceCreated() = 0;

    virtual void OnSurfaceChanged(int width, int height) = 0;

    virtual void OnDrawFrame() = 0;


protected:
    volatile bool   m_IsProgramChanged;
    size_t          m_ViewportWidth;
    size_t          m_ViewportHeight;
    NativeImage     m_RenderFrame;
    TransformMatrix m_TransformMatrix;

    volatile bool   m_IsTextureDirty;
};

#endif //MYOPENGLCAMERA_BASERENDER_H
