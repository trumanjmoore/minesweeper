#include <iostream>
#include <SFML/Graphics.hpp>
#pragma once

using namespace std;

struct Text{
    sf::Font font;
    sf::Text texts;

    Text(sf::Color color, int size, const string& output, float posx, float posy, sf::Text::Style style1){
        font.loadFromFile("files/font.ttf");
        texts.setFont(font);
        texts.setFillColor(color);
        texts.setCharacterSize(size);
        texts.setString(output);
        texts.setStyle(style1);
        setText(texts, posx, posy);
    }

    Text(sf::Color color, int size, const string& output, float posx, float posy, sf::Text::Style style1, sf::Text::Style style2){
        font.loadFromFile("files/font.ttf");
        texts.setFont(font);
        texts.setFillColor(color);
        texts.setCharacterSize(size);
        texts.setString(output);
        texts.setStyle(style1 | style2);
        setText(texts, posx, posy);
    }

    static bool setText(sf::Text &text, float x, float y) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

};

sf::RectangleShape setRect(sf::Vector2f size, float posx, float posy, float originx, float originy, sf::Color color){
    sf::RectangleShape rect;
    rect.setSize(size);
    rect.setPosition(posx,posy);
    rect.setOrigin(originx,originy);
    rect.setFillColor(color);
    return rect;
}
