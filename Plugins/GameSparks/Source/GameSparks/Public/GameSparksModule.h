#pragma once

#include "GameSparks/gsstl.h"
#include "GameSparks/GS.h"
#include "IGameSparks.h"
#include "Runtime/Engine/Public/Tickable.h"
#include "OnlineDelegateMacros.h"

// TODO: decide what to move into the public directory
#include "../Private/GameSparksComponent.h"


DECLARE_LOG_CATEGORY_EXTERN(UGameSparksModuleLog, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameSparksAvailable, bool /* available */);
typedef FOnGameSparksAvailable::FDelegate FOnGameSparksAvailableDelegate;


class GAMESPARKS_API UGameSparksModule : public IGameSparks, public FTickableGameObject
{
public:
	/** Configuration connection */
	FString Credentials;
	FString Stage;
	FString Domain;

	/** IModuleInterface implementation */
	virtual void StartupModule();
	virtual void ShutdownModule();

	/** FTickableGameObject implementation */
    virtual bool IsTickableWhenPaused() const;
    virtual bool IsTickableInEditor() const;
    virtual void Tick(float DeltaTime);
	virtual bool IsTickable() const;
	virtual TStatId GetStatId() const;

	/* Initialization and destruction of gamesparks */
	void Initialize(FString apikey, FString secret, bool previewServer, bool clearCachedAuthentication);
	void Shutdown();
    void Logout();

	//! set the API credential type (like device, server, etc.) - use empty string to reset to the default. the default is "device"
	void SetApiCredential(FString credential);

	//! Set the api stage. if you passed usePreviewServer=true into the constructor, this will be "preview", "live" otherwise.
	void SetApiStage(FString stage);

	//! set api domain to use. pass empty string to reset to the default. The default is "ws.gamesparks.net"
	void SetApiDomain(FString domain);


	static UGameSparksModule* GetModulePtr();

	void SendGameSparksAvailableToComponents(bool available);
	void SendDebugLogToComponents(const gsstl::string& message);

    GameSparks::Core::GS& GetGSInstance() { return GS; }
    const GameSparks::Core::GS& GetGSInstance() const { return GS; }
    
    void RegisterListeners();
    
	/// returns true, iff the GameSparks instance is ready to be used
	bool IsInitialized() const;

    DEFINE_ONLINE_DELEGATE_ONE_PARAM(OnGameSparksAvailable, bool);

private:
	GameSparks::Core::IGSPlatform* platform = nullptr;
    GameSparks::Core::GS GS;
    bool isInitialised = false;

	class FOnlineFactoryGameSparks* GameSparksFactory = 0;
};
