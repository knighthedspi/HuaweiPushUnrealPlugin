#include "PushBlueprint.h"

using namespace huawei;


FOnGetTokenSuccess UHuaweiPushBlueprint::_onGetTokenSuccess;
FOnGetTokenException UHuaweiPushBlueprint::_onGetTokenException;
void UHuaweiPushBlueprint::getToken(const FOnGetTokenSuccess &onSuccess, const FOnGetTokenException &onException)
{
	Push::getToken();
	UHuaweiPushBlueprint::_onGetTokenSuccess = onSuccess;
	UHuaweiPushBlueprint::_onGetTokenException = onException;
}

FOnDeleteTokenSuccess UHuaweiPushBlueprint::_onDeleteTokenSuccess;
FOnDeleteTokenException UHuaweiPushBlueprint::_onDeleteTokenException;
void UHuaweiPushBlueprint::deleteToken(const FOnDeleteTokenSuccess &onSuccess, const FOnDeleteTokenException &onException)
{
	Push::deleteToken();
	UHuaweiPushBlueprint::_onDeleteTokenSuccess = onSuccess;
	UHuaweiPushBlueprint::_onDeleteTokenException = onException;
}


void UHuaweiPushBlueprint::getActionIntentData()
{
	Push::getActionIntentData();
}

void UHuaweiPushBlueprint::setAutoInitEnabled(bool isEnable)
{
	Push::setAutoInitEnabled(isEnable);
}

FOnSubscribeSuccess UHuaweiPushBlueprint::_onSubscribeSuccess;
FOnSubscribeException UHuaweiPushBlueprint::_onSubscribeException;
void UHuaweiPushBlueprint::subscribe(FString topic, const FOnSubscribeSuccess &onSuccess, const FOnSubscribeException &onException)
{
	Push::subscribe(topic);
	UHuaweiPushBlueprint::_onSubscribeSuccess = onSuccess;
	UHuaweiPushBlueprint::_onSubscribeException = onException;
}

FOnUnSubscribeSuccess UHuaweiPushBlueprint::_onUnSubscribeSuccess;
FOnUnSubscribeException UHuaweiPushBlueprint::_onUnSubscribeException;
void UHuaweiPushBlueprint::unSubscribe(FString topic, const FOnUnSubscribeSuccess &onSuccess, const FOnUnSubscribeException &onException)
{
	Push::unSubscribe(topic);
	UHuaweiPushBlueprint::_onUnSubscribeSuccess = onSuccess;
	UHuaweiPushBlueprint::_onUnSubscribeException = onException;
}

void UHuaweiPushBlueprint::onException(int errorcode, int action, const FString message)
{
	switch (action)
	{
	case UN_SUBSCRIBE_FAILED:
		UHuaweiPushBlueprint::_onUnSubscribeException.ExecuteIfBound(message);
		break;
	case SUBSCRIBE_FAILED:
		UHuaweiPushBlueprint::_onSubscribeException.ExecuteIfBound(message);
		break;
	case DELETE_TOKEN_FAILED:
		UHuaweiPushBlueprint::_onDeleteTokenException.ExecuteIfBound(message);
		break;
	case GET_TOKEN_FAILED:
		UHuaweiPushBlueprint::_onGetTokenException.ExecuteIfBound(message);
		break;
	case ON_MESSAGE_RECEIVED:
		//UHuaweiPushBlueprint::_onMessageReceivedException.ExecuteIfBound(message);
	default:
		break;
	}
}

void UHuaweiPushBlueprint::onSubscribeSuccess()
{
	UHuaweiPushBlueprint::_onSubscribeSuccess.ExecuteIfBound();
}

void UHuaweiPushBlueprint::onUnSubscribeSuccess()
{
	UHuaweiPushBlueprint::_onUnSubscribeSuccess.ExecuteIfBound();
}

void UHuaweiPushBlueprint::onDeleteTokenSuccess()
{
	UHuaweiPushBlueprint::_onDeleteTokenSuccess.ExecuteIfBound();
}

void UHuaweiPushBlueprint::onGetTokenSuccess(const FString token)
{
	UHuaweiPushBlueprint::_onGetTokenSuccess.ExecuteIfBound(token);
}

void UHuaweiPushBlueprint::onNewToken(const FString token)
{
	//UHuaweiPushBlueprint::_onNewToken.ExecuteIfBound(token);
}

void UHuaweiPushBlueprint::onMessageReceived(const FString messageJson)
{
	//UHuaweiPushBlueprint::_onMessageReceived.ExecuteIfBound(messageJson);
}

void UHuaweiPushBlueprint::onGetActionIntentDataSuccess(const FString dataJson)
{
	//UHuaweiPushBlueprint::_onGetActionIntentDataSuccess.ExecuteIfBound(dataJson);
}