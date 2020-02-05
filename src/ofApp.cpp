#include "ofApp.h"
#include "Emitter.h"
//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" to create a very
// minimal shooter video arcade game.  The game creates two(2) Emitters,
// one for the gun (missiles launcher) and another one for emitting the
// invaders.  
//
// Game will run for 20 seconds.  To start a new game, press mouse button
//
//  Kevin M. Smith - CS 134 SJSU
//
//--------------------------------------------------------------
void ofApp::setup()
{
    start.load("start.png");
    gameover.load("gameover.png");
    gamefinish.load("gamefinish.png");

	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);
	hit = 0;
    burstSound.load("burstsound.mp3");
    bomb.load("bomb.mp3");
    bomb.setMultiPlay(true);

	// Record when the game starts - game will run for 10 sec
	//
	gameStartTime = ofGetElapsedTimeMillis();
    bossHP = 100;
    HP = 100;
	// Create and setup emitters  (you can play with parameters
	// I have another example program that creates a GUI for these
	// so with that you can get an idea of velocity/rate effects.
	//
	gun = new Emitter( new SpriteSystem() );
    boss = new Emitter( new SpriteSystem() );

    numEmitters = 2;
    for (int i = 0;  i < numEmitters; i++)
    {
        Emitter *emit = new Emitter(new SpriteSystem());
        
        emit->drawable = false;                // make emitter itself invisible

        invaders.push_back(emit);
    }
    radialForce = new ImpulseRadialForce(2000.0);
    emitter.sys->addForce(radialForce);
    emitter.setVelocity(ofVec3f(0, 0, 0));
    emitter.setOneShot(true);
    emitter.setEmitterType(RadialEmitter);
    emitter.setGroupSize(300);
    emitter.setRate(1);
    emitter.setLifespan(3);
    emitter.setParticleRadius(0.5);
    
    ////boss skill///

    radialForce = new ImpulseRadialForce(70000.0);

    emitter2.sys->addForce(radialForce);
    emitter2.setVelocity(ofVec3f(0, 0, 0));
    emitter2.setOneShot(true);
    emitter2.setEmitterType(RadialEmitter);
    emitter2.setGroupSize(10);
    emitter2.setRate(1);
    emitter2.setLifespan(5);
    emitter2.setParticleRadius(0.5);

    
	// Set up some reasonable parameters for the gun/missile launcher
	// Remember that "Y" decreases as we move toward teh top of the 
	// window, so we set a negative velocity of -1000;
	// We also don't want the missiles to live forever, so we set
	// a lifespan of 500 ms.
	//
	gun->setPosition(ofVec3f(ofGetWindowWidth()/2, ofGetWindowHeight()-10, 0));
	gun->setVelocity(ofVec3f(0, -1000, 0));
	gun->setRate(2.2);
	gun->setLifespan(1000);
    
    boss->setPosition(ofVec3f(ofGetWindowWidth()/2-150, 200, 0));
    boss->setVelocity(ofVec3f(0, 0, 0));
    boss->setRate(3);
    boss->setLifespan(500);
    
    ofImage img;
    img.load("turret.png");
    gun->setImage(img);
    img.load("missile.png");
    gun->setChildImage(img);
    
    img.load("boss.png");
    boss->setImage(img);
    img.load("bullet.png");
    boss->setChildImage(img);
    
    gun->sound = burstSound;
    gun->sound.setMultiPlay(true);
    gun->havesound = true;
	// Set up some reasonable parameters for the invader spirtes
	// Remember that "Y" decreases as we move toward the bottom of
	// the window, so we set a negative velocity of 400.
	// We also don't want the invaders to live forever, so we set
	// a lifespan of 5000 ms.  they are traveling much slower
    // than the missiles, so they need to live longer
	//
    invaders[0]->setPosition(ofVec3f(ofGetWindowWidth() / 2, 10, 0));
	invaders[0]->velocity.set(0, 400, 0);
	invaders[0]->setLifespan(5000);
	invaders[0]->setRate(2);
    img.load("enermy.png");
    invaders[0]->setChildImage(img);
    
    invaders[1]->setPosition(ofVec3f(ofGetWindowWidth() / 3, 10, 0));
    invaders[1]->velocity.set(0, 300, 0);
    invaders[1]->setLifespan(5000);
    invaders[1]->setRate(1);
    img.load("enermy2.png");
    invaders[1]->setChildImage(img);

	//
	//
    for (int i=0; i<invaders.size(); i++) {
        invaders[i]->stop();
    }
    boss->speed =220;
    boss->rate = 1;
    boss->lifespan = 5000;
    boss->velocity = ofVec3f(0,200,0);
	gun->stop();
    boss->stop();

    ////gui
    

}

