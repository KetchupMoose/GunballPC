#pragma once

#include <GameSparks/IGSPlatform.h>
#include <GameSparks/GSPlatformDeduction.h>
#include "GameSparksModule.h"

#include "Runtime/Launch/Resources/Version.h"

#if !defined(GS_MAKE_VERSION)
#   define GS_MAKE_VERSION(major, minor) (major*100 + minor)
#   define GS_UE_VERSION GS_MAKE_VERSION(ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION)
#endif

namespace GameSparks
{
	namespace UnrealEngineSDK
	{
		class GameSparksUnrealPlatform : public GameSparks::Core::IGSPlatform
		{
		public:
			GameSparksUnrealPlatform(const gsstl::string& apikey, const gsstl::string& secret, bool previewServer)
				: GameSparks::Core::IGSPlatform(apikey, secret, previewServer)
			{
			}

			virtual gsstl::string GetDeviceId() const
			{
				#if GS_UE_VERSION > GS_MAKE_VERSION(4, 14)
				return TCHAR_TO_UTF8(*FPlatformMisc::GetDeviceId());
				#else
				return TCHAR_TO_UTF8(*FPlatformMisc::GetUniqueDeviceId());
				#endif
			}

			virtual gsstl::string GetDeviceOS() const
			{
				return FPlatformProperties::IniPlatformName();
			}

			virtual gsstl::string GetPlatform() const
			{
				return FPlatformProperties::PlatformName();
			}

			virtual gsstl::string GetSDK() const
			{
				return "GameSparks Unreal Engine SDK";
			}

			virtual gsstl::string GetDeviceType() const
			{
#if PLATFORM_DESKTOP
				return "Desktop";
#else
				return "Handheld";
#endif	
			}

			virtual bool GetExtraDebug() const
			{
				return true;
			}

			/*virtual gsstl::string GetPreviewServiceUrl() const
			{
				return "wss://preview.gamesparks.net/ws/" + m_apiKey;
			}*/

			/*virtual gsstl::string GetLiveServiceUrl() const
			{
				return "wss://live.gamesparks.net/ws/" + m_apiKey;
			}*/

			virtual void DebugMsg(const gsstl::string& message) const
			{
				if (UGameSparksModule::GetModulePtr())
					UGameSparksModule::GetModulePtr()->SendDebugLogToComponents(message);
			}

            virtual gsstl::string ToWritableLocation(gsstl::string desired_name) const{
                #if PLATFORM_IOS || PLATFORM_ANDROID || PLATFORM_PS4 || defined(__ORBIS__)
					// Conditional since ConvertRelativePathToFull does not work on mobile devices.
					// see https://answers.unrealengine.com/questions/120796/adding-custom-files-to-the-android-content.html
					return GameSparks::Core::IGSPlatform::ToWritableLocation(desired_name);
                #else
                    FString writeableLocation = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir());
                    writeableLocation += "gamesparks_";
                    writeableLocation += FString(UTF8_TO_TCHAR(desired_name.c_str()));
                    return TCHAR_TO_UTF8(*writeableLocation);
				#endif
            }
		};
	}
}
