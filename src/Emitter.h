#pragma once
//----------------------------------------------------------------------------------
//
// Basic Sprite Emitter Class
//
//
//  Kevin M. Smith - CS 134 SJSU

#include "Sprite.h"


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public BaseObject {
public:
	Emitter(SpriteSystem *);
	void draw();
	void start();
	void stop();
	void setLifespan(float);    // in milliseconds
	void setVelocity(ofVec3f);  // pixel/sec
	void setChildImage(ofImage);
	void setChildSize(float w, float h) { childWidth = w; childHeight = h; }
	void setImage(ofImage);
	void setRate(float);
    void setParticleRadius(const float r) { particleRadius = r; }
    void setGroupSize(int s) { groupSize = s; }
    void setOneShot(bool s) { oneShot = s; }
    void spawn(float time);

	float maxDistPerFrame();
	void update();
	SpriteSystem *sys;
	float rate;
    bool oneShot;
    bool fired;
    float particleRadius;
    float radius;
    bool visible;
    int groupSize;      // number of particles to spawn in a group
    bool createdSys;
    bool havesound;
    ofVec3f velocity;
	float lifespan;
	bool started;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
    ofSoundPlayer sound;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
	float childWidth, childHeight;
    int speed;
};
