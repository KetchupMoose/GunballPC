// Copyright 2015 GameSparks Ltd 2015, Inc. All Rights Reserved.
#ifndef IGSPlatform_h__
#define IGSPlatform_h__

#pragma once

#include "./gsstl.h"
#include <GameSparks/GSLeakDetector.h>
#include <GameSparks/GSPlatformDeduction.h>
#include "GSTime.h"
#include <cassert>

namespace GameSparks
{
	namespace Core
	{
		//! This class is passed to the constructor of the GS class.
		//! If you want to customize platform related behaviour of the SDK you can inherit from
		//! this class. There already are specialisations for Cocos2dx, Marmalade and Unreal.
		class IGSPlatform
		{
			public:
				/*! Construct a IGSPlatform.
				 * @param apiKey the "GameSparks Api Key" of your project received from https://portal.gamesparks.net/
				 * @param apiSecret the "GameSparks Api Secret" of your project received from https://portal.gamesparks.net/
				 * @param usePreviewServer pass true if you want to use the preview server. Use false for the production server. Also see SetStage()
				 * @param verboseLogging more verbose logging. Usefully for debugging
				 */
				IGSPlatform(const gsstl::string& apiKey, const gsstl::string& apiSecret, bool usePreviewServer, bool verboseLogging=false)
					: m_apiKey(apiKey)
					, m_apiSecret(apiSecret)
					, m_verboseLogging(verboseLogging)
				{
                    SetApiStage(usePreviewServer?"preview":"live");

					m_AuthToken = "";
					m_RequestTimeoutSeconds = 5.0f;
				}

                virtual ~IGSPlatform(){}

				/*! Gets a unique identifier for the device

					Marmalade (MarmaladePlatform.h): s3eDeviceGetString(S3E_DEVICE_UNIQUE_ID);
					Unreal (GameSparksUnrealPlatform.h): FPlatformMisc::GetUniqueDeviceId()
					Cocos2dxPlatform: "NOT-IMPLEMENTED"

					base SDK iOS: [UIDevice identifierForVendor] on iOS >= 6.0, [UIDevice uniqueIdentifier] on earlier versions
					OS X: IOPlatformUUID is used

					the default implementation for all other platforms generates and stores a GUID on the first run of the application. If you want to use another identifier (e.g.) ANDROID_ID, you can override this member function
				*/
				virtual gsstl::string GetDeviceId() const;

				//! IOS or AND or WP8 - Required for in app purchases
				virtual gsstl::string GetDeviceOS() const;

				//! Will be used in analytics reports
				virtual gsstl::string GetPlatform() const;

				//! Will be used in analytics reports
				virtual gsstl::string GetSDK() const = 0;

				//! Will be used in analytics reports
				virtual gsstl::string GetDeviceType() const = 0;

				//! Set to true to get extended debugging information
				virtual bool GetExtraDebug() const
				{
					return m_verboseLogging;
				}

				//! return the API secret. The default returns the value passed into the constructor
				virtual gsstl::string GetGameSparksSecret() const
				{
					return m_apiSecret;
				}

                // these are deprecated - please use SetApiCredential/Stage/Domain
                GS_DEPRECATED(virtual gsstl::string GetPreviewServiceUrl() const){ assert(false); return "DEPRECATED"; }
                GS_DEPRECATED(virtual gsstl::string GetLiveServiceUrl() const){ assert(false); return "DEPRECATED"; }
                GS_DEPRECATED(virtual gsstl::string GetServiceUrl()){ assert(false); return "DEPRECATED"; }
                GS_DEPRECATED(virtual void SetPreviewMode(bool )){ assert(false); }
                GS_DEPRECATED(virtual bool GetPreviewMode() const){ assert(false); return false; }

				/// returns the Auth token
				virtual gsstl::string GetAuthToken() const { return m_AuthToken; }

				/// sets the auth token
                virtual void SetAuthToken(const gsstl::string& authToken)
                {
                    m_AuthToken = authToken;
                    StoreValue("gamesparks.authtoken", authToken);
                }

				/// returns the player id of the currently authenticated user
				virtual gsstl::string GetUserId() const { return m_UserId; }

				/// sets the player id for the currently authenticated user
                virtual void SetUserId( const gsstl::string& userId)
                {
                    m_UserId = userId;
                    StoreValue("gamesparks.userid", userId);
                }

				//! Receives debugging information from the API
				//! If you need more sophisticated logging, this is the method you should override
				virtual void DebugMsg(const gsstl::string& message) const = 0;

				/// returns the request timeout in seconds.
				virtual Seconds GetRequestTimeoutSeconds() const { return m_RequestTimeoutSeconds; }

				/// sets the request timeout in seconds
				virtual void SetRequestTimeoutSeconds(Seconds requestTimeoutSeconds) { m_RequestTimeoutSeconds = requestTimeoutSeconds; }

                //! store *value* at *key*.
				void StoreValue(const gsstl::string& key, const gsstl::string& value) const;

                //! Load Value associated with *key*. returns empty string, if key could not be retrieved.
				gsstl::string LoadValue(const gsstl::string& key) const;

				/// convert desired_name into a absolute path that can be used by fopen to open a file.
				virtual gsstl::string ToWritableLocation(gsstl::string desired_name) const;

                //! set the API credential type (like device, server, etc.) - use empty string to reset to the default. the default is "device"
                void SetApiCredential(const gsstl::string& credential)
                {
                    m_apiCredential = credential;
                }

                //! Set the api stage. if you passed usePreviewServer=true into the constructor, this will be "preview", "live" otherwise.
                void SetApiStage(const gsstl::string& stage)
                {
                    m_apiStage = stage;
                }

                //! set api domain to use. pass empty string to reset to the default. The default is "ws.gamesparks.net"
                void SetApiDomain(const gsstl::string& domain)
                {
                    m_apiDomain = domain;
                }

            protected:
				gsstl::string m_AuthToken; ///< the stored auth token received from the server
				gsstl::string m_UserId; ///< id of currently authenticated user
				Seconds m_RequestTimeoutSeconds; ///< after how many seconds a request will time out

				gsstl::string m_apiKey; ///< the api key from https://portal.gamesparks.net/
				gsstl::string m_apiSecret; ///< the secret from from https://portal.gamesparks.net/
                gsstl::string m_apiCredential;
                gsstl::string m_apiStage;
                gsstl::string m_apiDomain;

				bool m_verboseLogging; ///< use verbose logging?
           	private:
           		friend class GS;
           		void DurableInit()
           		{
                    m_AuthToken = LoadValue("gamesparks.authtoken");
                    m_UserId = LoadValue("gamesparks.userid");
           		}

                GS_LEAK_DETECTOR(IGSPlatform)
		};
	}
}
#endif // IGSPlatform_h__
