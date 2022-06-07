#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include <string>


namespace huawei
{
	
	class PushListener
	{
		virtual void getTokenSuccess(const FString &token) = 0;
		virtual void onNewToken(const FString &token) = 0;
		virtual void onMessageReceived(const FString &messageJson) = 0;
		virtual void getActionIntentDataSuccess(const FString &dataJson) = 0;
		virtual void subscribeSuccess() = 0;
		virtual void unSubscribeSuccess() = 0;
		virtual void onException(int errorcode, const FString &action, const FString &message) = 0;
	};

	class Push
	{
	public:
		static void getToken();
		static void deleteToken();
		static void setAutoInitEnabled(bool isEnable);
		static void getActionIntentData();
		static void subscribe(const FString &topic);
		static void unSubscribe(const FString &topic);

		static void setListener(PushListener *listener);
		static void removeListener();
		static PushListener *getListener();
	};
}