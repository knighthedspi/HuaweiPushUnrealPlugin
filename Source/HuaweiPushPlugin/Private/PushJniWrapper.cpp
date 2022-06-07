#include "PushJniWrapper.h"
#include "HuaweiPushModule.h"
#include "Async/AsyncWork.h"

using namespace std;
DEFINE_LOG_CATEGORY(HuaweiPushPlugin_Native);

#if PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

// Initialize JNI context
#define INIT_JAVA_METHOD(name, signature)
if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
{
	name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false);
	check(name != NULL);
}
else
{
	check(0);
}

#define DECLARE_JAVA_METHOD(name)
static jmethodID name = NULL;

string jstring2string(JNIEnv *env, jstring jstr)
{
	if (jstr == nullptr || !env)
	{
		return "";
	}

	const char *chars = env->GetStringUTFChars(jstr, nullptr);
	string ret(chars);
	env->ReleaseStringUTFChars(jstr, chars);

	return ret;
}

FString jstring2FString(JNIEnv *env, jstring jstr)
{
	if (jstr == nullptr || !env)
	{
		return "";
	}

	const char *chars = env->GetStringUTFChars(jstr, nullptr);
	FString ret = FString(UTF8_TO_TCHAR(chars));
	env->ReleaseStringUTFChars(jstr, chars);

	return ret;
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeSubscribeSuccess(JNIEnv *env, jobject thiz)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->subscribeSuccess() });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeUnSubscribeSuccess(JNIEnv *env, jobject thiz)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->unSubscribeSuccess() });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnException(JNIEnv *env, jobject thiz, int error, jstring action_, jstring message_)
{
	FString action = jstring2FString(env, action_);
	FString message = jstring2FString(env, message_);
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onException(error,action, message); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeGetTokenSuccess(JNIEnv *env, jobject thiz, jstring data_)
{
	FString token = jstring2FString(env, data_);
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->getTokenSuccess(token); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnNewToken(JNIEnv *env, jobject thiz, jstring data_)
{
	FString token = jstring2FString(env, data_);
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onNewToken(token); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnMessageReceived(JNIEnv *env, jobject thiz, jstring data_)
{
	FString data = jstring2FString(env, data_);
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onMessageReceived(data); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeGetActionIntentDataSuccess(JNIEnv *env, jobject thiz, jstring data_)
{
	FString data = jstring2FString(env, data_);
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onGetActionIntentDataSuccess(data); });
}

namespace huawei
{

	DECLARE_JAVA_METHOD(HuaweiPush_Get_Token);
	DECLARE_JAVA_METHOD(HuaweiPush_Delete_Token);
	DECLARE_JAVA_METHOD(HuaweiPush_Set_Auto_Init_Enabled);
	DECLARE_JAVA_METHOD(HuaweiPush_Get_Action_Intent_Data_Success);
	DECLARE_JAVA_METHOD(HuaweiPush_Subscribe);
	DECLARE_JAVA_METHOD(HuaweiPush_Unsubscribe);

	static PushJniWrapper *s_instance = nullptr;

	PushJniWrapper *PushJniWrapper::getInstance()
	{
		if (!s_instance)
		{
			s_instance = new PushJniWrapper();
		}

		return s_instance;
	}

	PushJniWrapper::PushJniWrapper()
	{
		_listener = nullptr;

		INIT_JAVA_METHOD(HuaweiPush_Get_Token, "()V");
		INIT_JAVA_METHOD(HuaweiPush_Delete_Token, "()V");
		INIT_JAVA_METHOD(HuaweiPush_Get_Action_Intent_Data, "()V");
		INIT_JAVA_METHOD(HuaweiPush_Subscribe, "(Ljava/lang/String)V");
		INIT_JAVA_METHOD(HuaweiPush_Unsubscribe, "(Ljava/lang/String)V");
		INIT_JAVA_METHOD(HuaweiPush_Set_Auto_Init_Enabled, "(Z)V");
	}

	PushJniWrapper::~PushJniWrapper()
	{
	}

	void PushJniWrapper::getToken()
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiPush_Get_Token);
			UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiPush_Get_Token\n"));
		}
		else
		{
			UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
		}
	}

	void PushJniWrapper::deleteToken()
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiPush_Delete_Token);
			UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiPush_Delete_Token\n"));
		}
		else
		{
			UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
		}
	}

	void PushJniWrapper::setAutoInitEnabled(bool isEnable)
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiPush_Set_Auto_Init_Enabled, isEnable);
			UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiPush_Set_Auto_Init_Enabled\n"));
		}
		else
		{
			UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
		}
	}

	void PushJniWrapper::getActionIntentData()
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiPush_Get_Action_Intent_Data);
			UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiPush_Get_Action_Intent_Data\n"));
		}
		else
		{
			UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
		}
	}

	void PushJniWrapper::subscribe(std::string topic)
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring topic0 = Env->NewStringUTF(topic.c_str());
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiPush_Subscribe, topic0);
			Env->DeleteLocalRef(topic0);
			UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiPush_Subscribe\n"));
		}
		else
		{
			UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
		}
	}

	void PushJniWrapper::unSubscribe(std::string topic)
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring topic0 = Env->NewStringUTF(topic.c_str());
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiPush_Get_PurchaseRecords, topic0);
			Env->DeleteLocalRef(topic0);
			UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiPush_Get_PurchaseRecords\n"));
		}
		else
		{
			UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
		}
	}

	/********************************************
	 * Listener
	 ********************************************/

	void PushJniWrapper::setListener(PushListener *listener)
	{
		_listener = listener;
	}

	void PushJniWrapper::removeListener()
	{
		_listener = nullptr;
	}

	PushListener *PushJniWrapper::getListener()
	{
		return _listener;
	}

	/********************************************
	 * Callbacks
	 ********************************************/

	void PushJniWrapper::subscribeSuccess()
	{
		UE_LOG(HuaweiPush_Native, Log, TEXT("Subscribe Success success"));
		if (_listener != nullptr)
		{
			_listener->SubscribeSuccess();
		}
	}

	void PushJniWrapper::unSubscribeSuccess()
	{
		UE_LOG(HuaweiPush_Native, Log, TEXT("unSubscribe Success success"));
		if (_listener != nullptr)
		{
			_listener->unSubscribeSuccess();
		}
	}

	void PushJniWrapper::onException(int errorcode, const FString &action, const FString &message)
	{
		if (_listener != nullptr)
		{
			_listener->onException(errorcode, action, message);
		}
	}

	void PushJniWrapper::getTokenSuccess(const FString &token)
	{
		if (_listener != nullptr)
		{
			_listener->getTokenSuccess(token);
		}
	}

	void PushJniWrapper::onNewToken(const FString &token)
	{
		if (_listener != nullptr)
		{
			_listener->onNewToken(token);
		}
	}

	void PushJniWrapper::onMessageReceived(const FString &messageJson)
	{
		if (_listener != nullptr)
		{
			_listener->onMessageReceived(messageJson);
		}
	}

	void PushJniWrapper::getActionIntentDataSuccess(const FString &dataJson)
	{
		if (_listener != nullptr)
		{
			_listener->getActionIntentDataSuccess(dataJson);
		}
	}

}

