#include "../OnlineSubsystemGameSparksPrivatePCH.h"
#include "OnlineIdentityInterfaceGameSparks.h"
#include "GameSparksModule.h"
#include <GameSparks/generated/GSRequests.h>

// FOnlineIdentityInterfaceGameSparks

/**
 * Sets the needed configuration properties
 */
FOnlineIdentityInterfaceGameSparks::FOnlineIdentityInterfaceGameSparks()
	: bHasLoginOutstanding(false)
	, LocalUserNumPendingLogin(0)
{}


/**
 * Obtain online account info for a user that has been registered
 *
 * @param UserId user to search for
 *
 * @return info about the user if found, NULL otherwise
 */
TSharedPtr<FUserOnlineAccount> FOnlineIdentityInterfaceGameSparks::GetUserAccount(const FUniqueNetId& UserId) const
{
	TSharedPtr<FUserOnlineAccount> Result;

	const TSharedRef<FUserOnlineAccountGameSparks>* FoundUserAccount = UserAccounts.Find(UserId.ToString());
	if (FoundUserAccount != NULL)
	{
		Result = *FoundUserAccount;
	}

	return Result;
}

/**
 * Obtain list of all known/registered user accounts
 *
 * @param UserId user to search for
 *
 * @return info about the user if found, NULL otherwise
 */
TArray<TSharedPtr<FUserOnlineAccount> > FOnlineIdentityInterfaceGameSparks::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount> > Result;
	
	for (FUserOnlineAccountGameSparksMap::TConstIterator It(UserAccounts); It; ++It)
	{
		Result.Add(It.Value());
	}

	return Result;
}


TSharedPtr<const FUniqueNetId> FOnlineIdentityInterfaceGameSparks::GetUniquePlayerId(int32 LocalUserNum) const
{
	const TSharedPtr<const FUniqueNetId>* FoundId = UserIds.Find(LocalUserNum);
	if (FoundId != NULL)
	{
		return *FoundId;
	}
	return NULL;
}


