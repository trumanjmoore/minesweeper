#include "../../Downloads/leaderboard_window.h"
#include <cmath>
#include <vector>
#include <chrono>
#pragma once


struct Tile{
    sf::Sprite tile_sprite;
    sf::Texture hidden_texture;
    sf::Texture revealed_texture;
    vector<Tile> tiles;
    bool flag = false;
    bool mine = false;
    bool hidden = true;
    vector<Tile*> neighbors;
    int neighbors_mines;

    Tile(sf::Texture &texture, float posx, float posy){
        tile_sprite.setTexture(texture);
        tile_sprite.setPosition(posx,posy);
        neighbors_mines = 0;
    }

    void Reveal(sf::Texture &texture){
        tile_sprite.setTexture(texture);
        hidden = false;
    }

    void drawTile(sf::RenderWindow &window) const{
        window.draw(tile_sprite);
    }

    bool checkBounds(float corx, float cory) const{
        auto bounds = tile_sprite.getGlobalBounds();
        if(bounds.contains(corx, cory))
            return true;
        else return false;
    }
    bool checkFlag() const{
        return flag;
    }
    bool checkMine() const{
        return mine;
    }
    void checkAdjacentMines(){
        for (int i = 0; i < neighbors.size(); ++i) {
            if (neighbors.at(i)->mine)
                neighbors_mines += 1;
        }
    }
};

struct MakeSprite{
    sf::Sprite _sprite;
    sf::Texture _texture;

    MakeSprite(string texture, sf::Vector2f pos){
        _texture.loadFromFile(texture);
        _sprite.setTexture(_texture);
        _sprite.setPosition(pos.x,pos.y);
    }

    MakeSprite(const sf::Texture& _texture, sf::Vector2f pos){
        _sprite.setTexture(_texture);
        _sprite.setPosition(pos.x,pos.y);
    }

    void SetRect(int x, int y){
        _sprite.setTextureRect(sf::IntRect (x,0,21,y));
    }

    bool checkBounds(float corx, float cory) const{
        auto bounds = _sprite.getGlobalBounds();
        if(bounds.contains(corx, cory))
            return true;
        else return false;
    }

    void changeTexture(string texture){
        _texture.loadFromFile(texture);
        _sprite.setTexture(_texture);
    }
};

float currentTime(std::chrono::time_point<std::chrono::system_clock> start, std::chrono::time_point<std::chrono::system_clock> now){
    return float(chrono::duration_cast <chrono::microseconds> (now - start).count());
}

void changeCounter(int num_mines, MakeSprite &one, MakeSprite &two, MakeSprite &three){
    if (num_mines<0)
        num_mines *= -1;
    if((num_mines)>99) {
        one.SetRect(21 * ((num_mines) % 10), 32);
        cout << (num_mines)%10<< endl;
        two.SetRect(21 * (((num_mines) / 10) % 10), 32);
        cout << ((num_mines)/10)%10<< endl;
        three.SetRect(21 * (((num_mines) / 100) % 10), 32);
        cout << ((num_mines)/100)%10<< endl;
    } else if((num_mines)>9) {
        one.SetRect(21 * ((num_mines) % 10), 32);
        two.SetRect(21 * ((num_mines) / 10), 32);
        three.SetRect(0, 32);
    } else {
        one.SetRect(21 * (num_mines), 32);
        two.SetRect(0, 32);
        three.SetRect(0, 32);
    }

}

void revealAdjacent(Tile &tile, sf::Texture &texture, vector<MakeSprite> &numbers, vector<sf::Texture> &numtexts) {
    if (tile.neighbors_mines == 0) {
        for (int j = 0; j < tile.neighbors.size(); ++j) {
            if (!tile.neighbors.at(j)->mine)
                if (!tile.neighbors.at(j)->flag)
                    if (tile.neighbors.at(j)->hidden) {
                        tile.neighbors.at(j)->Reveal(texture);
                        revealAdjacent(*tile.neighbors.at(j), texture, numbers, numtexts);
                    }
        }
    } else{
        int i = tile.neighbors_mines - 1;
        MakeSprite newnum(numtexts.at(i), tile.tile_sprite.getPosition());
        numbers.push_back(newnum);
    }
}

void make_tiles(vector<Tile> &tiles, int rows, int cols, sf::Texture &texture){
    for (int i = 0; i < (cols); i++) {
        for (int j = 0; j < rows; ++j) {
            Tile tile(texture, i * 32, j * 32);
            tiles.push_back(tile);
        }
    }
}