//--------------------------------------------------------------
//  ofApp::update() is called every frame by the OF mainloop. 
//  If the frame rate is 60 fps (frames per second) it gets called 
//  every 1/60 sec
//
void ofApp::update() {
    ofSeedRandom();

	//  call the emitter's update function to create new sprites
	//  as needed
	//
   
    gunUpdateBykeys();
    gun->speed = 120;
	gun->update();
    boss->update();
    emitter.update(position,"");
    emitter2.update(ofVec3f(ofGetWindowWidth()/2, ofGetWindowHeight()/2,0),"skill");
    
    //increase level of difficulty
    if (bossHP<=50)
    {
        boss->rate = 3;
        emitter2.setGroupSize(30);
    }
    if (hit>10)
    {
        invaders[0]->velocity.set(0, 600, 0);
        invaders[1]->velocity.set(0, 800, 0);
    }
        
        
    if (ofGetElapsedTimef()-gameStartTime>5)
    {
        gameStartTime = ofGetElapsedTimef();
        emitter2.sys->reset();
        emitter2.start();
    }
    
    for (int i=0; i<invaders.size(); i++) {
        invaders[i]->update();
    }
    

	// check for collisions between missles and invaders
	//  
	checkCollisions();
    
	// we will randomize initial velocity so that not the invaders
	// are going at the same speed just to make it a little harder/interesting
	// to play
	//
    // invaders path
    for (int i=0; i<invaders.size(); i++) {
        ofVec3f v = invaders[i]->velocity;
        invaders[i]->update();
        invaders[i]->setVelocity(ofVec3f(ofRandom(-v.y /2, v.y /2), v.y, v.z));
        //invaders[i]->setVelocity(ofVec3f(0, 100,0));
        for (int j=0; j<invaders[i]->sys->sprites.size(); j++)
        {
        float age = invaders[0]->sys->sprites[j].age();
            invaders[0]->sys->sprites[j].trans.x += sin(ofGetElapsedTimef()*2)*20 ;
        }
    }
    // boss path
    boss->setVelocity(ofVec3f(ofRandom(-100, 100), 200, 0));
    boss->trans.x +=  3 * sin(ofGetElapsedTimef());
    boss->trans.y +=  1 * cos(ofGetElapsedTimef());

	// game runs for 20 seconds; check to see if over and stop
	// emitters.  We are computing in milleseconds so we need
	// to multiply seconds by 1000 to convert to ms.
	//
    // check HP
    if (bossHP <= 0)
    {
        if (gameOver == false)
        {
            hit += 100;

        }
        gameOver = true;
        
    }
    if (HP <= 0)
    {
        boss->stop();
        gun->stop();
        for (int i=0; i<invaders.size(); i++)
        {
            invaders[i]->stop();
        }
        gameOver = true;
    }
    
    
	float t = ofGetElapsedTimeMillis();
	/*if ((t - gameStartTime > (20 * 1000))) {
		gameOver = true;
		gun->stop();
		invaders->stop();
	}
     */
}


//--------------------------------------------------------------
//  ofApp::draw() this is where everything gets drawn
//  you should only put draw code in this method. 
//
//
void ofApp::draw(){

	// if game is over, just draw a label in middle of screen
	//
   
   

    
    if (!gamestart)
    {
        start.draw(0, 0 );
    }
	else if (gameOver)
    {
        
        if (bossHP <= 0)
            gamefinish.draw(0,0);
        else
            gameover.draw(0,0);

        string hitText;
        hitText += "score: " + std::to_string(hit);
        ofDrawBitmapString(hitText, ofPoint(ofGetWindowWidth() / 2-20, ofGetWindowHeight() / 2 + 50));
	}
	else
    {
        emitter.draw();
        emitter2.draw();
        ofDrawRectangle(ofGetWindowWidth()/2, 0,bossHP,20);
        ofDrawRectangle(ofGetWindowWidth()-20, ofGetWindowHeight()-100,20,HP);
        for (int i=0; i<invaders.size(); i++) {
            invaders[i]->draw();
        }
		gun->draw();
        boss->draw();
        string hitText;
        hitText += "score: " + std::to_string(hit);
        ofDrawBitmapString(hitText, ofPoint(10, 20));
	}

	
	
    
}