bool FOnlineIdentityInterfaceGameSparks::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	FString ErrorStr;

	if (bHasLoginOutstanding)
	{
		ErrorStr = FString::Printf(TEXT("Registration already pending for user %d"), 
			LocalUserNumPendingLogin);
	}
	else
	{
		// keep track of local user requesting registration
		LocalUserNumPendingLogin = LocalUserNum;
		bHasLoginOutstanding = true;
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogOnline, Error, TEXT("RegisterUser() failed: %s"),
			*ErrorStr);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, FUniqueNetIdString(TEXT("")), ErrorStr);
		return false;
	}

    auto AuthenticationRequest_Response = [=](GameSparks::Core::GS& gs, const GameSparks::Api::Responses::AuthenticationResponse& authResponse)
    {
        bHasLoginOutstanding = false;

        if(authResponse.GetHasErrors())
        {
            TriggerOnLoginCompleteDelegates(LocalUserNum, false, FUniqueNetIdString(TEXT("")), authResponse.GetErrors().GetValue().GetJSON().c_str());
        }
        else
        {
            TSharedRef<FUserOnlineAccountGameSparks> UserRef(new FUserOnlineAccountGameSparks(authResponse));
			UserIds.Add(LocalUserNum, UserRef->UserIdPtr);
            UserAccounts.Add(UserRef->response.GetUserId().GetValue().c_str(), UserRef);
            TriggerOnLoginCompleteDelegates(LocalUserNum, true, FUniqueNetIdString(authResponse.GetAuthToken().GetValue().c_str()), TEXT(""));
        }
    };

    GameSparks::Core::GS& gs = UGameSparksModule::GetModulePtr()->GetGSInstance();

    if(AccountCredentials.Type == "GSDevice")
    {
        GameSparks::Api::Requests::DeviceAuthenticationRequest authRequest(gs);
        authRequest.SetDeviceOS(gs.GetDeviceOS());
        authRequest.SetDeviceId(gs.GetDeviceId()); // generate a random device id (for easy testing)
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSCredentials")
    {
        GameSparks::Api::Requests::AuthenticationRequest authRequest(gs);
        authRequest.SetUserName(TCHAR_TO_UTF8(*AccountCredentials.Id));
        authRequest.SetPassword(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSAmazon")
    {
        GameSparks::Api::Requests::AmazonConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSFacebook")
    {
        GameSparks::Api::Requests::FacebookConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSGameCenter")
    {
        GameSparks::Api::Requests::GameCenterConnectRequest authRequest(gs);
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSGooglePlay")
    {
        GameSparks::Api::Requests::GooglePlayConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSGooglePlus")
    {
        GameSparks::Api::Requests::GooglePlusConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSKongregate")
    {
        GameSparks::Api::Requests::KongregateConnectRequest authRequest(gs);
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSPSN")
    {
        GameSparks::Api::Requests::PSNConnectRequest authRequest(gs);
        authRequest.SetAuthorizationCode(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSQQ")
    {
        GameSparks::Api::Requests::QQConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSSteam")
    {
        GameSparks::Api::Requests::SteamConnectRequest authRequest(gs);
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSTwitch")
    {
        GameSparks::Api::Requests::TwitchConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSTwitter")
    {
        GameSparks::Api::Requests::TwitterConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Id));
        authRequest.SetAccessSecret(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSViber")
    {
        GameSparks::Api::Requests::ViberConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSWeChat")
    {
        GameSparks::Api::Requests::WeChatConnectRequest authRequest(gs);
        authRequest.SetAccessToken(TCHAR_TO_UTF8(*AccountCredentials.Token));
        authRequest.Send(AuthenticationRequest_Response);
    }
    else if(AccountCredentials.Type == "GSXBOXLive")
    {
        GameSparks::Api::Requests::XBOXLiveConnectRequest authRequest(gs);
        authRequest.Send(AuthenticationRequest_Response);
    }
    else
    {
        UE_LOG(LogOnline, Error, TEXT("OnlineSubsystem GameSparks: AccountCredentials.Type must be one of [GSAmazon, GSCredentials, GSDevice, GSFacebook, GSGameCenter, GSGooglePlay, GSGooglePlus, GSKongregate, GSPSN, GSQQ, GSRegis, GSSteam, GSTwitch, GSTwitter, GSViber, GSWeChat, GSXBOXLive]."), *ErrorStr);
        return false;
    }

    //TODO: implement
	return true;
}


TSharedPtr<const FUniqueNetId> FOnlineIdentityInterfaceGameSparks::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	if (Bytes != NULL && Size > 0)
	{
		FString StrId(Size, (TCHAR*)Bytes);
		return MakeShareable(new FUniqueNetIdString(StrId));
	}
	return NULL;
}


TSharedPtr<const FUniqueNetId> FOnlineIdentityInterfaceGameSparks::CreateUniquePlayerId(const FString& Str)
{
	return MakeShareable(new FUniqueNetIdString(Str));
}

// All of the methods below here fail because they aren't supported

bool FOnlineIdentityInterfaceGameSparks::Logout(int32 LocalUserNum)
{
	TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		// remove cached user account
		UserAccounts.Remove(UserId->ToString());
		// remove cached user id
		UserIds.Remove(LocalUserNum);
		// not async but should call completion delegate anyway
		TriggerOnLogoutCompleteDelegates(LocalUserNum, true);

        //TODO check if it's appropriate to call UGameSparksModule::Logout() here.
		return true;
	}
	else
	{
		UE_LOG(LogOnline, Warning, TEXT("No logged in user found for LocalUserNum=%d."), LocalUserNum);
		TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
	}
	return false;
}


bool FOnlineIdentityInterfaceGameSparks::AutoLogin(int32 LocalUserNum)
{
	return false;
}


ELoginStatus::Type FOnlineIdentityInterfaceGameSparks::GetLoginStatus(int32 LocalUserNum) const
{
	TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		return GetLoginStatus(*UserId);
	}
	return ELoginStatus::NotLoggedIn;
}


ELoginStatus::Type FOnlineIdentityInterfaceGameSparks::GetLoginStatus(const FUniqueNetId& UserId) const
{
	TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(UserId);
	if (UserAccount.IsValid() &&
		UserAccount->GetUserId()->IsValid() &&
		!UserAccount->GetAccessToken().IsEmpty())
	{
		return ELoginStatus::LoggedIn;
	}
	return ELoginStatus::NotLoggedIn;
}


FString FOnlineIdentityInterfaceGameSparks::GetPlayerNickname(int32 LocalUserNum) const
{
	TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		return  GetPlayerNickname(*UserId);
	}
	return TEXT("");
}


FString FOnlineIdentityInterfaceGameSparks::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	const TSharedRef<FUserOnlineAccountGameSparks>* FoundUserAccount = UserAccounts.Find(UserId.ToString());
	if (FoundUserAccount != NULL)
	{
		const TSharedRef<FUserOnlineAccountGameSparks>& UserAccount = *FoundUserAccount;
		return UserAccount->GetDisplayName();
	}
	return TEXT("");
}


FString FOnlineIdentityInterfaceGameSparks::GetAuthToken(int32 LocalUserNum) const
{
	TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(*UserId);
		if (UserAccount.IsValid())
		{
			return UserAccount->GetAccessToken();
		}
	}
	return FString();
}


void FOnlineIdentityInterfaceGameSparks::GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(UserId, Privilege, (uint32)EPrivilegeResults::NoFailures);
}	


FPlatformUserId FOnlineIdentityInterfaceGameSparks::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId)
{
	for (int i = 0; i < MAX_LOCAL_PLAYERS; ++i)
	{
		auto CurrentUniqueId = GetUniquePlayerId(i);
		if (CurrentUniqueId.IsValid() && (*CurrentUniqueId == UniqueNetId))
		{
			return i;
		}
	}

	return PLATFORMUSERID_NONE;
}


#if GS_UE_VERSION > GS_MAKE_VERSION(4,10)
FString FOnlineIdentityInterfaceGameSparks::GetAuthType() const
{
	return TEXT("");
}
#endif
