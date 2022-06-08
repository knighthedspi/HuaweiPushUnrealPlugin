// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "push.h"
#include "PushBlueprint.generated.h"

#define UN_SUBSCRIBE_FAILED 100
#define SUBSCRIBE_FAILED 101
#define DELETE_TOKEN_FAILED 102
#define GET_TOKEN_FAILED 103
#define ON_MESSAGE_RECEIVED 104

DECLARE_DYNAMIC_DELEGATE(FOnSubscribeSuccess);
DECLARE_DYNAMIC_DELEGATE(FOnUnSubscribeSuccess);
DECLARE_DYNAMIC_DELEGATE(FOnDeleteTokenSuccess);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetTokenSuccess, FString, token);
//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnNewToken, FString, token);
//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnMessageReceived, FString, messageJson);
//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetActionIntentDataSuccess, FString, dataJson);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSubscribeException, FString, message);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUnSubscribeException, FString, message);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDeleteTokenException, FString, message);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetTokenException, FString, message);
//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnMessageReceivedException, FString, message);


UCLASS()
class HUAWEIPUSHPLUGIN_API UHuaweiPushBlueprint : public UBlueprintFunctionLibrary,
	public huawei::PushListener
{
	GENERATED_BODY()

public:
	UHuaweiPushBlueprint(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
		huawei::Push::setListener(this);
	};

	static FOnSubscribeSuccess _onSubscribeSuccess;
	static FOnUnSubscribeSuccess _onUnSubscribeSuccess;
	static FOnDeleteTokenSuccess _onDeleteTokenSuccess;
	static FOnGetTokenSuccess _onGetTokenSuccess;
	//static FOnNewToken _onNewToken;
	//static FOnMessageReceived _onMessageReceived;
	//static FOnGetActionIntentDataSuccess _onGetActionIntentDataSuccess;

	static FOnSubscribeException _onSubscribeException;
	static FOnUnSubscribeException _onUnSubscribeException;
	static FOnDeleteTokenException _onDeleteTokenException;
	static FOnGetTokenException _onGetTokenException;
	//static FOnMessageReceivedException _onMessageReceivedException;
	
	UFUNCTION(BlueprintCallable, Category = "HuaweiPush Category")
	static void getToken(const FOnGetTokenSuccess &onSuccess, const  FOnGetTokenException &onException);
	
	UFUNCTION(BlueprintCallable, Category = "HuaweiPush Category")
	static void deleteToken(const FOnDeleteTokenSuccess &onSuccess, const FOnDeleteTokenException &onException);

	UFUNCTION(BlueprintCallable, Category = "HuaweiPush Category")
	static void subscribe(FString topic, const FOnSubscribeSuccess &onSuccess, const FOnSubscribeException &onException);

	UFUNCTION(BlueprintCallable, Category = "HuaweiPush Category")
	static void unSubscribe(FString topic, const FOnUnSubscribeSuccess &onSuccess, const FOnUnSubscribeException &onException);

	UFUNCTION(BlueprintCallable, Category = "HuaweiPush Category")
	static void getActionIntentData();

	UFUNCTION(BlueprintCallable, Category = "HuaweiPush Category")
	static void setAutoInitEnabled(bool isEnable);

	void onGetTokenSuccess(const FString token);
	void onDeleteTokenSuccess();
	void onNewToken(const FString token);
	void onMessageReceived(const FString messageJson);
	void onGetActionIntentDataSuccess(const FString dataJson);
	void onSubscribeSuccess();
	void onUnSubscribeSuccess();
	void onException(int errorcode, int action, const FString message);
};
