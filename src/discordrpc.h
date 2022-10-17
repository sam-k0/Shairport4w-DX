#pragma once
#include <string>
#include "discord_rpc.h"
#include "gms.h"

/* DLL global variables */
static const char* APPLICATION_ID = "345229890980937739";
static bool initialized = false;
static int64_t endTime = -1; // Since epoch
static int64_t startTime = -1; // Since epoch

/***
 Discord Callbacks
***/

static void handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("\nDiscord: connected to user %s#%s - %s\n",
        connectedUser->username,
        connectedUser->discriminator,
        connectedUser->userId);

    // Try to return the discord stuff
#ifdef compileforgamemaker
    int themap = CreateDsMap(0);
    DsMapAddString(themap, "event_type", "GMRPC_READY");
    DsMapAddString(themap, "user_id", connectedUser->userId);
    DsMapAddString(themap, "username", connectedUser->username);
    DsMapAddString(themap, "discriminator", connectedUser->discriminator);
    DsMapAddString(themap, "avatar", connectedUser->avatar);
    CreateAsynEventWithDSMap(themap, EVENT_OTHER_SOCIAL);
#endif // compileforgamemaker
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void handleDiscordJoin(const char* secret)
{
    printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
    printf("\nDiscord: spectate (%s)\n", secret);
}

static void handleDiscordJoinRequest(const DiscordUser* request)
{
    printf("\nDiscord: join request from %s#%s - %s\n",
        request->username,
        request->discriminator,
        request->userId);
}



/* Discord Functions */

static void discordInit()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    handlers.joinGame = handleDiscordJoin;
    handlers.spectateGame = handleDiscordSpectate;
    handlers.joinRequest = handleDiscordJoinRequest;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);

    // Set initialized to true
    initialized = true;
}

/**
* @param appid The app id of the app
* @brief This is the first function to be called. Initializes the rpc dll
*/
static gmbool gmrpc_init(const char* appid)
{
    APPLICATION_ID = appid;
    discordInit();
    //std::cout << "Initialized presence to App " << appid << endl;
    return gmtrue;
}

/**
* @brief Sets the start timestamp (Time since epoch)
* @param passedTime The timestamp
*/
static gmbool gmrpc_setStarttime(gmint passedTime)
{
    startTime = int64_t(passedTime);
    return gmtrue;
}

/**
* @brief Sets the end timestamp (Time since epoch)
* @param passedTime The timestamp
*/
static gmbool gmrpc_setEndtime(gmint passedTime)
{
    endTime = int64_t(passedTime);
    return gmtrue;
}

/**
* @param state The state to display
* @param details The details to display
* @param largeKey The image to show (large)
* @param smallKey The image to show (small)
* @brief This changes the presence of displayed info
*/
static gmbool gmrpc_setPresence(stringToDLL state, stringToDLL details, stringToDLL largeKey, stringToDLL smallKey)
{
    if (!initialized) // Check if initialized rpc
    {
        //gmu::debugmessage("GMRPC is not initialized! Please call gmrpc_init(...) first");
        return gmfalse;
    }

    char statebuf[256];
    char detbuf[256];
    char smallBuf[256];
    char largeBuf[256];
    // Get RPC
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    /// Set state
    string tempState = gmu::constcharptr_to_string(state);
    sprintf(statebuf, "%s", gmu::string_to_charptr(tempState));
    discordPresence.state = statebuf;

    /// Set Details
    string tempDetails = gmu::constcharptr_to_string(details);
    sprintf(detbuf, "%s", gmu::string_to_charptr(tempDetails));
    discordPresence.details = detbuf;

    /// Set small image
    string tempSmall = gmu::constcharptr_to_string(smallKey);
    sprintf(smallBuf, "%s", gmu::string_to_charptr(tempSmall));
    discordPresence.smallImageKey = smallBuf;

    /// Set large image
    string tempLarge = gmu::constcharptr_to_string(largeKey);
    sprintf(largeBuf, "%s", gmu::string_to_charptr(tempLarge));
    discordPresence.largeImageKey = largeBuf;

    /// Add timestamps if set
    if (startTime != -1)
    {
        discordPresence.startTimestamp = startTime;
    }
    else if (endTime != -1)
    {
        discordPresence.endTimestamp = endTime;
    }
    // Reset timestamps
    startTime = -1;
    endTime = -1;

    /// Finish and call update
    discordPresence.instance = 0;
    Discord_UpdatePresence(&discordPresence);
    Discord_RunCallbacks();

    //cout << "Updated Presence to " << tempState << " / " << tempDetails << endl;
    return gmtrue;
}

/**
* @brief Exits and frees the dll
*/
static gmbool gmrpc_exit()
{
    if (!initialized) // Check if initialized rpc
    {
        //gmu::debugmessage("GMRPC is not initialized! Please call gmrpc_init(...) first");
        return gmfalse;

    }
    //std::cout << "Exiting GMRPC" << endl;
    initialized = false;
    Discord_Shutdown();
    return gmtrue;
}

/**
* @brief Reset presence
*/
static gmbool gmrpc_clear()
{
    if (!initialized) // Check if initialized rpc
    {
        //gmu::debugmessage("GMRPC is not initialized! Please call gmrpc_init(...) first");
        return gmfalse;
    }
    Discord_ClearPresence();
    //std::cout << "Clearing Presence" << endl;
    return gmtrue;
}


/// User management




/**
* @brief This is to check the connection to the dll. Does nothing really.
*/
static gmint gmrpc_checkConnection(gmint n)
{
    return n * 2;
}


