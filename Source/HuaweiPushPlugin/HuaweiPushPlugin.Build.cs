using System.IO;
using Tools.DotNETCommon;

namespace UnrealBuildTool.Rules
{
    public class HuaweiPushPlugin : ModuleRules
    {
        public HuaweiPushPlugin(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivateIncludePaths.AddRange(
                new string[] {
                    "HuaweiPushPlugin/Private"
                });

            PublicDependencyModuleNames.AddRange(
                new string[] {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "RenderCore",
                    "RHI"
                });

            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "Settings",
                    "Launch"
                }
            );

            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                PublicDependencyModuleNames.AddRange(
                    new string[]
                    {
                        "Launch"
                    })
                ;
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "HuaweiPushPlugin_APL.xml")));
            }
        }
    }
}