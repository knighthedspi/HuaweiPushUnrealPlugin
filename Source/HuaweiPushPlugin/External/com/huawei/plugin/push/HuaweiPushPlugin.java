package com.huawei.plugin.push;


import android.app.Activity;
import android.app.NativeActivity;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;

import com.huawei.agconnect.AGConnectOptionsBuilder;
import com.huawei.hmf.tasks.OnCompleteListener;
import com.huawei.hmf.tasks.Task;
import com.huawei.hms.aaid.HmsInstanceId;
import com.huawei.hms.common.ApiException;
import com.huawei.hms.push.HmsMessaging;
import com.huawei.hms.support.common.ActivityMgr;
import com.huawei.plugin.push.utils.ExceptionHandle;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Objects;

import static com.huawei.plugin.push.utils.Constants.DELETE_TOKEN_FAILED;
import static com.huawei.plugin.push.utils.Constants.GET_TOKEN_FAILED;
import static com.huawei.plugin.push.utils.Constants.SUBSCRIBE_FAILED;
import static com.huawei.plugin.push.utils.Constants.UNKNOWN_ERROR;
import static com.huawei.plugin.push.utils.Constants.UN_SUBSCRIBE_FAILED;

public class HuaweiPushPlugin {

    private static final String TAG = "HuaweiAccountPlugin";
    private static boolean isInit = false;
    private static NativeActivity mActivity = null;
    private static HuaweiPushListener mListener = null;
    private static String APP_ID = "";

    public static void initialize(NativeActivity activity, HuaweiPushListener listener) {
        if (!isInit) {
            mActivity = activity;
            mListener = listener;
            APP_ID = new AGConnectOptionsBuilder().build(mActivity).getString("client/app_id");
            isInit = true;
        }
    }

    public static void getToken() {
        // Create a thread.
        new Thread() {
            @Override
            public void run() {
                try {
                    // Set tokenScope to HCM.
                    String tokenScope = "HCM";
                    String token = HmsInstanceId.getInstance(mActivity).getToken(APP_ID, tokenScope);
                    Log.i(TAG, "get token: " + token);

                    // Check whether the token is null.
                    if (!TextUtils.isEmpty(token)) {
                        mListener.onGetTokenSuccess(token);
                    } else {
                        mListener.onException(UNKNOWN_ERROR, GET_TOKEN_FAILED, "token is null");
                    }
                } catch (ApiException e) {
                    Log.e(TAG, "get token failed, " + e);
                    ExceptionHandle.handle(mActivity, GET_TOKEN_FAILED, e, mListener);
                }
            }
        }.start();
    }

    public static void deleteToken() {
        // Create a thread.
        new Thread() {
            @Override
            public void run() {
                try {
                    // Set tokenScope to HCM.
                    String tokenScope = "HCM";

                    // Delete the token.
                    HmsInstanceId.getInstance(mActivity).deleteToken(APP_ID, tokenScope);
                    mListener.onDeleteTokenSuccess();
                    Log.i(TAG, "token deleted successfully");
                } catch (ApiException e) {
                    Log.e(TAG, "deleteToken failed." + e);
                    ExceptionHandle.handle(mActivity, DELETE_TOKEN_FAILED, e, mListener);
                }
            }
        }.start();
    }

    public static void setAutoInitEnabled(final boolean isEnable) {
        if (isEnable) {
            // Enable automatic initialization.
            HmsMessaging.getInstance(mActivity).setAutoInitEnabled(true);
        } else {
            // Disable automatic initialization.
            HmsMessaging.getInstance(mActivity).setAutoInitEnabled(false);
        }
    }

    /**
     *
     */
    public static void getActionIntentData() {
        Activity activity = ActivityMgr.INST.getCurrentActivity();
        activity = activity == null ? mActivity : activity;
        Intent intent = activity.getIntent();
        if (null != intent) {
            // Obtains the value in data.
            Bundle bundle = intent.getExtras();
            JSONObject jsonObject = new JSONObject();
            if (bundle != null) {
                for (String key : bundle.keySet()) {

                    try {
                        String content = bundle.getString(key);
                        jsonObject.put(key, content);
                        Log.i(TAG, "receive data from push, key = " + key + ", content = " + content);
                    } catch (JSONException e) {
                        Log.i(TAG, Objects.requireNonNull(e.getMessage()));
                    }
                }
                mListener.onGetActionIntentDataSuccess(jsonObject.toString());
            }
        } else {
            Log.i(TAG, "intent is null");
        }
    }


    public static void subscribe(String topic) {
        try {
            // Subscribe to a topic.
            HmsMessaging.getInstance(mActivity).subscribe(topic)
                    .addOnCompleteListener(new OnCompleteListener<Void>() {
                        @Override
                        public void onComplete(Task<Void> task) {
                            // Obtain the topic subscription result.
                            if (task.isSuccessful()) {
                                Log.i(TAG, "subscribe topic successfully");
                                mListener.onSubscribeSuccess();
                            } else {
                                Log.e(TAG, "subscribe topic failed, return value is " + task.getException().getMessage());
                                ExceptionHandle.handle(mActivity, SUBSCRIBE_FAILED, task.getException(), mListener);
                            }
                        }
                    });
        } catch (Exception e) {
            Log.e(TAG, "subscribe failed, catch exception : " + e.getMessage());
            mListener.onException(UNKNOWN_ERROR, SUBSCRIBE_FAILED, e.getMessage());
        }
    }

    public static void unsubscribe(String topic) {
        try {
            // Unsubscribe from a topic.
            HmsMessaging.getInstance(mActivity).unsubscribe(topic)
                    .addOnCompleteListener(new OnCompleteListener<Void>() {
                        @Override
                        public void onComplete(Task<Void> task) {
                            // Obtain the topic unsubscription result.
                            if (task.isSuccessful()) {
                                Log.i(TAG, "unsubscribe topic successfully");
                                mListener.onUnSubscribeSuccess();
                            } else {
                                Log.e(TAG, "unsubscribe topic failed, return value is " + task.getException().getMessage());
                                ExceptionHandle.handle(mActivity, UN_SUBSCRIBE_FAILED, task.getException(), mListener);
                            }
                        }
                    });
        } catch (Exception e) {
            Log.e(TAG, "unsubscribe failed, catch exception : " + e.getMessage());
            mListener.onException(UNKNOWN_ERROR, UN_SUBSCRIBE_FAILED, e.getMessage());
        }
    }

    public static HuaweiPushListener getPushListener() {
        return mListener;
    }
}
