#include "shapes.h"
#include "fstream"
#pragma once

struct Leaders{
    string name;
    string time;
    Leaders(fstream &file){
        getline(file, time, ',');
        getline(file, name);
    }
    void ChangeLeader(string newtime, string newname){
        name = std::move(newname);
        time = std::move(newtime);
    }
};

string makeTexts(vector<Leaders> &leading){
    return "1.\t" + leading.at(0).time + "\t" + leading.at(0).name + "\n\n"
    + "2.\t" + leading.at(1).time + "\t" + leading.at(1).name + "\n\n"
    + "3.\t" + leading.at(2).time + "\t" + leading.at(2).name + "\n\n"
    + "4.\t" + leading.at(3).time + "\t" + leading.at(3).name + "\n\n"
    + "5.\t" + leading.at(4).time + "\t" + leading.at(4).name + "\n\n";
}

bool RunLeaderBoard(int rows, int cols, const string& time = "99:99", const string& playersname = "", bool newplayer = false) {
    int playerpos;
    int width = cols*16;
    int height = (rows*16)+50;
    fstream leaders("files/leaderboard.txt");
    vector<Leaders> leading;
    Leaders line1(leaders);
    Leaders line2(leaders);
    Leaders line3(leaders);
    Leaders line4(leaders);
    Leaders line5(leaders);
    leading.push_back(line1);
    leading.push_back(line2);
    leading.push_back(line3);
    leading.push_back(line4);
    leading.push_back(line5);

    for(int i = 0; i < leading.size(); i++){
        if (time < leading.at(i).time){
            for (int j = leading.size()-1; j > i; j--) {
                leading.at(j).ChangeLeader(leading.at(j-1).time, leading.at(j-1).name);
            }
            playerpos = i;
            leading.at(i).ChangeLeader(time, playersname+ '*');
            break;
        }
    }
    string text = makeTexts(leading);
    ofstream newleaders;
    newleaders.open("files/leaderboard.txt", std::ofstream::out | std::ofstream::trunc);
    for(int i = 0; i < leading.size(); i++){
        newleaders << leading.at(i).time + "," + leading.at(i).name + "\n";
    }
    newleaders.close();

    sf::RenderWindow leaderboard_window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    auto leaderboard_background = setRect(sf::Vector2f(width, height), 0, 0, 0, 0, sf::Color::Blue);

    //Leaderboard text
    Text leaderboard_text(sf::Color::White, 20, "LEADERBOARD", width/2.0f, height/2.0f-120, sf::Text::Bold, sf::Text::Underlined);
    Text text1(sf::Color::White, 18, text, width/2.0f, (height/2.0f)+20, sf::Text::Bold);


    while (leaderboard_window.isOpen())
    {
        sf::Event event{};
        while (leaderboard_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                if(newplayer){
                    leading.at(playerpos).ChangeLeader(time, playersname);
                    text = makeTexts(leading);
                    newleaders.open("files/leaderboard.txt", std::ofstream::out | std::ofstream::trunc);
                    for(int i = 0; i < leading.size(); i++){
                        newleaders << leading.at(i).time + "," + leading.at(i).name + "\n";
                    }
                }
                leaderboard_window.close();
                return false;
            }
        }
        leaderboard_window.clear();
        leaderboard_window.draw(leaderboard_background);
        leaderboard_window.draw(leaderboard_text.texts);
        leaderboard_window.draw(text1.texts);
        leaderboard_window.display();
    }
}
