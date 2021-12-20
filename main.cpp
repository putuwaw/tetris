#include <bits/stdc++.h>
#include "SFML/Graphics.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/System/Clock.hpp"

using namespace std;
using namespace sf;

const int P = 15;
const int L = 17;

int arena[L][P], warnaKotak, ubahX, warnaKotak2, score = 0, sfxoff = 0;
bool pos1, pos2, pos3, pos4, rotasi, pause = false, lose = false;
float timer = 0, delay = 0.5, waktu;

RenderWindow jendelaUtama, jendelaGame, jendelaSetting, jendelaAbout;
Music musik, sfxPoint;
Texture tGame, tFrameGame, tKotak, tLose, tPause;
Clock c;

struct titik{
    int x;
    int y;
}a[4], b[4], pic2[4];

int bentuk[7][4]={
    {1,3,5,7},// I
    {1,2,3,4},// S
    {2,4,5,7},// Z
    {1,2,3,5},// T
    {2,4,6,7},// L
    {1,3,4,5},// J
    {2,3,4,5} // O
};

bool aman(){
    for (int i = 0; i < 4; i++){
        if (a[i].x >= P || a[i].x < 0 || a[i].y >= L){
            return 0;
        }
        else if(arena[a[i].y][a[i].x]){
            return 0;
        }
    }
    return 1;
}

bool gameOver(){
    for (int j = 0; j < 4; j++){
        for (int i = 0; i < P; i++){
            if (arena[j][i] != 0){
                return 1;
            }
        }
    }
    return 0;
}

void resetAll(){
    score = 0, timer = 0;
    for (int i = 0; i < L; i++){
        for (int j = 0; j < P; j++){
			arena[i][j] = 0;
        }
    }
}

void prepare(){
    warnaKotak = rand()%9 + 1;
    int n = rand()%7;
    for (int i = 0; i < 4; i++){
        a[i].x = bentuk[n][i] % 2;
        a[i].y = bentuk[n][i] / 2;
    }
    warnaKotak2 = rand()%9 + 1;
    int m = rand()%7;
    for (int i = 0; i < 4; i++){
        pic2[i].x = bentuk[m][i] % 2;
        pic2[i].y = bentuk[m][i] / 2;
    }
}

void input(){
    float waktu = c.getElapsedTime().asSeconds();
    c.restart();
    timer += waktu;
    Event eventGame;
    while(jendelaGame.pollEvent(eventGame)){
        if (eventGame.type==Event::Closed){
            jendelaGame.close();
        }
        else{
	        // INPUT USER
	        if (Keyboard::isKeyPressed(Keyboard::Down)){
	            delay = 0.05;
	        }
	        else if (Keyboard::isKeyPressed(Keyboard::Right)){
	            ubahX = 1;
	        }
	        else if (Keyboard::isKeyPressed(Keyboard::Left)){
	            ubahX = -1;
	        }
	        else if (Keyboard::isKeyPressed(Keyboard::Up)){
	            rotasi = true;
	        }
	        else if (Keyboard::isKeyPressed(Keyboard::P)){
	            pause = true;
	        }
	        else if (Keyboard::isKeyPressed(Keyboard::Enter)){
	            pause = false;
	        }
	        else if (Keyboard::isKeyPressed(Keyboard::Y)){
	            lose = false;
	            resetAll();
	        }
	        else if (Keyboard::isKeyPressed(Keyboard::N)){
	            jendelaGame.close();
	        }
    	}
    }
}

