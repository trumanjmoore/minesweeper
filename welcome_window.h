#include "game_window.h"
#pragma once

string RunWelcome(){
    ifstream config("files/board_config.cfg");
    string cols;
    string rows;
    getline(config, cols);
    getline(config, rows);
    float width = stoi(cols)*32;
    float height = stoi(rows)*32+100;

    //Welcome texts
    Text welcome_text(sf::Color::White, 24, "WELCOME TO MINESWEEPER!", width/2.0f, height/2.0f-150, sf::Text::Bold, sf::Text::Underlined);

    //Asking for user input
    Text input_prompt(sf::Color::White, 24, "Enter you name:", width/2.0f, height/2.0f-75, sf::Text::Bold);

    //User input texts
    Text player_text(sf::Color::Yellow, 18, "|", width/2.0f, height/2.0f-45, sf::Text::Bold);

    char playerInput;
    string playerString = "|";

    //Welcome Window
    sf::RenderWindow welcome_window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    auto welcome_background = setRect(sf::Vector2f(width, height), 0, 0, 0, 0, sf::Color::Blue);

    while (welcome_window.isOpen())
    {
        sf::Event event{};
        while (welcome_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                welcome_window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                playerInput = event.text.unicode;
                if (isalpha(playerInput)) {
                    if (player_text.texts.getString() == "|") {
                        playerInput = toupper(playerInput);
                    } else {
                        playerInput = tolower(playerInput);
                    }
                    if(playerString.length() < 11) {
                        playerString.pop_back();
                        playerString += playerInput;
                        playerString += "|";
                        player_text.texts.setString(playerString);
                    }
                }
                if(playerString.length() > 1) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
                        playerString.pop_back();
                        playerString.pop_back();
                        playerString += "|";
                        player_text.texts.setString(playerString);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                        welcome_window.close();
                        playerString.pop_back();
                        RunGame(playerString);
                    }
                }
            }
        }
        player_text.setText(player_text.texts, width/2.0f, height/2.0f-45);
        
        welcome_window.clear();
        welcome_window.draw(welcome_background);
        welcome_window.draw(welcome_text.texts);
        welcome_window.draw(input_prompt.texts);
        welcome_window.draw(player_text.texts);
        welcome_window.display();
    }
}
