#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.h"
#include "Particle.h"
#include "ParticleEmitter.h"


typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;



class Emitter;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void checkCollisions();
        void mouseDragged(int x, int y, int button);
        void keyPressed(int key);
        void moveSprite(MoveDir dir) ;
        void keyReleased(int key);
        void mousePressed(int x, int y, int button);
        void gunUpdateBykeys();

/*
        void mouseMoved(int x, int y );
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    */
        vector<Emitter *> invaders;
        Emitter *invader, *gun,*boss;

        ofVec3f mouse_last;
        int playerHP;
        int bossHP;
        ParticleEmitter emitter,emitter2;// boom effect and skill
        ImpulseRadialForce *radialForce;
        ofVec3f position;
        ofxFloatSlider rate;
        ofxFloatSlider life;
        ofxVec3Slider velocity;
        ofxLabel screenSize;
        ofxPanel gui;
        int hit;
		float gameStartTime;
		bool gameOver = false;
    bool gamestart = false;
        ofVec3f start_point;
        ofVec3f finish_point;
        float start_time;
        float finish_time;
        ofVec3f mouse_last2;
        MoveDir moveDir;
        ofImage start;
        ofImage gameover;
        ofImage gamefinish;
        ofVec3f startAccelPoint;
        ofSoundPlayer burstSound;
        ofSoundPlayer bomb;
        int HP;
        int numEmitters;

};
