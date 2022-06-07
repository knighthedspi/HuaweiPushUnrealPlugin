#pragma once
#include "push.h"
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(HuaweiPushPlugin_Native, Log, All);

namespace huawei
{

	class PushJniWrapper
	{

	public:
		static PushJniWrapper *getInstance();

		PushJniWrapper();
		~PushJniWrapper();

		void getToken();
		void deleteToken();
		void setAutoInitEnabled(bool isEnable);
		void getActionIntentData();
		void subscribe(std::string topic);
		void unSubscribe(std::string topic);

		// Listener
		void setListener(PushListener *listener);
		void removeListener();
		PushListener *getListener();

		// Callbacks
		void getTokenSuccess(const FString &token);
		void onNewToken(const FString &token);
		void onMessageReceived(const FString &messageJson);
		void getActionIntentDataSuccess(const FString &dataJson);
		void subscribeSuccess();
		void unSubscribeSuccess();
		void onException(int errorcode, const FString &action, const FString &message);

	protected:
		PushListener *_listener;
	};
}
