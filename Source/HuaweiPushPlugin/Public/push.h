#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include <string>
#include "json98.h"

namespace huawei
{

	class PushListener
	{
		virtual void onGetTokenSuccess(const FString token) = 0;
		virtual void onDeleteTokenSuccess() = 0;
		virtual void onNewToken(const FString token) = 0;
		virtual void onMessageReceived(const FString messageJson) = 0;
		virtual void onGetActionIntentDataSuccess(const FString dataJson) = 0;
		virtual void onSubscribeSuccess() = 0;
		virtual void onUnSubscribeSuccess() = 0;
		virtual void onException(int errorcode, const FString action, const FString message) = 0;
	};

	class Push
	{
	public:
		static void getToken();
		static void deleteToken();
		static void setAutoInitEnabled(bool isEnable);
		static void getActionIntentData();
		static void subscribe(const FString topic);
		static void unSubscribe(const FString topic);

		static void setListener(PushListener *listener);
		static void removeListener();
		static PushListener *getListener();
	};
}