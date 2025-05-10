package com.github.crazyorr.ffmpegrecorder;

import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.webkit.WebView;
import android.widget.MediaController;
import android.widget.TextView;
import android.widget.VideoView;
import android.webkit.WebSettings;
import android.webkit.WebViewClient;

public class PlaybackActivity extends AppCompatActivity {

    public static final String INTENT_NAME_VIDEO_PATH = "INTENT_NAME_VIDEO_PATH";

    private VideoView mVvPlayback;
    private VideoView mVvPlayback2;
    public WebView mywebView;
    private int mVideoCurPos;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_playback);
        TextView tvVideoPath = (TextView) findViewById(R.id.tv_video_path);
        mVvPlayback = (VideoView) findViewById(R.id.vv_playback);

        String path = getIntent().getStringExtra(INTENT_NAME_VIDEO_PATH);
        if (path == null) {
            finish();
        }

        tvVideoPath.setText(path);
        mVvPlayback.setVideoPath(path);
        mVvPlayback.setKeepScreenOn(true);
        mVvPlayback.setMediaController(new MediaController(this));
        mVvPlayback.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {

            @Override
            public void onCompletion(MediaPlayer mp) {
            }
        });
        mVvPlayback.start();

        String path2= Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)+"/output.mp4";
        Log.i("mike",path2); 
        mVvPlayback2 = (VideoView) findViewById(R.id.vv_playback2);
        mVvPlayback2.setVideoPath(path2);
        mVvPlayback2.setKeepScreenOn(true);
        mVvPlayback2.setMediaController(new MediaController(this));
        mVvPlayback2.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {

            @Override
            public void onCompletion(MediaPlayer mp) {
            }
        });
        mVvPlayback2.start();

        mywebView=(WebView)findViewById(R.id.webview);
        WebSettings webSettings=mywebView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        mywebView.loadUrl("https://www.youtube.com/shorts/yb5E0L5O8xs?feature=share");
        mywebView.setWebViewClient(new WebViewClient());    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mVvPlayback.stopPlayback();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mVvPlayback.pause();
        mVideoCurPos = mVvPlayback.getCurrentPosition();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mVvPlayback.seekTo(mVideoCurPos);
        mVvPlayback.start();
    }
}
