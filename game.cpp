#include "Framework.h"
#include <string>
#include <cstring>
#include <vector>
#include <time.h>
#include <algorithm>
#include <iostream>

class Platform{
public:
    Sprite* sprite;
    float x, y;
    bool hasBeenPassed = false;
    Platform(Sprite* sprite, float x, float y) {
        this->sprite = sprite;
        this->x = x;
        this->y = y;
    }
};

class Projectile {
public:
    Sprite* sprite;
    int x, y;
    float velocity;
    Projectile(Sprite* sprite, int x, int y, float velocity) {
        this->sprite = sprite;
        this->x = x;
        this->y = y;
        this->velocity = velocity;
    }
};

class Enemy{
public:
    Sprite* sprite;
    float x, y;
    bool alive = true;
    Enemy(Sprite* sprite, float x, float y) {
        this->sprite = sprite;
        this->x = x;
        this->y = y;
    }
};

class MyFramework : public Framework {

private:
    int width;
    int height;
    bool fullscreen;

    Sprite* bgk;

    Sprite* doodle;
    Sprite* doodleR;
    Sprite* doodleShoot;
    int doodleX, doodleY;
    int doodleHeight, doodleWidth;
    bool left = false;
    bool right = false;
    bool jump = false;
    bool shooting = false;
    int move = 1;
    int lastKey = 1; // 1 for left 2 for right

    Sprite* platform;
    int platformHeight, platformWidth;

    std::vector<Platform> platforms;

    Sprite* projectile;
    std::vector<Projectile> projectiles;

    float points;
    int numPlatforms; // height travelled and number of platforms

    float gravity = 0.2f;
    float velocity = 0.0f;

    int cameraY = 0;

    bool isPlaying = false;
    Sprite* playButton;
    int count = 0;

    Sprite* scores;
    Sprite* n0;
    Sprite* n1;
    Sprite* n2;
    Sprite* n3;
    Sprite* n4;
    Sprite* n5;
    Sprite* n6;
    Sprite* n7;
    Sprite* n8;
    Sprite* n9;

    Sprite* title;

    Sprite* enemy;
    std::vector<Enemy> enemies;

public:
    MyFramework(int width, int height, bool fullscreen){
        this->width = width;
        this->height = height;
        this->fullscreen = fullscreen;

        bgk = nullptr;
        doodle = nullptr;
        platform = nullptr;
    }

    virtual void PreInit(int& width, int& height, bool& fullscreen)
    {
        width = this->width;
        height = this->height;
        fullscreen = false;
    }

    virtual bool Init() {

        bgk = createSprite("bck.png");
        if (!bgk) {
            return false;
        }

        getScreenSize(width, height);
        setSpriteSize(bgk, width, height);

        doodle = createSprite("blue-lik-left.png");
        if (!doodle) {
            return false;
        }

        doodleR = createSprite("blue-lik-right.png");
        if(!doodleR){
            return false;
        }

        doodleShoot = createSprite("bunny-puca.png");
        if(!doodleShoot){
            return false;
        }

        getSpriteSize(doodle, doodleWidth, doodleHeight);

        platform = createSprite("platform.png");
        if (!platform) {
            return false;
        }
        getSpriteSize(platform, platformWidth, platformHeight);


        projectile = createSprite("projectile.png");
        if(!projectile){
            return false;
        }

        if(!count) {
            int gap = height / 8;
            srand(time(0));
            for (int i = 0; i < 10; i++) {
                int j = (rand() % 2) + 1;
                if (j == 2) {
                    int x1 = rand() % (width / 2 - platformWidth);
                    int x2 = rand() % (width - platformWidth - width / 2) + width / 2;
                    int y = height - i * gap;
                    Platform p1(platform, x1, y);
                    Platform p2(platform, x2, y);
                    platforms.push_back(p1);
                    platforms.push_back(p2);
                } else if (j == 1) {
                    int x = rand() % (width - platformWidth);
                    int y = height - i * gap;
                    Platform p(platform, x, y);
                    platforms.push_back(p);
                }
            }
        }

        for(auto& platform : platforms){
            if(platform.y < height / 4){
                doodleX = platform.x;
                break;
            }
        }

        doodleY = height - doodleHeight * 8;
        cameraY = doodleY;

        playButton = createSprite("play.png");
        if (!playButton) {
            return false;
        }

        scores = createSprite("score.png");

        if(!scores){
            return false;
        }

        isPlaying = false;

        numPlatforms = 0;
        points = 0;

        n0 = createSprite("0.png");
        if(!n0){
            return false;
        }

        n1 = createSprite("1.png");
        if(!n1){
            return false;
        }

        n2 = createSprite("2.png");
        if(!n2){
            return false;
        }

        n3 = createSprite("3.png");
        if(!n3){
            return false;
        }

        n4 = createSprite("4.png");
        if(!n4){
            return false;
        }

        n5 = createSprite("5.png");
        if(!n5){
            return false;
        }

        n6 = createSprite("6.png");
        if(!n6){
            return false;
        }

        n7 = createSprite("7.png");
        if(!n7){
            return false;
        }

        n8 = createSprite("8.png");
        if(!n8){
            return false;
        }

        n9 = createSprite("9.png");
        if(!n9){
            return false;
        }

        title = createSprite("doodle-jump.png");
        if(!title){
            return false;
        }

        return true;
    }

