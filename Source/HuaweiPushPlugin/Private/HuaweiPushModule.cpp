
#include "HuaweiPushModule.h"
#include "HuaweiPushSettings.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "Misc/ConfigCacheIni.h"
#include "UObject/Package.h"

#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "HuaweiPushModule"

class HuaweiPushModule : public IHuaweiPushModule
{
	virtual void StartupModule() override
	{
		// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
		Settings = NewObject<UPrHuaweiPushSettings>(GetTransientPackage(), "HuaweiPushSettings", RF_Standalone);
		Settings->AddToRoot();
		if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "HuaweiPushPlugin",
				LOCTEXT("HuaweiPushSettingsName", "HuaweiPushPlugin"),
				LOCTEXT("HuaweiPushSettingsDescription", "Settings for Huawei Push"),
				Settings);
		}
	}

	virtual void ShutdownModule() override
	{
		// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
		// we call this function before unloading the module.
		if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "HuaweiPushPlugin");
		}

		if (!GExitPurge)
		{
			Settings->RemoveFromRoot();
		}
	}

private:
	UPrHuaweiPushSettings *Settings;
};

IMPLEMENT_MODULE(HuaweiPushModule, HuaweiPushPlugin)

#undef LOCTEXT_NAMESPACE
