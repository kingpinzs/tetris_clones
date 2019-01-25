#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <iostream>

using namespace sf;

const int M = 20; //grid hight
const int N = 10; //grid width
//when resoution changes
const int scaleFactor = 18 * 3;
const float zoomLevel = 1.0 *3;
const int fieldOffsetX = 28*3;
const int fieldOffsetY = 31*3;
//const int scaleFactorX = 36;

int field[M][N] = {0}; //the grid zeroed out

int score = 0;
int level = 0;

struct Point
{
    int x, y;
} pieace[4], b[4];

int figures[7][4] =
{
    1,3,5,7, //I
    2,4,5,7, //Z
    3,5,4,6, //S
    3,5,4,7, //T
    2,3,5,7, //L
    3,5,7,6, //J
    2,3,4,5, //O
};

bool check()
{
    for (int i=0; i<4; i++)
    {
        if (pieace[i].x<0 || pieace[i].x>=N || pieace[i].y>=M)
        {
            return 0;
        }
        else if (field[pieace[i].y][pieace[i].x])
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    srand(time(0));
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(960, 1420), "Falling Blocks clone");

    Font font;
    font.loadFromFile("Coiny-Regular.ttf");
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("0000");
    scoreText.setCharacterSize(72);
    scoreText.setFillColor(Color::Yellow);
    scoreText.setPosition(1280/2,-1);
    // Load a sprite to display
    sf::Texture tilesTexture, backgroundTexture, forgroundTexture;
    if (!tilesTexture.loadFromFile("tiles.png"))
        return EXIT_FAILURE;
    if (!backgroundTexture.loadFromFile("background.png"))
        return EXIT_FAILURE;
    if (!forgroundTexture.loadFromFile("frame.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite(tilesTexture), background(backgroundTexture), frame(forgroundTexture);

    sprite.scale(sf::Vector2f(zoomLevel, zoomLevel));//make the images bigger so I can see them on the 4k screen
    background.scale(Vector2f(zoomLevel,zoomLevel));
    frame.scale(Vector2f(zoomLevel,zoomLevel));

    int dx = 0;
    bool rotate = 0;
    int colorNum = 1;
    float timer = 0, delay = 0.3;

    Clock clock;

    //create first pieace
    int n=rand()%7;
    for(int i=0; i<4; i++)
    {
        pieace[i].x = figures[n][i] % 2;
        pieace[i].y = figures[n][i] / 2;
    }

    // Start the game loop
    while (app.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up)
                    rotate = true;
                else if (event.key.code == Keyboard::Left)
                    dx = -1;
                else if (event.key.code == Keyboard::Right)
                    dx = 1;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            delay = 0.05;
        }

        //move
        for (int i=0; i<4; i++)
        {
            b[i] = pieace[i];
            pieace[i].x += dx;
        }
        if (!check())
        {
            for (int i=0; i<4; i++)
            {
                pieace[i] = b[i];
            }
        }


        //rotate
        if (rotate)
        {
            Point p = pieace[1];
            for (int i=0; i<4; i++)
            {
                int x = pieace[i].y - p.y;
                int y = pieace[i].x - p.x;
                pieace[i].x = p.x - x;
                pieace[i].y = p.y + y;
            }
            if (!check())
            {
                for (int i=0; i<4; i++)
                {
                    pieace[i]=b[i];
                }
            }
        }

        //tick
        if (timer>delay)
        {
            for (int i=0 ; i<4; i++)
            {
                b[i]=pieace[i];
                pieace[i].y+=1;
            }
            if (!check())
            {
                colorNum=1+rand()%7;
                for (int i=0; i<4; i++)
                {
                    field[b[i].y][b[i].x]=colorNum;
                }
                int n=rand()%7;

                for(int i=0; i<4; i++)
                {
                    pieace[i].x = figures[n][i] % 2;
                    pieace[i].y = figures[n][i] / 2;
                }
            }
            timer=0;
        }

        //check lines
        int k = M - 1;
        for (int i=M-1; i>0; i--) {
            int count=0;
            for (int j=0; j<N; j++) {
                if (field[i][j]) {
                    count++;
                }
                field[k][j]=field[i][j];
            }
            if(count<N) {
                k--;
            }
        }
        scoreText.setString(std::to_string(score));

        dx = 0;
        rotate = 0;
        delay = 0.3;
        //draw
        // Clear screen
        app.clear(Color::White);
        app.draw(background);

        //what does this do?????
        for (int i=0; i<M; i++)
        {
            for (int j=0; j<N; j++)
            {
                if (field[i][j] == 0)
                {
                    continue;
                }

                sprite.setTextureRect(IntRect(field[i][j]*18,0,18,18));
                sprite.setPosition(j*scaleFactor,i*scaleFactor);
                sprite.move(fieldOffsetX,fieldOffsetY);
                app.draw(sprite);
            }
        }

        for (int i=0; i<4; i++)
        {
            sprite.setTextureRect(IntRect(colorNum*18,0,18,18));
            sprite.setPosition(pieace[i].x*scaleFactor, pieace[i].y*scaleFactor);
            sprite.move(fieldOffsetX,fieldOffsetY); //offset
            app.draw(sprite);
        }

        app.draw(frame);
        app.draw(scoreText);
        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