void make_mines(vector<MakeSprite> &mines, vector<Tile> &tiles, int num_mines, int rows, int cols, sf::Texture &texture ) {
    for (int i = 0; i < (num_mines); ++i) {
        float randomx = 32 * (rand() % cols);
        float randomy = 32 * (rand() % rows);
        MakeSprite mine(texture, sf::Vector2f(randomx, randomy));
        for (int j = 0; j < tiles.size(); ++j) {
            if (tiles.at(j).tile_sprite.getGlobalBounds().contains(randomx, randomy)) {
                if (tiles.at(j).mine) {
                    i--;
                    break;
                }
                tiles.at(j).mine = true;
            }
        }
        mines.push_back(mine);
    }
}

void check_adjacent(vector<Tile> &tiles, int rows, int cols){
    for (int i = 0; i < tiles.size(); i++) {
        vector<Tile*> adjacents;
        int check1;
        int check2;
        int check3;
        int check4;
        int check5;
        int check6;
        int check7;
        int check8;
        if(i%rows == 0) {
            check1 = - 1;
            check2 = - 1;
            check3 = i - (rows) + 1;
            check4 = i - (rows);
            check5 = i + (rows);
            check6 = - 1;
            check7 = i + 1;
            check8 = i + (rows) + 1;
        }
        else if (i%(rows) == (rows)-1){
            check1 = i - (rows) - 1;
            check2 = i - 1;
            check3 = -1;
            check4 = i - (rows);
            check5 = i + (rows);
            check6 = i + (rows) - 1;
            check7 = -1;
            check8 = -1;
        }
        else{
            check1 = i - (rows) - 1;
            check2 = i - 1;
            check3 = i - (rows) + 1;
            check4 = i - (rows);
            check5 = i + (rows);
            check6 = i + (rows) - 1;
            check7 = i + 1;
            check8 = i + (rows) + 1;
        }
        if(check1 >= 0 && check1 < tiles.size()){
            adjacents.push_back(&(tiles.at(check1)));        }
        if(check2 >= 0 && check2 < tiles.size()){
            adjacents.push_back(&(tiles.at(check2)));        }
        if(check3 >= 0 && check3 < tiles.size()){
            adjacents.push_back(&(tiles.at(check3)));        }
        if(check4 >= 0 && check4 < tiles.size()){
            adjacents.push_back(&(tiles.at(check4)));        }
        if(check5 >= 0 && check5 < tiles.size()){
            adjacents.push_back(&(tiles.at(check5)));        }
        if(check6 >= 0 && check6 < tiles.size()){
            adjacents.push_back(&(tiles.at(check6)));        }
        if(check7 >= 0 && check7 < tiles.size()){
            adjacents.push_back(&(tiles.at(check7)));        }
        if(check8 >= 0 && check8 < tiles.size()){
            adjacents.push_back(&(tiles.at(check8)));        }
        tiles.at(i).neighbors = adjacents;
        tiles.at(i).checkAdjacentMines();
    }
}

