#include "TileContainerView.hpp"
#include "Configuration.hpp"
void TileContainerView::drawConstraintRectangle(sf::Vector2f position , sf::Vector2f size,sf::Color color){
    constraintsRectangle.emplace_back(sf::RectangleShape(size));
    constraintsRectangle.back().setPosition(position);
    constraintsRectangle.back().setFillColor(color);
}


void TileContainerView::drawConstraintBackground(){
    drawConstraintRectangle(sf::Vector2f(0,0),sf::Vector2f(screen_size_x/2.f,max_coords_y),sf::Color(50,130,171,255));
    drawConstraintRectangle(sf::Vector2f(0,0),sf::Vector2f(max_coords_x,screen_size_y/2),sf::Color(50,130,171,255));
    drawConstraintRectangle(sf::Vector2f((max_coords_x-screen_size_x/2),0.f),sf::Vector2f(screen_size_x/2,(max_coords_y-screen_size_y/2)),sf::Color(50,130,171,255));
    drawConstraintRectangle(sf::Vector2f(0.f,(max_coords_y-screen_size_y/2)),sf::Vector2f(max_coords_x,screen_size_y/2.f),sf::Color(50,130,171,255));
}
void TileContainerView::drawGrids(){
    int countX{screen_size_y/2};
    int countY{screen_size_y/2};
    while(countX <= max_coords_x){
        drawConstraintRectangle(sf::Vector2f(countX,0),sf::Vector2f(20,max_coords_y),sf::Color(109,149,62,255));
        countX+=screen_size_y/2;
    }
    while(countY <= (max_coords_y-screen_size_y/2)){
        drawConstraintRectangle(sf::Vector2f(0,countY),sf::Vector2f(max_coords_x,20),sf::Color(109,149,62,255));
        countY += screen_size_y/2;
    }
}

void TileContainerView::checkProjectileCollision(Player& player,std::shared_ptr<Entity> entity){
    for(auto proj : player.projectile){
        
       if(entity->collides(proj->getBounds())){
           //std::cout << "projectile collided in position " << entity->getPosition().x << " " << entity->getPosition().y << std::endl;
           proj->remove();
       }
       
    }
}

 void TileContainerView::changeCenter(float newPosA,float newPosB){
            cameraView.setCenter(newPosA,newPosB);
            center.x = newPosA;
            center.y = newPosB;
}


void TileContainerView::processEvents(sf::Event& event){
        /* bool gainedFocus{false};
        if(event.type == sf::Event::GainedFocus){
            gainedFocus = true;
        } */
        
           
            fixCollision(gamePlayers.getPrimaryPlayer(),event);
            fixCollision(gamePlayers.getSecondaryPlayer(),event);
            //fixCollision(*player.second);
            sf::Vector2f prevPos{gamePlayers.getPrimaryPlayerPosition()};
            Player& player = gamePlayers.getPrimaryPlayer();
            Player& secPlayer = gamePlayers.getSecondaryPlayer();
            if(toMove(prevPos)){
                gamePlayers.getPrimaryPlayer().processEvent();
                changeCenter(gamePlayers.getPrimaryPlayer().getPosition().x, gamePlayers.getPrimaryPlayer().getPosition().y);
            
            }else{
                if(player.getPosition().x < screen_size_x/2){
                    player.setPosition(sf::Vector2f(screen_size_x/2,player.getPosition().y));
                }else if(player.getPosition().y < screen_size_y/2){
                    player.setPosition(sf::Vector2f(player.getPosition().x,screen_size_y/2));
                }else if(player.getPosition().x > (max_coords_x-screen_size_x/2)){
                    player.setPosition(sf::Vector2f((max_coords_x-screen_size_x/2),player.getPosition().y));
                }else if(player.getPosition().y > (max_coords_y-screen_size_y/2)){
                    player.setPosition(sf::Vector2f(player.getPosition().x,(max_coords_y-screen_size_y/2)));
                }
                
                 if(secPlayer.getPosition().x < screen_size_x/2){
                    secPlayer.setPosition(sf::Vector2f(screen_size_x/2,secPlayer.getPosition().y));
                }else if(secPlayer.getPosition().y < screen_size_y/2){
                    secPlayer.setPosition(sf::Vector2f(secPlayer.getPosition().x,screen_size_y/2));
                }else if(secPlayer.getPosition().x > (max_coords_x-screen_size_x/2)){
                    secPlayer.setPosition(sf::Vector2f((max_coords_x-screen_size_x/2),secPlayer.getPosition().y));
                }else if(secPlayer.getPosition().y > (max_coords_y-screen_size_y/2)){
                    secPlayer.setPosition(sf::Vector2f(secPlayer.getPosition().x,(max_coords_y-screen_size_y/2)));
                }
                
            
            }
            if(gamePlayers.playerInstance.size() > 1){
                //std::cout << "ok " << std::endl;
                for(auto i{gamePlayers.playerInstance.begin()};i<gamePlayers.playerInstance.end()-1;i++){
                    if(gamePlayers.checkConnectionMode() == Mode::Client){
                        sf::Vector2f previousPosition = (*i).second->getPosition();
                    if(toMoveApproximate(previousPosition))
                        //std::cout << "move is set to true" << std::endl;
                        (*i).second->processEvent();
                        //(*i).first->listenNetworkEvents();
                    }else{
                    
                        sf::Vector2f previousPosition = (*i).first->getPosition();
                    if(toMoveApproximate(previousPosition))
                        (*i).first->processEvent();
                        //(*i).second->listenNetworkEvents();
                    }
                }
            }
            //secPlayer.listenNetworkEvents();
            

           
        
        }