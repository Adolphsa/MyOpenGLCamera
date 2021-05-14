package com.lucas.opengl;

import android.Manifest;
import android.os.Bundle;
import android.util.Log;
import android.util.Size;
import android.view.ViewTreeObserver;
import android.view.WindowManager;
import android.widget.RelativeLayout;

import androidx.appcompat.app.AppCompatActivity;

import com.lucas.opengl.camera.Camera2FrameCallback;
import com.lucas.opengl.camera.Camera2Wrapper;

import pub.devrel.easypermissions.EasyPermissions;

import static com.lucas.opengl.render.NativeRender.IMAGE_FORMAT_I420;

public class CameraActivity extends BaseRenderActivity implements Camera2FrameCallback {

    private static final String TAG = "CameraActivity";

    private RelativeLayout mSurfaceViewRoot;
    private Camera2Wrapper mCamera2Wrapper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera);

        initViews();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (EasyPermissions.hasPermissions(CameraActivity.this, Manifest.permission.CAMERA)) {
            mCamera2Wrapper.startCamera();
        }

        updateTransformMatrix(mCamera2Wrapper.getCameraId());
        if (mSurfaceViewRoot != null) {
            updateGLSurfaceViewSize(mCamera2Wrapper.getPreviewSize());
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (EasyPermissions.hasPermissions(CameraActivity.this, Manifest.permission.CAMERA)) {
            mCamera2Wrapper.stopCamera();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mByteFlowRender.unInit();
    }

    private void initViews() {
        mSurfaceViewRoot = (RelativeLayout) findViewById(R.id.surface_root);
        RelativeLayout.LayoutParams p = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT);
        mSurfaceViewRoot.addView(mGLSurfaceView, p);
        mByteFlowRender.init(mGLSurfaceView);

        mCamera2Wrapper = new Camera2Wrapper(this);

        ViewTreeObserver treeObserver = mSurfaceViewRoot.getViewTreeObserver();
        treeObserver.addOnPreDrawListener(new ViewTreeObserver.OnPreDrawListener() {
            @Override
            public boolean  onPreDraw() {
                mSurfaceViewRoot.getViewTreeObserver().removeOnPreDrawListener(this);
                mRootViewSize = new Size(mSurfaceViewRoot.getMeasuredWidth(), mSurfaceViewRoot.getMeasuredHeight());
                updateGLSurfaceViewSize(mCamera2Wrapper.getPreviewSize());
                return true;
            }
        });
    }

    @Override
    public void onPreviewFrame(byte[] data, int width, int height) {
        Log.d(TAG, "onPreviewFrame() called with: data = [" + data + "], width = [" + width + "], height = [" + height + "]");
        mByteFlowRender.setRenderFrame(IMAGE_FORMAT_I420, data, width, height);
        mByteFlowRender.requestRender();
    }

    @Override
    public void onCaptureFrame(byte[] data, int width, int height) {

    }
}