//  This is a simple O(M x N) collision check
//  For each missle check to see which invaders you hit and remove them
//
void ofApp::checkCollisions() {

	// find the distance at which the two sprites (missles and invaders) will collide
	// detect a collision when we are within that distance.
	//
    float collisionDist ;
    int hitAdd;
    // check for missles and monster
    for (int i=0; i<invaders.size(); i++)
    {
        collisionDist = gun->childHeight / 2 + invaders[i]->childHeight / 2-10;
        for (int j = 0; j < gun->sys->sprites.size(); j++)
        {
            hitAdd = invaders[i]->sys->removeNear(gun->sys->sprites[j].trans, collisionDist);
            hit += hitAdd;
            if (hitAdd != 0)
            {
                emitter.sys->reset();
                emitter.start();
                position = ofVec3f(gun->sys->sprites[j].trans.x,gun->sys->sprites[j].trans.y,0);
                bomb.play();
            }
            hitAdd =0;
        }
    }
    // check for missles and boss
    collisionDist = boss->height / 2 + gun->childHeight / 2;
    for (int j = 0; j < gun->sys->sprites.size(); j++)
    {
        int a = gun->sys->removeNear(boss->trans, collisionDist);
        if (a!= NULL)//hit
        {
            bossHP -= 5;
        }
    }
    // check for bullets and gun
    collisionDist = boss->childHeight/2 + gun->height / 2;
    for (int j = 0; j < boss->sys->sprites.size(); j++)
    {
        int a = boss->sys->removeNear(gun->trans, collisionDist);
        if (a!= NULL)//hit
        {
            HP -= 5*a;

        }
    }
    // check for invader and gun
    collisionDist = invaders[0]->childHeight/ 2 + gun->height / 2;
    for (int i=0; i<invaders.size(); i++)
    {
        int a = invaders[i]->sys->removeNear(gun->trans, collisionDist);
        if (a!= NULL)//hit
        {
            HP -= 10*a;
        }
    }
    // check for skill and gun
    collisionDist = boss->childHeight/2 + gun->height / 2;
    for (int i = 0; i < emitter2.sys->particles.size(); i++)
    {
        int a = emitter2.sys->removeNear(gun->trans, collisionDist);
        
        if (a!= NULL)//hit
        {
            HP -= 5 * a;
        }
    }
	// Loop through all the missiles, then remove any invaders that are within
	// "collisionDist" of the missiles.  the removeNear() function returns the
	// number of missiles removed.
	//
	
}



//  support dragging of gun with mouse
//
void ofApp::mousePressed(int x, int y, int button){
    mouse_last = ofPoint(x, y);
}

void ofApp::mouseDragged(int x, int y, int button){
    ofPoint mouse_cur = ofPoint(x, y);
    ofVec3f delta = mouse_cur - mouse_last;
    gun->trans += delta;
    mouse_last = mouse_cur;
    
    
    
}


//--------------------------------------------------------------
// Start game if not running; otherwise track mouse last coord
// 
void ofApp::keyPressed(int key) {
    switch (key)
    {
        case ' ':
        {
            if (gun->started == false)
            {
                gameStartTime = ofGetElapsedTimef();    
                boss->start();
                gun->start();
                for (int i=0; i<invaders.size(); i++)
                {
                    invaders[i]->start();
                }
                gamestart =true;
                
            
            }
            else{
                gun->rate = 2.2;
                gun->draw();
                
                
            }
           
      
        }
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;

        case OF_KEY_RIGHT:
            moveSprite(MoveRight);
        
            break;
        case OF_KEY_LEFT:
            moveSprite(MoveLeft);
            break;
        case OF_KEY_UP:
            moveSprite(MoveUp);
            break;
        case OF_KEY_DOWN:
            moveSprite(MoveDown);
            break;
            
    }
}
void ofApp::moveSprite(MoveDir dir)
{
    moveDir = dir;
}
void ofApp::keyReleased(int key) {
    switch (key) {
        case ' ':
            gun->rate = 0;
            break;
        case OF_KEY_RIGHT:
            moveSprite(MoveStop);
            break;
        case OF_KEY_LEFT:
            moveSprite(MoveStop);
            break;
        case OF_KEY_UP:
            moveSprite(MoveStop);
            break;
        case OF_KEY_DOWN:
            moveSprite(MoveStop);
            break;
    }
}
void ofApp::gunUpdateBykeys()
{
    float dist = gun->speed * 2 / ofGetFrameRate();
    static ofVec3f dir;
    ofRectangle r = ofGetWindowRect();
    switch (moveDir)
    {
        case MoveUp:
            dir = ofVec3f(0, -dist, 0);
            break;
        case MoveDown:
            dir = ofVec3f(0, dist, 0);
            break;
        case MoveLeft:
            dir = ofVec3f(-dist, 0, 0);
            break;
        case MoveRight:
            dir = ofVec3f(dist, 0, 0);
            break;
        case MoveStop:
            dir = ofVec3f(0,0,0);
    }
    gun->trans += dir;
    //set bound
    ofPoint winsize = ofGetWindowSize();
    if (gun->trans.x >winsize.x-70)
        gun->trans.x = winsize.x-70;
    if (gun->trans.y >winsize.y-50)
        gun->trans.y = winsize.y-50;
    if (gun->trans.x <70)
        gun->trans.x =70;
    if (gun->trans.y <50)
        gun->trans.y =50;
    
}


