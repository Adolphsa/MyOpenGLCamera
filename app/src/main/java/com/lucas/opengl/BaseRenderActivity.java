package com.lucas.opengl;

import android.hardware.camera2.CameraCharacteristics;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Size;
import android.widget.RelativeLayout;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.lucas.opengl.camera.CameraUtil;
import com.lucas.opengl.render.MyGLRender;

/**
 * Created by lucas on 2021/5/14.
 */
public abstract class BaseRenderActivity extends AppCompatActivity {

    protected MyGLRender mByteFlowRender;
    protected GLSurfaceView mGLSurfaceView;

    //protected int mCurrentShaderIndex = SHADER_NUM - 1;
    protected int mCurrentShaderIndex = 23;
    protected Size mRootViewSize, mScreenSize;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGLSurfaceView = new GLSurfaceView(this);
        mByteFlowRender = new MyGLRender();
    }


    public void updateTransformMatrix(String cameraId) {
        if (Integer.valueOf(cameraId) == CameraCharacteristics.LENS_FACING_FRONT) {
            mByteFlowRender.setTransformMatrix(90, 0);
        } else {
            mByteFlowRender.setTransformMatrix(90, 1);
        }

    }

    public void updateGLSurfaceViewSize(Size previewSize) {
        Size fitSize = null;
        fitSize = CameraUtil.getFitInScreenSize(previewSize.getWidth(), previewSize.getHeight(), getScreenSize().getWidth(), getScreenSize().getHeight());
        RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) mGLSurfaceView
                .getLayoutParams();
        params.width = fitSize.getWidth();
        params.height = fitSize.getHeight();
        params.addRule(RelativeLayout.ALIGN_PARENT_TOP | RelativeLayout.CENTER_HORIZONTAL);

        mGLSurfaceView.setLayoutParams(params);
    }

    public Size getScreenSize() {
        if (mScreenSize == null) {
            DisplayMetrics displayMetrics = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
            mScreenSize = new Size(displayMetrics.widthPixels, displayMetrics.heightPixels);
        }
        return mScreenSize;
    }
}
