package com.huawei.plugin.push;

public interface HuaweiPushListener {

    void getTokenSuccess(String token);

    void onNewToken(String token);

    void onMessageReceived(String messageJson);

    void getActionIntentDataSuccess(String dataJson);

    void subscribeSuccess();

    void unSubscribeSuccess();

    void onException(int errorCode, String action, String message);

}