    virtual void Close() {
        destroySprite(bgk);
        destroySprite(doodle);
        destroySprite(platform);
        destroySprite(playButton);
    }

    virtual bool Tick() {
        if (isPlaying) {
            if(numPlatforms % 50 == 0 && numPlatforms > 0){
                Enemy e(enemy, platforms.front().x, platforms.front().y - platformHeight);
                enemies.push_back(e);
            }

            doodleY += static_cast<int>(velocity) / 2;
            if (velocity < 2)
                velocity += gravity;
            cameraY = doodleY;

            for (auto &p: platforms) {
                p.y += 0.2;
                if(doodleY < p.y && !p.hasBeenPassed){
                    ++numPlatforms;
                    p.hasBeenPassed = true;
                }
            }

            for(auto &e : enemies){
                if(e.alive)
                    e.y += 0.2;
            }

            for (auto &pr : projectiles) {
                pr.y -= 3;
            }

            points += 0.01;

            for (auto &p: platforms) {
                if (p.y >= height + height / 4) {
                    if (p.x <= width / 2) {
                        int x = rand() % (width / 2 - platformWidth);
                        int y = 0;
                        p.x = x;
                        p.y = y;
                    } else {
                        int x = rand() % (width - platformWidth - width / 2) + width / 2;
                        int y = 0;
                        p.x = x;
                        p.y = y;
                    }
                    p.hasBeenPassed = false;
                }
            }

            if (left) {
                doodleX -= move;
                if (doodleX < -doodleWidth * 3 / 4) {
                    doodleX = width;
                }
            }

            if (right) {
                doodleX += move;
                if (doodleX > width) {
                    doodleX = 0;
                }
            }

            if (jump) {
                velocity = -4.0f;
            }

            bool onPlatform = false;
            for (auto p: platforms) {
                if (doodleY + doodleHeight >= p.y && doodleY + doodleHeight <= p.y + platformHeight
                    && doodleX + doodleWidth / 2 >= p.x && doodleX <= p.x + platformWidth / 2) {
                    onPlatform = true;
                    break;
                }
            }

            if (onPlatform) {
                velocity = -9.0f;
            }


            drawSprite(bgk, 0, 0);
            drawSprite(scores, 0, 0);

            int nrc = 0,aux = static_cast<int>(points), last = 0;

            while(aux != 0){
                int c = aux % 10;
                aux /= 10;
                switch(c){
                    case 0:
                        drawSprite(n0, width / 2 - nrc * 15, 0);
                    case 1:
                        drawSprite(n1, width / 2 - nrc * 15, 0);
                    case 2:
                        drawSprite(n2, width / 2 - nrc * 15, 0);
                    case 3:
                        drawSprite(n3, width / 2 - nrc * 15, 0);
                    case 4:
                        drawSprite(n4, width / 2 - nrc * 15, 0);
                    case 5:
                        drawSprite(n5, width / 2 - nrc * 15, 0);
                    case 6:
                        drawSprite(n6, width / 2 - nrc * 15, 0);
                    case 7:
                        drawSprite(n7, width / 2 - nrc * 15, 0);
                    case 8:
                        drawSprite(n8, width / 2 - nrc * 15, 0);
                    case 9:
                        drawSprite(n9, width / 2 - nrc * 15, 0);
                }
                last = width / 2 - nrc * 15;
                ++nrc;
            }

            int nrc2 = 0,aux2 = static_cast<int>(numPlatforms);

            while(aux2 != 0){
                int c = aux % 10;
                aux2 /= 10;
                switch(c){
                    case 0:
                        drawSprite(n0, last - nrc * 15, 0);
                    case 1:
                        drawSprite(n1, last - nrc * 15, 0);
                    case 2:
                        drawSprite(n2, last - nrc * 15, 0);
                    case 3:
                        drawSprite(n3, last - nrc * 15, 0);
                    case 4:
                        drawSprite(n4, last - nrc * 15, 0);
                    case 5:
                        drawSprite(n5, last - nrc * 15, 0);
                    case 6:
                        drawSprite(n6, last - nrc * 15, 0);
                    case 7:
                        drawSprite(n7, last - nrc * 15, 0);
                    case 8:
                        drawSprite(n8, last - nrc * 15, 0);
                    case 9:
                        drawSprite(n9, last - nrc * 15, 0);
                }
                ++nrc2;
            }

            for (auto p: platforms) {
                drawSprite(p.sprite, p.x, p.y);
            }

            for(auto pr : projectiles) {
                drawSprite(pr.sprite, pr.x, pr.y);
            }

            for(auto e : enemies){
                drawSprite(e.sprite, e.x, e.y);
            }

            if (shooting) {
                drawSprite(doodleShoot, doodleX, doodleY);
            } else if (right || (!left && lastKey == 2)) {
                drawSprite(doodleR, doodleX, doodleY);
            } else {
                drawSprite(doodle, doodleX, doodleY);
            }

//            drawSprite(points2, width / 10, height / 20);

            std::cout << numPlatforms << " " << points << std::endl;

            if (doodleY > height) {
                isPlaying = false;
                ++count;
                Init();
            }
            return false;
        } else {
            if (count) {
                for (auto &p: platforms) {
                    if (p.y >= height + height / 4) {
                        if (p.x <= width / 2) {
                            int x = rand() % (width / 2 - platformWidth);
                            int y = 0;
                            p.x = x;
                            p.y = y;
                        } else {
                            int x = rand() % (width - platformWidth - width / 2) + width / 2;
                            int y = 0;
                            p.x = x;
                            p.y = y;
                        }
                    }
                }
                count--;
            }
            drawSprite(bgk, 0, 0);
            drawSprite(playButton, width / 3, height / 2);
            drawSprite(title, width / 4, height / 3);
            return false;
        }
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
        if(isPlaying) {
            if (button == FRMouseButton::LEFT) {
                shooting = true;
                Projectile p(projectile, doodleX + doodleWidth / 3, doodleY - doodleHeight / 2, -3.0);
                projectiles.push_back(p);
            }
            if (isReleased)
                shooting = false;
        } else {
            if (button == FRMouseButton::LEFT) {
                isPlaying = true;
            }
        }
    }

    virtual void onKeyPressed(FRKey k) {
        if(isPlaying) {
            if (k == FRKey::LEFT) {
                left = true;
                lastKey = 1;
            }

            if (k == FRKey::RIGHT) {
                right = true;
                lastKey = 2;
            }

            if(k == FRKey::UP){
                jump = true;
            }
        }
    }

    virtual void onKeyReleased(FRKey k) {
        if(isPlaying) {
            if (k == FRKey::LEFT) {
                left = false;
            }

            if (k == FRKey::RIGHT) {
                right = false;
            }

            if(k == FRKey::UP){
                jump = false;
            }
        }
    }

    virtual const char* GetTitle() override
    {
        return "Doodle Jump";
    }
};

int main(int argc, char *argv[])
{

    int width = 400;
    int height = 580;

    for(int i = 1; i < argc; i++){
        if(std::strcmp(argv[i], "-window") == 0){
            if(i + 1 < argc){
                std::string str(argv[i + 1]);
                size_t Pos = str.find('x');
                if(Pos != std::string::npos){
                    width = std::stoi(str.substr(0, Pos));
                    height = std::stoi(str.substr(Pos + 1, std::string::npos));
                }
            }
        }
    }

    return run(new MyFramework(width, height, false));
}