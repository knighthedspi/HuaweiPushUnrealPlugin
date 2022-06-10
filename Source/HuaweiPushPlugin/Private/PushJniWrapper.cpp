#include "PushJniWrapper.h"
#include "HuaweiPushModule.h"
#include "Async/AsyncWork.h"
#include "push.h"

using namespace std;
DEFINE_LOG_CATEGORY(HuaweiPushPlugin_Native);

#if PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"

// Initialize JNI context
#define INIT_JAVA_METHOD(name, signature) \
if (JNIEnv *Env = FAndroidApplication::GetJavaEnv()) \
{ \
    name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false); \
    check(name != NULL); \
} \
else \
{ \
    check(0); \
} \

#define DECLARE_JAVA_METHOD(name) \
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

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnSubscribeSuccess(JNIEnv *env, jobject thiz)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onSubscribeSuccess(); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnUnSubscribeSuccess(JNIEnv *env, jobject thiz)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onUnSubscribeSuccess(); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnDeleteTokenSuccess(JNIEnv *env, jobject thiz)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onDeleteTokenSuccess(); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnException(JNIEnv *env, jobject thiz, int error, int action, jstring message_)
{
	FString message = jstring2FString(env, message_);
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onException(error, action, message); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnGetTokenSuccess(JNIEnv *env, jobject thiz, jstring data_)
{
	FString token = jstring2FString(env, data_);
	AsyncTask(ENamedThreads::GameThread, [=]()
	{ huawei::PushJniWrapper::getInstance()->onGetTokenSuccess(token); });
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

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnGetActionIntentDataSuccess(JNIEnv *env, jobject thiz, jstring data_)
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
	DECLARE_JAVA_METHOD(HuaweiPush_Get_Action_Intent_Data);
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
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
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
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
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
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
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
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiPush_Get_Action_Intent_Data);
			UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiPush_Get_Action_Intent_Data\n"));
		}
		else
		{
			UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
		}
	}

	void PushJniWrapper::subscribe(const string topic)
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
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

	void PushJniWrapper::unSubscribe(const string topic)
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
		{
			jstring topic0 = Env->NewStringUTF(topic.c_str());
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiPush_Unsubscribe, topic0);
			Env->DeleteLocalRef(topic0);
			UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiPush_Unsubscribe\n"));
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

	void PushJniWrapper::onSubscribeSuccess()
	{
		UE_LOG(HuaweiPushPlugin_Native, Log, TEXT("Subscribe Success"));
		if (_listener != nullptr)
		{
			_listener->onSubscribeSuccess();
		}
	}

	void PushJniWrapper::onDeleteTokenSuccess()
	{
		UE_LOG(HuaweiPushPlugin_Native, Log, TEXT("Delete Token Success"));
		if (_listener != nullptr)
		{
			_listener->onDeleteTokenSuccess();
		}
	}

	void PushJniWrapper::onUnSubscribeSuccess()
	{
		UE_LOG(HuaweiPushPlugin_Native, Log, TEXT("unSubscribe Success success"));
		if (_listener != nullptr)
		{
			_listener->onUnSubscribeSuccess();
		}
	}

	void PushJniWrapper::onException(int errorcode, int action, const FString message)
	{
		if (_listener != nullptr)
		{
			_listener->onException(errorcode, action, message);
		}
	}

	void PushJniWrapper::onGetTokenSuccess(const FString token)
	{
		if (_listener != nullptr)
		{
			_listener->onGetTokenSuccess(token);
		}
	}

	void PushJniWrapper::onNewToken(const FString token)
	{
		if (_listener != nullptr)
		{
			_listener->onNewToken(token);
		}
	}

	void PushJniWrapper::onMessageReceived(const FString messageJson)
	{
		if (_listener != nullptr)
		{
			_listener->onMessageReceived(messageJson);
		}
	}

	void PushJniWrapper::onGetActionIntentDataSuccess(const FString dataJson)
	{
		if (_listener != nullptr)
		{
			_listener->onGetActionIntentDataSuccess(dataJson);
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

	void PushJniWrapper::subscribe(const string topic)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::unSubscribe(const string topic)
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

	void PushJniWrapper::onGetTokenSuccess(const FString token)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onException(int errorcode, int action, const FString message)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onNewToken(const FString token)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onMessageReceived(const FString messageJson)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onGetActionIntentDataSuccess(const FString dataJson)
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onSubscribeSuccess()
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onUnSubscribeSuccess()
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}

	void PushJniWrapper::onDeleteTokenSuccess()
	{
		UE_LOG(HuaweiPushPlugin_Native, Warning, TEXT("Huawei Push is not supported on this platform\n"));
	}
}

#endif
