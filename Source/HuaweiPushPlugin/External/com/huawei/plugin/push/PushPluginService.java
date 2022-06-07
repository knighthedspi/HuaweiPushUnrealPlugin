package com.huawei.plugin.push;

import android.util.Log;

import com.google.gson.Gson;
import com.huawei.hms.push.HmsMessageService;
import com.huawei.hms.push.RemoteMessage;
import com.huawei.plugin.push.utils.Constants;

import org.json.JSONException;
import org.json.JSONObject;

public class PushPluginService extends HmsMessageService {
    private String TAG = "PushPluginService";

    @Override
    public void onNewToken(String s) {
        super.onNewToken(s);
        HuaweiPushPlugin.getPushListener().onNewToken(s);
        Log.i(TAG, s);
    }

    @Override
    public void onMessageReceived(RemoteMessage message) {
        super.onMessageReceived(message);
        Log.i(TAG, "onMessageReceived is called");

        // Check whether the message is empty.
        if (message == null) {
            Log.e(TAG, "Received message entity is null!");
            return;
        }
        String gson = new Gson().toJson(message);
        try {
            JSONObject jsonObject = new JSONObject(gson);
            JSONObject jsonObject1 = new JSONObject(jsonObject.getString("i"));
            JSONObject jsonObject2 = new JSONObject(jsonObject1.getString("mMap"));
            HuaweiPushPlugin.getPushListener().onMessageReceived(jsonObject2.toString());
        } catch (JSONException e) {
            e.printStackTrace();
            HuaweiPushPlugin.getPushListener().onException(Constants.UNKNOWN_ERROR, "json error", e.getMessage());
        }

    }
}