void update(){
	// GERAK KIRI - KANAN
    for (int i = 0; i < 4; i++){
        a[i].x += ubahX;
    }
    if (!aman()){
        for (int i = 0; i < 4; i++){
            a[i].x -= ubahX;
        }
    }
    // GERAK ROTASI
    if (rotasi){
        titik p = a[1];
        for (int i = 0; i < 4; i++){
            b[i] = a[i];
            int x = a[i].y-p.y;
            int y = a[i].x-p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
        if (!aman()){
            for (int i = 0; i < 4; i++){
                a[i] = b[i];
            }
        }
    }
    // GERAK TURUN
    if (timer > delay){
        for (int i = 0; i < 4; i++){
            a[i].y += 1;
        }
        if (!aman()){
            for (int i = 0; i < 4; i++){
                a[i].y -= 1;
                arena[a[i].y][a[i].x] = warnaKotak;
            }
            // PAKAI BENTUK DI PIC 2
            warnaKotak = warnaKotak2;
            for (int i = 0; i < 4; i++){
                a[i].x = pic2[i].x;
                a[i].y = pic2[i].y;
            }
            // BUAT BENTUK BARU
            warnaKotak2 = rand()%9 + 1;
            int m = rand()%7;
            for (int i = 0; i < 4; i++){
                pic2[i].x = bentuk[m][i] % 2;
                pic2[i].y = bentuk[m][i] / 2;
            }
        }
        timer = 0;
    }
    // RESET
    delay = 0.5;
    ubahX = 0;
    rotasi = false;
    // HAPUS LINE
    int temp = L-1;
    for (int i = L-1; i > 0; i--){
        int counter = 0;
        for (int j = 0; j < P; j++){
            if (arena[i][j]){
                counter++;
            }
            arena[temp][j] = arena[i][j];
        }
        if (counter == P){
            if (sfxoff == 0){
                sfxPoint.play();
                sfxPoint.setPlayingOffset(seconds(0.5));
            }
        score += 100;
        }
        else if (counter < P){
            temp--;
        }
    }
}

void draw(){
    tGame.loadFromFile("asset//bg-game.png");
    tKotak.loadFromFile("asset//kotak.png");
    tFrameGame.loadFromFile("asset//frame-game.png");
    tLose.loadFromFile("asset//lose.png");
    Sprite notifLose(tLose);
    tPause.loadFromFile("asset//pause.png");
    Sprite notifPause(tPause);
    Sprite bgGame(tGame), frame(tFrameGame), kotak(tKotak);

    jendelaGame.draw(bgGame);
    // GAMBAR ARENA
    for (int i = 0; i < L; i++){// ke bawah sebanyak Lebar
        for (int j = 0; j < P; j++){ // ke kanan sebanyak Panjang
            kotak.setTextureRect(IntRect(arena[i][j]*39,0,39,39));
            kotak.setPosition(j*39,i*39);
            kotak.move(363,10);
            jendelaGame.draw(kotak);
        }
    }
    // GAMBAR KOTAK DI ARENA
    for (int i = 0; i < 4; i++){
        kotak.setTextureRect(IntRect(warnaKotak*39,0,39,39));
        kotak.setPosition(a[i].x*39,a[i].y*39);
        kotak.move(363,10);
        jendelaGame.draw(kotak);
    }
    // GAMBAR KOTAK YG AKAN MUNCUL
    for (int i = 0; i < 4; i++){
        kotak.setTextureRect(IntRect(warnaKotak2*39,0,39,39));
        kotak.setPosition(pic2[i].x*39,pic2[i].y*39);
        kotak.move(100,200);
        jendelaGame.draw(kotak);
    }
    // SCORE
    stringstream ss; // change int to string
    ss << score;
    string stringSkorGame = ss.str();
    // TEXT
    Font fontGame;
    fontGame.loadFromFile("asset//Montserrat-Bold.otf");
    Text textScoreGame;
    textScoreGame.setFont(fontGame);
    textScoreGame.setCharacterSize(50);
    textScoreGame.setString(stringSkorGame);
    textScoreGame.setFillColor(Color::White);
    textScoreGame.setPosition(1050,170);
	jendelaGame.draw(textScoreGame);
    // LAST DRAW
    jendelaGame.draw(frame);
    // PAUSE AND GAME OVER
    if (pause){
        jendelaGame.draw(notifPause);
    }
    else if (lose){
        jendelaGame.draw(notifLose);
    }
}

int main(){
	// MUSIC
    musik.openFromFile("asset//main-music.ogg");
    sfxPoint.openFromFile("asset//sfx-point.ogg");
    musik.setLoop(true);
    musik.play();

	// TEXTURE AND SPRITE
    Texture tUtama;
    tUtama.loadFromFile("asset//main-menu.png");
    Sprite bgUtama(tUtama);

    jendelaUtama.create(VideoMode(1280,720),"Jendela Utama");
    while(jendelaUtama.isOpen()){
        Vector2i lokasiUtama = Mouse::getPosition(jendelaUtama);
        Event eventUtama;
        while(jendelaUtama.pollEvent(eventUtama)){
            if (eventUtama.type==Event::Closed){
                jendelaUtama.close();
            }
            else{
            pos1 = false, pos2 = false, pos3 = false, pos4 = false;
                if (lokasiUtama.x > 530 && lokasiUtama.x < 700 && lokasiUtama.y > 490 &&  lokasiUtama.y < 520){
                pos1 = true;
                    if (Mouse::isButtonPressed(Mouse::Left)){
                        prepare();
                        jendelaGame.create(VideoMode(1280,720), "Game Tetris");
                        while(jendelaGame.isOpen()){
                            input();
                            if (!pause && !lose){
                                update();
                            }
                            draw();
                            // GAME OVER
                            if (gameOver()){
                                lose = true;
                            }
                            jendelaGame.display();
                        }
                    }
                }
                else if (lokasiUtama.x > 560 && lokasiUtama.x < 680 && lokasiUtama.y > 545 &&  lokasiUtama.y < 573){
                pos2 = true;
                    if (Mouse::isButtonPressed(Mouse::Left)){
                        jendelaSetting.create(VideoMode(1280,720),"Setting");

                        bool soundoff;

                        Texture tSetting, tmusicOn, tmusicOff, tmusic, tSFX, tSFXOn, tSFXOff;
                        tSetting.loadFromFile("asset//bg-setting.png");
                        tmusicOn.loadFromFile("asset//musik-on.png");
                        tmusicOff.loadFromFile("asset//musik-off.png");
                        tSFXOn.loadFromFile("asset//sfx-on.png");
                        tSFXOff.loadFromFile("asset//sfx-off.png");

                        Sprite bgSetting(tSetting), musicOn(tmusicOn), musicOff(tmusicOff), sfxOn(tSFXOn), sfxOff(tSFXOff);

                        while(jendelaSetting.isOpen()){
                            Vector2i lokasiSetting = Mouse::getPosition(jendelaSetting);
                            Event eventSetting;
                            while(jendelaSetting.pollEvent(eventSetting)){
                                if (eventSetting.type==Event::Closed){
                                    jendelaSetting.close();
                                }
                                else{
                                    if (lokasiSetting.x < 124 && lokasiSetting.y > 620){
                                            jendelaSetting.close();
                                    }
                                    else if(lokasiSetting.x < 860 && lokasiSetting.x > 750 && lokasiSetting.y > 270 && lokasiSetting.y < 360){
                                        if(Mouse::isButtonPressed(Mouse::Left)){
                                            soundoff = 1;
                                        }
                                    }
                                    else if(lokasiSetting.x < 749 && lokasiSetting.x > 670 && lokasiSetting.y > 270 && lokasiSetting.y < 360){
                                        if(Mouse::isButtonPressed(Mouse::Left)){
                                            soundoff = 0;
                                            musik.play();
                                            soundoff = 3;
                                        }
                                    }
                                    else if (lokasiSetting.x < 860 && lokasiSetting.x > 750 && lokasiSetting.y > 488 && lokasiSetting.y < 576){
                                        if (Mouse::isButtonPressed(Mouse::Left)){
                                            sfxoff = 1;
                                        }
                                    }
                                    else if (lokasiSetting.x < 749 && lokasiSetting.x > 670 && lokasiSetting.y > 488 && lokasiSetting.y < 576){
                                        if (Mouse::isButtonPressed(Mouse::Left)){
                                            sfxoff = 0;
                                        }
                                    }
                                }
                            }
                        jendelaSetting.draw(bgSetting);
                        musicOn.setPosition(672,276);
                        jendelaSetting.draw(musicOn);
                            if (soundoff == 1){
                                musik.pause();
                                musicOff.setPosition(672,276);
                                jendelaSetting.draw(musicOff);
                            }
                        sfxOn.setPosition(672,488);
                        jendelaSetting.draw(sfxOn);
	                        if (sfxoff == 1){
	                            sfxOff.setPosition(672,488);
	                            jendelaSetting.draw(sfxOff);
	                        }
                        jendelaSetting.display();
                        }
                    }
                }
                else if (lokasiUtama.x > 563 && lokasiUtama.x < 670 && lokasiUtama.y > 595 &&  lokasiUtama.y < 626){
                pos3 = true;
                    if (Mouse::isButtonPressed(Mouse::Left)){
                        Texture tAbout;
                        tAbout.loadFromFile("asset//bg-about.png");

                        Sprite bgAbout(tAbout);
                        jendelaAbout.create(VideoMode(1280,720), "About", Style::Default);

                        while(jendelaAbout.isOpen()){
                            Vector2i lokasiAbout = Mouse::getPosition(jendelaAbout);
                            Event eventAbout;
                            while(jendelaAbout.pollEvent(eventAbout)){
                                if (eventAbout.type==Event::Closed){
                                    jendelaAbout.close();
                                }
                                else{
                                    if (lokasiAbout.x < 124 && lokasiAbout.y > 620){
                                            jendelaAbout.close();
                                    }
                                }
                            }
                            jendelaAbout.draw(bgAbout);
                            jendelaAbout.display();
                        }
                    }
                }
                else if (lokasiUtama.x > 580 && lokasiUtama.x < 647 && lokasiUtama.y > 645 &&  lokasiUtama.y < 679){
                pos4 = true;
                    if (Mouse::isButtonPressed(Mouse::Left)){
                        jendelaUtama.close();
                    }
                }
            }
        }
        jendelaUtama.draw(bgUtama);

		// TEXT
        Font fontUtama;
        fontUtama.loadFromFile("asset//Montserrat-Bold.otf");
        Text textJudul, textGame, textSetting, textAbout, textExit;

        textGame.setFont(fontUtama);
        textGame.setCharacterSize(30);
        textGame.setString("Start Game");
        textGame.setFillColor(Color::White);
            if (pos1) textGame.setFillColor(Color::Red);
        textGame.setPosition(530,490);

        textSetting.setFont(fontUtama);
        textSetting.setCharacterSize(30);
        textSetting.setString("Setting");
        textSetting.setFillColor(Color::White);
            if (pos2) textSetting.setFillColor(Color::Red);
        textSetting.setPosition(560,540);

        textAbout.setFont(fontUtama);
        textAbout.setCharacterSize(30);
        textAbout.setString("About");
        textAbout.setFillColor(Color::White);
            if (pos3) textAbout.setFillColor(Color::Red);
        textAbout.setPosition(567,590);

        textExit.setFont(fontUtama);
        textExit.setCharacterSize(30);
        textExit.setString("Exit");
        textExit.setFillColor(Color::White);
            if (pos4) textExit.setFillColor(Color::Red);
        textExit.setPosition(584,640);
		// DRAW TEXT
        jendelaUtama.draw(textJudul);
        jendelaUtama.draw(textGame);
        jendelaUtama.draw(textSetting);
        jendelaUtama.draw(textAbout);
        jendelaUtama.draw(textExit);
        jendelaUtama.display();
    }
return 0;
}
