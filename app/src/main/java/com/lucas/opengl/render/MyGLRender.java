package com.lucas.opengl.render;

import android.content.res.Resources;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.util.Size;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by lucas on 2021/5/14.
 */
public class MyGLRender extends NativeRender implements GLSurfaceView.Renderer {

    private static final String TAG = "MyGLRender";

    private GLSurfaceView mGLSurfaceView;
    public volatile boolean mReadPixels = false;
    private Size mCurrentImgSize;
    private String mImagePath;
    private Callback mCallback;

    public MyGLRender()
    {

    }

    public void init(GLSurfaceView surfaceView) {
        mGLSurfaceView = surfaceView;
        mGLSurfaceView.setEGLContextClientVersion(2);
        mGLSurfaceView.setRenderer(this);
        mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        native_CreateContext(GL_RENDER_TYPE);
        native_Init(0);
    }

    public void requestRender() {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.requestRender();
        }
    }

    public void setTransformMatrix(int degree, int mirror) {
        Log.d(TAG, "setTransformMatrix() called with: degree = [" + degree + "], mirror = [" + mirror + "]");
        native_SetTransformMatrix(0, 0, 1, 1, degree, mirror);
    }

    public void setRenderFrame(int format, byte[] data, int width, int height) {
        Log.d(TAG, "setRenderFrame() called with: data = [" + data + "], width = [" + width + "], height = [" + height + "]");
        native_UpdateFrame(format, data, width, height);
    }

    public void loadShaderFromAssetsFile(int shaderIndex, Resources r) {
        String result = null;
        try {
            InputStream in = r.getAssets().open("shaders/fshader_" + shaderIndex + ".glsl");
            int ch = 0;
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            while ((ch = in.read()) != -1) {
                baos.write(ch);
            }
            byte[] buff = baos.toByteArray();
            baos.close();
            in.close();
            result = new String(buff, "UTF-8");
            result = result.replaceAll("\\r\\n", "\n");
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (result != null) {
            native_LoadShaderScript(shaderIndex, result);
        }
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.d(TAG, "onSurfaceCreated() called with: gl = [" + gl + "], config = [" + config + "]");
        native_OnSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.d(TAG, "onSurfaceChanged() called with: gl = [" + gl + "], width = [" + width + "], height = [" + height + "]");
        native_OnSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        native_OnDrawFrame();
    }

    public void unInit() {
        native_UnInit();
        native_DestroyContext();
    }

    public interface Callback {
        void onReadPixelsSaveToLocal(String imgPath);
    }
}