#else

// Stub function for other platforms
namespace huawei
{

	static PushJniWrapper *s_instance = nullptr;

	PushJniWrapper *PushJniWrapper::getInstance()
	{
		if (!s_instance)
		{
			s_instance = new PushJniWrapper();
		}

		return s_instance;
	}

	PushJniWrapper::PushJniWrapper()
	{
		_listener = nullptr;
	}

	PushJniWrapper::~PushJniWrapper()
	{
	}

	void PushJniWrapper::getToken()
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::deleteToken()
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::setAutoInitEnabled(bool isEnable)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::getActionIntentData()
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::subscribe(std::string topic)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::unSubscribe(std::string topic)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	/********************************************
	 * Listener
	 ********************************************/

	void PushJniWrapper::setListener(PushListener *listener)
	{
		_listener = listener;
	}

	void PushJniWrapper::removeListener()
	{
		_listener = nullptr;
	}

	PushListener *PushJniWrapper::getListener()
	{
		return _listener;
	}

	/********************************************
	 * Callbacks
	 ********************************************/

	void PushJniWrapper::getTokenSuccess(const FString &token)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onException(int errorcode, const FString &action, const FString &message)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onNewToken(const FString &token)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onMessageReceived(const FString &messageJson)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::getActionIntentDataSuccess(const FString &dataJson)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::subscribeSuccess()
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::unSubscribeSuccess()
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}
}

#endif
