# Epic Online Services (EOS) Integration & Onboarding Guide

## EOS Setup Overview

**The Stations** utilizes Epic Online Services (EOS) to provide cross-platform multiplayer (PC/Mac/Android) and P2P networking completely free of charge.

## Developer Portal Steps

1. Log in to [Epic Developer Portal](https://dev.epicgames.com/portal/).
2. Create product **TheStations**.
3. Generate Product ID, Sandbox ID, and Deployment ID.
4. Go to **Clients** tab and create a client policy:
   - **Policy Type**: PeerToPeer and Lobbies enabled.
5. Obtain Client ID and Client Secret.

## Configuration Injection

Credentials should be supplied to `the_stations::EOSConfig` upon startup:

```cpp
the_stations::EOSConfig config;
config.productId = "YOUR_PRODUCT_ID";
config.sandboxId = "YOUR_SANDBOX_ID";
config.deploymentId = "YOUR_DEPLOYMENT_ID";
config.clientId = "YOUR_CLIENT_ID";
config.clientSecret = "YOUR_CLIENT_SECRET";

the_stations::EOSNetworkManager networkManager;
networkManager.initialize(config);
```

## Room Matching Logic

- **Host**: Calls `createLobby(8)` -> Generates 5-digit room code (e.g. `X7K9P`).
- **Guest**: Enters 5-digit room code -> Calls `searchAndJoinLobby(roomCode)`.
- **Public**: Calls `joinPublicLobby()`.
