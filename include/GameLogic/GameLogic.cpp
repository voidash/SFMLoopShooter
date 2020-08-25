#include "GameLogic.hpp"

Player& GameLogic::getPrimaryPlayer(){
            std::shared_ptr<Player> player;
            switch(checkConnectionMode()){
                    case Mode::Client:
                        player = playerInstance.back().second;
                        break;
                    case Mode::Host:
                        player = playerInstance.back().first;
                        break;
                }
                return *player;
}


  Player& GameLogic::getSecondaryPlayer(){
            std::shared_ptr<Player> player;
            switch(checkConnectionMode()){
                    case Mode::Client:
                        player = playerInstance.back().first;
                        break;
                    case Mode::Host:
                        player = playerInstance.back().second;
                        break;
                }
                return *player;
        }

 void GameLogic::addPlayerInstance(){
            if(availableInstance < maxInstance){
                availableInstance++;
                int bufCount{0};
                if(!playerInstance.empty()){
                    for(auto& a:playerInstance){
                        
                        a.first->setPosition(sf::Vector2f(screen_size_x/2,screen_size_y/2+bufCount*200));
                        a.second->setPosition(sf::Vector2f(max_coords_x - screen_size_x/2,screen_size_y/2+bufCount*200));
                        a.first->startRendering();
                        a.second->startRendering();

                        switch(checkConnectionMode()){
                            case Mode::Client:
                                a.second->setEventPlayMode();
                            case Mode::Host:
                                a.first->setEventPlayMode();
                        }
                        bufCount++;
                    }
                }

                portCount++;
                bool primary{false};
                switch(checkConnectionMode()){
                    case Mode::Client:
                        primary = false;
                        break;
                    case Mode::Host:
                        primary = true;
                        break;
                }

                playerInstance.emplace_back(std::make_pair(new Player(sf::Vector2f(screen_size_x/2,screen_size_y/2+instanceOffset),DefaultResources::player,true,window,++count,portCount,primary),
                                                           new Player(sf::Vector2f(max_coords_x - screen_size_x/2,screen_size_y/2+instanceOffset),DefaultResources::enemy_player,true,window,++count,portCount,!primary)));
                
                switch(checkConnectionMode()){
                    case Mode::Host:
                      {
                        //playerInstance.back().second->cbind();
                        playerInstance.back().first->sbind(true);
                        //std::cout << "ok" << std::endl;
                        break;
                      }
                    case Mode::Client:
                        //playerInstance.back().first->cbind();
                        playerInstance.back().second->sbind(false);
                        break;
                        
                } 
                
                
                instanceOffset+=200;
                getPrimaryPlayer().bind();
                
            }
        }

     void GameLogic::checkKill(){
            std::vector<std::shared_ptr<Projectile>> listOfProjectile;
            for(auto& instance:playerInstance){
                listOfProjectile.insert(listOfProjectile.end(),instance.first->projectile.begin(),instance.first->projectile.end());
                listOfProjectile.insert(listOfProjectile.end(),instance.second->projectile.begin(),instance.second->projectile.end());
            }
            for(auto& instance:playerInstance){
                for(auto& proj:listOfProjectile){
                    if(instance.first->collides(proj->getBounds()) and instance.first->getCount() != proj->getCount()){
                        instance.first->stopRendering();
                        proj->remove();
                        //proj->setPosition(sf::Vector2f(-100,-100));
                        update();
                    }
                    if(instance.second->collides(proj->getBounds()) and instance.second->getCount() != proj->getCount()){
                        instance.second->stopRendering();
                        proj->remove();
                        update();
                        //std::cout << "getting killed for no reason " << std::endl;
                    }
                }

            }
            
            
        }    
        void GameLogic::checkForGameOver(){
            if(!getPrimaryPlayer().isRendering()){ 
                if(availableInstance == maxInstance){
                    gameOverText.setPosition(getSecondaryPlayer().getPosition());
                    gameOverText.setText(std::string{"You lose"},75,sf::Color::Red);
                }
                addPlayerInstance();
            }else if(!getSecondaryPlayer().isRendering()){
                if(availableInstance == maxInstance){
                    gameOverText.setPosition(getPrimaryPlayer().getPosition());
                    gameOverText.setText(std::string{"You win"},75,sf::Color::Red);
                }
                addPlayerInstance();
            }
            
            
        }