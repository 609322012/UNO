#pragma once

#include <memory>
#include <random>
#include <deque>
#include <cstdlib>

#include "stat.h"
#include "../network/server.h"

namespace UNO { namespace Game {

using namespace Network;

class GameBoard {
public:
    explicit GameBoard(std::string port);

private:
    void ReceiveUsername(int index, const std::string &username);

    void StartGame();

    void GameLoop();
    
    void HandleDraw(const std::unique_ptr<DrawInfo> &info);
    
    void HandleSkip(const std::unique_ptr<SkipInfo> &info);
    
    void HandlePlay(const std::unique_ptr<PlayInfo> &info);

    void Win();

    template <typename InfoT>
    void Broadcast(InfoT &info) {
        int currentPlayer = mGameStat->GetCurrentPlayer();
        for (int i = 0; i < Common::Common::mPlayerNum; i++) {
            if (i != currentPlayer) {
                info.mPlayerIndex = Common::Util::WrapWithPlayerNum(currentPlayer - i);
                mServer.DeliverInfo<InfoT>(i, info);
            }
        }
    }

private:
    constexpr static int PLAYER_NUM = 3;
    Network::Server mServer;

    // state of game board
    std::unique_ptr<Deck> mDeck;
    std::unique_ptr<DiscardPile> mDiscardPile;
    std::unique_ptr<GameStat> mGameStat;

    // state of all players
    std::vector<PlayerStat> mPlayerStats;
};
}}