void RunGame(const string& playername) {
    srand((unsigned) time(NULL));

    sf::Clock timer;
    sf::Clock timer_check;
    float total_paused = 0;

    //Game Window
    ifstream config("files/board_config.cfg");
    string cols;
    string rows;
    int num_mines;
    string tempmines;
    getline(config, cols);
    getline(config, rows);
    getline(config, tempmines);
    sf::RenderWindow game_window(sf::VideoMode((stoi(cols)
    *32), (stoi(rows) * 32) + 100), "Minesweeper", sf::Style::Close);
    auto game_background = setRect(sf::Vector2f(stoi(cols) * 32, (stoi(rows) * 32 + 100)), 0, 0, 0, 0,
                                   sf::Color::White);
    bool lose = false;
    bool win = false;
    num_mines = stoi(tempmines);
    int real_num_mines = num_mines;

    sf::Texture hidden_texture;
    hidden_texture.loadFromFile("files/images/tile_hidden.png");
    sf::Texture revealed_texture;
    revealed_texture.loadFromFile("files/images/tile_revealed.png");
    sf::Texture flag_texture;
    flag_texture.loadFromFile("files/images/flag.png");
    sf::Texture mine_texture;
    mine_texture.loadFromFile("files/images/mine.png");

    vector<sf::Texture> numtexts;
    sf::Texture one;
    one.loadFromFile("files/images/number_1.png");
    numtexts.push_back(one);
    sf::Texture two;
    two.loadFromFile("files/images/number_2.png");
    numtexts.push_back(two);
    sf::Texture three;
    three.loadFromFile("files/images/number_3.png");
    numtexts.push_back(three);
    sf::Texture four;
    four.loadFromFile("files/images/number_4.png");
    numtexts.push_back(four);
    sf::Texture five;
    five.loadFromFile("files/images/number_5.png");
    numtexts.push_back(five);
    sf::Texture six;
    six.loadFromFile("files/images/number_6.png");
    numtexts.push_back(six);
    sf::Texture seven;
    seven.loadFromFile("files/images/number_7.png");
    numtexts.push_back(seven);
    sf::Texture eight;
    eight.loadFromFile("files/images/number_8.png");
    numtexts.push_back(eight);
    vector<MakeSprite> numbers;


    MakeSprite face("files/images/face_happy.png", sf::Vector2f(((stoi(cols) / 2) * 32) - 32, 32 * (stoi(rows) + .5f)));
    bool showleaders = false;
    MakeSprite debug("files/images/debug.png", sf::Vector2f((stoi(cols) * 32) - 304, 32 * (stoi(rows) + .5f)));
    bool debug_check = false;
    MakeSprite pause("files/images/pause.png", sf::Vector2f((stoi(cols) * 32) - 240, 32 * (stoi(rows) + .5f)));
    bool ispause = false;
    MakeSprite leaderboard("files/images/leaderboard.png",
                           sf::Vector2f((stoi(cols) * 32) - 176, 32 * (stoi(rows) + .5f)));
    bool isleaderboard = false;

    MakeSprite timermin1("files/images/digits.png", sf::Vector2f((stoi(cols) * 32) - 97, 32 * (stoi(rows) + .5f) + 16));
    MakeSprite timermin2("files/images/digits.png", sf::Vector2f((stoi(cols) * 32) - 76, 32 * (stoi(rows) + .5f) + 16));
    MakeSprite timersec1("files/images/digits.png", sf::Vector2f((stoi(cols) * 32) - 54, 32 * (stoi(rows) + .5f) + 16));
    MakeSprite timersec2("files/images/digits.png", sf::Vector2f((stoi(cols) * 32) - 33, 32 * (stoi(rows) + .5f) + 16));
    timermin1.SetRect(0, 32);
    timermin2.SetRect(0, 32);
    timersec1.SetRect(0, 32);
    timersec2.SetRect(0, 32);

    MakeSprite negative("files/images/digits.png", sf::Vector2f(12, 32 * (stoi(rows) + .5f) + 16));
    MakeSprite counter3("files/images/digits.png", sf::Vector2f(33, 32 * (stoi(rows) + .5f) + 16));
    MakeSprite counter2("files/images/digits.png", sf::Vector2f(54, 32 * (stoi(rows) + .5f) + 16));
    MakeSprite counter1("files/images/digits.png", sf::Vector2f(75, 32 * (stoi(rows) + .5f) + 16));
    negative.SetRect(210, 32);
    bool isnegative;
    if (num_mines < 0)
        isnegative = true;
    else
        isnegative = false;

    changeCounter(num_mines, counter1, counter2, counter3);

    vector<Tile> tiles;
    make_tiles(tiles, stoi(rows), stoi(cols), hidden_texture);

    vector<Tile> tiles_pause;
    make_tiles(tiles_pause, stoi(rows), stoi(cols), revealed_texture);

    vector<MakeSprite> mines;
    make_mines(mines, tiles, num_mines, stoi(rows), stoi(cols), mine_texture);

    check_adjacent(tiles, stoi(rows), stoi(cols));

    vector<MakeSprite> flags;

    while (game_window.isOpen()) {

        sf::Event event{};
        while (game_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                game_window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                auto coordinates = sf::Mouse::getPosition(game_window);
                if (!win && !lose) {
                    if ((sf::Mouse::isButtonPressed(sf::Mouse::Right))) {
                        for (int i = 0; i < tiles.size(); ++i) {
                            if (tiles.at(i).checkBounds(coordinates.x, coordinates.y)) {
                                if (!tiles.at(i).checkFlag()) {
                                    tiles.at(i).flag = true;
                                    MakeSprite flag(flag_texture, tiles.at(i).tile_sprite.getPosition());
                                    flags.push_back(flag);
                                    num_mines -= 1;
                                    changeCounter(num_mines, counter1, counter2, counter3);
                                    if (num_mines < 0)
                                        isnegative = true;
                                } else {
                                    for (int j = 0; j < flags.size(); ++j) {
                                        if (flags.at(j).checkBounds(coordinates.x, coordinates.y)) {
                                            tiles.at(i).flag = false;
                                            flags.erase(flags.begin() + j);
                                            num_mines += 1;
                                            changeCounter(num_mines, counter1, counter2, counter3);
                                            if (num_mines >= 0)
                                                isnegative = false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if ((sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
                        auto coordinates = sf::Mouse::getPosition(game_window);
                        for (int i = 0; i < tiles.size(); ++i) {
                            if (tiles.at(i).checkBounds(coordinates.x, coordinates.y)) {
                                if (!tiles.at(i).flag)
                                    if (tiles.at(i).mine) {
                                        face.changeTexture("files/images/face_lose.png");
                                        for (int j = 0; j < tiles.size(); ++j) {
                                            if (tiles.at(j).mine)
                                                tiles.at(j).Reveal(revealed_texture);
                                        }
                                        lose = true;
                                    } else {
                                        tiles.at(i).Reveal(revealed_texture);
                                        revealAdjacent(tiles.at(i), revealed_texture, numbers, numtexts);
                                    }
                            }
                        }
                    }
                    if (debug.checkBounds(coordinates.x, coordinates.y)) {
                        if (!debug_check)
                            debug_check = true;
                        else
                            debug_check = false;
                    }
                    if (pause.checkBounds(coordinates.x, coordinates.y)) {
                        if (!ispause) {
                            pause.changeTexture("files/images/play.png");
                            ispause = true;
                            timer_check.restart();
                        } else {
                            pause.changeTexture("files/images/pause.png");
                            total_paused += static_cast<float>(timer_check.getElapsedTime().asSeconds());
                            ispause = false;
                        }
                    }
                }
                if (leaderboard.checkBounds(coordinates.x, coordinates.y)) {
                    isleaderboard = true;
                    timer_check.restart();
                }
                if (face.checkBounds(coordinates.x, coordinates.y)) {
                    tiles.clear();
                    mines.clear();
                    flags.clear();
                    numbers.clear();
                    make_tiles(tiles, stoi(rows), stoi(cols), hidden_texture);
                    make_mines(mines, tiles, real_num_mines, stoi(rows), stoi(cols), mine_texture);
                    check_adjacent(tiles, stoi(rows), stoi(cols));
                    changeCounter(real_num_mines, counter1, counter2, counter3);
                    face.changeTexture("files/images/face_happy.png");
                    lose = false;
                    win = false;
                    debug_check = false;
                    num_mines = real_num_mines;
                    timer.restart();
                    timer_check.restart();
                    total_paused = 0;
                    if (ispause)
                        ispause = false;
                }
            }
            int num_revealed = 0;
            for (int i = 0; i < tiles.size(); ++i) {
                if (!tiles.at(i).hidden) {
                    num_revealed += 1;
                }
            }
            if (num_revealed != 0 && num_revealed == tiles.size() - real_num_mines) {
                if (!win) {
                    for (int i = 0; i < tiles.size(); ++i) {
                        if (tiles.at(i).mine) {
                            if (!tiles.at(i).flag)
                                tiles.at(i).flag = true;
                            MakeSprite flag(flag_texture, tiles.at(i).tile_sprite.getPosition());
                            flags.push_back(flag);
                            num_mines -= 1;
                            changeCounter(num_mines, counter1, counter2, counter3);
                        }
                    }
                }
                face.changeTexture("files/images/face_win.png");
                if (!win)
                    showleaders = true;
                win = true;
            }
        }

        game_window.clear();
        game_window.draw(game_background);
        game_window.draw(face._sprite);
        game_window.draw(debug._sprite);
        game_window.draw(pause._sprite);
        game_window.draw(leaderboard._sprite);

        if (isnegative)
            game_window.draw(negative._sprite);
        game_window.draw(counter1._sprite);
        game_window.draw(counter2._sprite);
        game_window.draw(counter3._sprite);


        if (isleaderboard) {
            for (int i = 0; i < tiles_pause.size(); ++i) {
                tiles_pause.at(i).drawTile(game_window);
            }
            game_window.draw(timermin1._sprite);
            game_window.draw(timermin2._sprite);
            game_window.draw(timersec1._sprite);
            game_window.draw(timersec2._sprite);
            game_window.display();
            if (!RunLeaderBoard(stoi(rows), stoi(cols))) {
                isleaderboard = false;
                if (!ispause)
                    total_paused += static_cast<float>(timer_check.getElapsedTime().asSeconds());
            }
        }

        if (lose) {
            for (int i = 0; i < tiles.size(); ++i) {
                tiles.at(i).drawTile(game_window);
            }
            for (int i = 0; i < flags.size(); ++i) {
                game_window.draw(flags.at(i)._sprite);
            }
            for (int i = 0; i < mines.size(); ++i) {
                game_window.draw(mines.at(i)._sprite);
            }
            for (int i = 0; i < numbers.size(); ++i) {
                game_window.draw(numbers.at(i)._sprite);
            }
        }
        else if (win) {
            for (int i = 0; i < tiles.size(); ++i) {
                tiles.at(i).drawTile(game_window);
            }
            for (int i = 0; i < flags.size(); ++i) {
                game_window.draw(flags.at(i)._sprite);
            }
            for (int i = 0; i < numbers.size(); ++i) {
                game_window.draw(numbers.at(i)._sprite);
            }
        } else if (ispause) {
            for (int i = 0; i < tiles_pause.size(); ++i) {
                tiles_pause.at(i).drawTile(game_window);
            }
        } else if (debug_check) {
            if (!win && !lose) {
                for (int i = 0; i < tiles.size(); ++i) {
                    tiles.at(i).drawTile(game_window);
                }
                for (int i = 0; i < mines.size(); ++i) {
                    game_window.draw(mines.at(i)._sprite);
                }
                for (int i = 0; i < flags.size(); ++i) {
                    game_window.draw(flags.at(i)._sprite);
                }
                for (int i = 0; i < numbers.size(); ++i) {
                    game_window.draw(numbers.at(i)._sprite);
                }
                float totalseconds = static_cast<float>(timer.getElapsedTime().asSeconds());
                totalseconds -= total_paused;
                int tensminutes = int(totalseconds / 60) / 10;
                int onesminutes = int(totalseconds / 60) % 10;
                int tensseconds = (int(totalseconds) % 60) / 10;
                int onesseconds = (int(totalseconds) % 60) % 10;
                timersec2.SetRect(onesseconds * 21, 32);
                timersec1.SetRect(tensseconds * 21, 32);
                timermin2.SetRect(onesminutes * 21, 32);
                timermin1.SetRect(tensminutes * 21, 32);
            }
        } else {
            for (int i = 0; i < tiles.size(); ++i) {
                tiles.at(i).drawTile(game_window);
            }
            for (int i = 0; i < flags.size(); ++i) {
                game_window.draw(flags.at(i)._sprite);
            }
            for (int i = 0; i < numbers.size(); ++i) {
                game_window.draw(numbers.at(i)._sprite);
            }
            float totalseconds = static_cast<float>(timer.getElapsedTime().asSeconds());
            totalseconds -= total_paused;
            int tensminutes = int(totalseconds / 60) / 10;
            int onesminutes = int(totalseconds / 60) % 10;
            int tensseconds = (int(totalseconds) % 60) / 10;
            int onesseconds = (int(totalseconds) % 60) % 10;
            timersec2.SetRect(onesseconds * 21, 32);
            timersec1.SetRect(tensseconds * 21, 32);
            timermin2.SetRect(onesminutes * 21, 32);
            timermin1.SetRect(tensminutes * 21, 32);
        }
        if (showleaders) {
            game_window.draw(timermin1._sprite);
            game_window.draw(timermin2._sprite);
            game_window.draw(timersec1._sprite);
            game_window.draw(timersec2._sprite);
            game_window.display();
            float totalseconds = static_cast<float>(timer.getElapsedTime().asSeconds());
            totalseconds -= total_paused;
            string tensmins = to_string(int(totalseconds / 60) / 10);
            string onesmins = to_string(int(totalseconds / 60) % 10);
            string tenssecs = to_string((int(totalseconds) % 60) / 10);
            string onessecs = to_string((int(totalseconds) % 60) % 10);
            string time = tensmins + onesmins + ":" + tenssecs + onessecs;
            if (!RunLeaderBoard(stoi(rows), stoi(cols), time, playername, true)) {
                showleaders = false;
            }
        }
        game_window.draw(timermin1._sprite);
        game_window.draw(timermin2._sprite);
        game_window.draw(timersec1._sprite);
        game_window.draw(timersec2._sprite);
        game_window.display();
    }

}
