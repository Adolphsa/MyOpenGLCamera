package com.lucas.opengl.camera;

/**
 * Created by lucas on 2021/5/14.
 */
public interface Camera2FrameCallback {
    void onPreviewFrame(byte[] data, int width, int height);
    void onCaptureFrame(byte[] data, int width, int height);
}
