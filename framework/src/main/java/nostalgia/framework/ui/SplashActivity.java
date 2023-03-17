package nostalgia.framework.ui;

import android.annotation.TargetApi;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;

import androidx.activity.ComponentActivity;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.Nullable;

import com.blankj.utilcode.constant.PermissionConstants;
import com.blankj.utilcode.util.PermissionUtils;

import java.util.Timer;
import java.util.TimerTask;

import nostalgia.framework.R;
import nostalgia.framework.utils.NLog;
import nostalgia.framework.utils.SPUtils;

/**
 * Created by huzongyao on 2018/6/4.
 */

public class SplashActivity extends ComponentActivity {
    private static final String TAG = "SplashActivity";

    private ActivityResultLauncher<Intent> storagePermissionLauncher;

    private SPUtils spUtils;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        spUtils = new SPUtils(this);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            storagePermissionLauncher = registerForActivityResult(new ActivityResultContracts.StartActivityForResult(), result -> {
                Intent resultIntent = result.getData();
                if (result.getResultCode() == RESULT_OK && resultIntent != null) {
                    Uri data = resultIntent.getData();
                    final int takeFlags = Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_GRANT_WRITE_URI_PERMISSION;
                    getContentResolver().takePersistableUriPermission(data, takeFlags);
                    spUtils.setString("ROM_ROOT_DIR", data.toString());
                    requestStoragePermissionSuccess();
                } else {
                    NLog.e(TAG, "Failed to obtain necessary permissions.");
                    finish();
                }
            });
        }

        Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                startWithPermission();
            }
        }, 800L);
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    private void startWithPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            String rootPath = spUtils.getString("ROM_ROOT_DIR", "");
            if (rootPath.isEmpty()) {
                Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
                storagePermissionLauncher.launch(intent);
            } else {
                requestStoragePermissionSuccess();
            }
        } else {
            PermissionUtils.permission(PermissionConstants.STORAGE)
                    .callback(new PermissionUtils.SimpleCallback() {
                        @Override
                        public void onGranted() {
                            requestStoragePermissionSuccess();
                        }

                        @Override
                        public void onDenied() {
                            finish();
                        }
                    }).request();
        }
    }

    private void requestStoragePermissionSuccess() {
        Intent intent = new Intent();
        intent.setAction(getString(R.string.action_gallery_page));
        startActivity(intent);
        finish();
    }
}
