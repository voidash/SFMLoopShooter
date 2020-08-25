//what should it do?

//when check for projectile collision with players
#pragma once
#include <Views/Configuration.hpp>
#include <vector>
#include <memory>
#include <Entity/Player.hpp>
#include <utility>
#include "Configuration.hpp"
#include "NetworkHandler.hpp"
#include <SFML/Network.hpp>
#include <GUI/Text.hpp>

//if focus instance dies, either add new instance or if all instance are already added
// stop the game


class GameLogic : public sf::Drawable{
    public: 
    
        GameLogic(sf::RenderWindow& window):window(window),
        gameOverText(sf::Vector2f(300,500),DefaultFont::pixel){
            Player::setDefaultMovements();
            addPlayerInstance();
            
        }

        //according to the mode, either host or client . select primary and secondary player
        Player& getPrimaryPlayer();
        Player& getSecondaryPlayer();

        //  add new player instance into the game. 
        void addPlayerInstance();



        sf::Vector2f getPrimaryPlayerPosition(){
           return getPrimaryPlayer().getPosition();
        }
        
       Mode checkConnectionMode(){
           return NetworkHandler::get().getMode();
       } 
        
        void stopInstanceRendering(Player& player){
            player.stopRendering();
        }
        
        void startInstanceRendering(Player& player){
            player.startRendering();
        }

        void checkForGameOver();
        
        void checkKill();
        
  

        
        void update(){
            for(auto& players:playerInstance){
                players.first->update();
                players.second->update();
            }
        }

        std::vector<std::pair<std::shared_ptr<Player>,std::shared_ptr<Player>>> playerInstance;
        
        

    private:
        int portCount{0};
        Text gameOverText;
        sf::IpAddress ip;
        unsigned short port;

        sf::Packet packet;
        sf::RenderWindow& window;
        int instanceOffset{0};
        bool gameOver{false};   

         //the maximum amount of instance a single player can get is given by max instance and it can be surpassed
        bool nextInstance{false}; 
        unsigned int maxInstance{3};
        unsigned int availableInstance{};

        int count{0};

        
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
           for(auto& playerPair: playerInstance){
               target.draw(*playerPair.first);
               target.draw(*playerPair.second);
           }
           target.draw(gameOverText);

        }


       
};