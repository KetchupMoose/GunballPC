using UnrealBuildTool;
using System.IO;
using System;

public class OnlineSubsystemGameSparks : ModuleRules
{
    public OnlineSubsystemGameSparks(TargetInfo Target)
    {
        Definitions.Add("GAMESPARKS_PACKAGE=1");
        Definitions.Add("ONLINESUBSYSTEMGAMESPARKS_PACKAGE=1");

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "OnlineSubsystem",
                "Engine",
                "Json",
                "GameSparks",
            }
        );

        PrivateIncludePaths.Add("Private");

        /*PublicIncludePaths.Add(Path.Combine(GameSparksPath, "include"));

        PrivateIncludePaths.AddRange(new string[]{
            Path.Combine(GameSparksPath, "src", "GameSparks"),
            Path.Combine(GameSparksPath, "src", "cjson"),
            Path.Combine(GameSparksPath, "src", "easywsclient"),
            Path.Combine(GameSparksPath, "src", "google"),
            Path.Combine(GameSparksPath, "src", "hmac"),
            Path.Combine(GameSparksPath, "src", "mbedtls"),
            Path.Combine(GameSparksPath, "src")
        });

        switch (Target.Platform)
        {
            case UnrealTargetPlatform.Win32:
                break;
            case UnrealTargetPlatform.Win64:
                break;
            case UnrealTargetPlatform.Android:
                break;
            case UnrealTargetPlatform.IOS:
                PublicAdditionalLibraries.Add("c++");
                break;
            case UnrealTargetPlatform.Mac:
                break;
            case UnrealTargetPlatform.PS4:
                break;
            case UnrealTargetPlatform.XboxOne:
            case UnrealTargetPlatform.HTML5:
            case UnrealTargetPlatform.Linux:
            default:
                throw new NotImplementedException("This target platform is not configured for GameSparks SDK: " + Target.Platform.ToString());
        }*/
    }

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string GameSparksPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "GameSparksBaseSDK")); }
    }
}
