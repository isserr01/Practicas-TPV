#include "MainMenuState.h"
#include "Button.h"
#include "Label.h"
#include "game.h"
#include "PlayState.h"
#include <iostream>
#include <algorithm>

MainMenuState::MainMenuState(Game* game)
    : GameState(game), index(0), mapButton(nullptr)
{
    
    createButtons();
    title = new Label(this, game->getTexture(Game::ELIGE_UN_MAPA), 100.0f, 200.0f);
}

MainMenuState::~MainMenuState() {
    delete title;
    delete leftArrow;
    delete rightArrow;
    if (mapButton) delete mapButton;
    gameObjects.clear();
}

void MainMenuState::createButtons() {
    
    leftArrow = new Button(this, game->getTexture(Game::ARROWL), 30.f, 300.f);
    rightArrow = new Button(this, game->getTexture(Game::ARROWR), 400.f, 300.f);

    leftArrow->connect([this]() { switchMap(-1); });
    rightArrow->connect([this]() { switchMap(1); });

    addObject(leftArrow);
    addObject(rightArrow);

   
    createMapButton();
}

void MainMenuState::createMapButton() {

    if (mapButton) {
        removeObject(mapButton);             
        removeEventListener(mapButton);      
        delete mapButton;
        mapButton = nullptr;
    }



    Game::TextureName tex;
    switch (index) {
    case 0: tex = Game::AVISPADO; break;
    case 1: tex = Game::ORIGINAL; break;
    case 2: tex = Game::PRACTICA1; break;
    case 3: tex = Game::TRIVIAL; break;
    case 4: tex = Game::VELOZ; break;
    }

    mapButton = new Button(this, game->getTexture(tex), 150.f, 300.f);
    mapButton->connect([this]() {
        game->popState();
        switch (index) {
        case 0: game->pushState(std::make_shared<PlayState>(game, "Avispado.txt")); break;
        case 1: game->pushState(std::make_shared<PlayState>(game, "Original.txt")); break;
        case 2: game->pushState(std::make_shared<PlayState>(game, "Práctica1.txt")); break;
        case 3: game->pushState(std::make_shared<PlayState>(game, "Trivial.txt")); break;
        case 4: game->pushState(std::make_shared<PlayState>(game, "Veloz.txt")); break;
        }
        });

    addObject(mapButton); 
}


void MainMenuState::switchMap(int dir) {
    index += dir;
    if (index < 0) index = 4; 
    if (index > 4) index = 0; 
    createMapButton();
}

void MainMenuState::render() const {
    SDL_RenderClear(game->getRenderer());
    game->getTexture(Game::MAINMENU_BACKGROUND)->render();

    leftArrow->render();
    rightArrow->render();
    if (mapButton) mapButton->render();
    title->render();

    SDL_RenderPresent(game->getRenderer());
}

void MainMenuState::update() {
    leftArrow->update();
    rightArrow->update();
    if (mapButton) mapButton->update();
}

void MainMenuState::handleEvent(const SDL_Event& e) {
    GameState::handleEvent(e);
}
