/*==============================================================
  character movement testing using Fly2

  - Load a scene
  - Generate a terrain object
  - Load a character
  - Control a character to move
  - Change poses

  (C)2012 Chuan-Chang Wang, All Rights Reserved
  Created : 0802, 2012

  Last Updated : 1010, 2014, Kevin C. Wang
 ===============================================================*/
#include "FlyWin32.h"
#include <math.h>


VIEWPORTid vID;                 // the major viewport
SCENEid sID;                    // the 3D scene
OBJECTid cID, tID, cpID;              // the main camera and the terrain for terrain following
CHARACTERid actorID, enemyID;            // the major character
ACTIONid idleID, runID, fightID, curPoseID, enemy_idleID, fightID2, fightID3; 
ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;

// some globals
int frame = 0;
int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;
int turnF = 0;
int count = 0;

int lrF = 0;
int upF = 0 ;
int upArrow=0;
int downArrow=0;
int leftArrow=0;
int rightArrow=0;
int arrowFlag=0;
float radius;
float height;
float constant=502.49f;
float zone;
float percent=0.25f;
int upingF=0;
int upingDir=0;
int zoneFlag = 0;
int zoneCounter = 0;
FnCharacter actor, enemy;
FnObject cp;
FnObject terrain;

// hotkey callbacks
void QuitGame(BYTE, BOOL4);
void Movement(BYTE, BOOL4);

// timer callbacks
void GameAI(int);
void RenderIt(int);

// mouse callbacks
void InitPivot(int, int);
void PivotCam(int, int);
void InitMove(int, int);
void MoveCam(int, int);
void InitZoom(int, int);
void ZoomCam(int, int);
float GetDistance(int, int);
bool peopleCollide(int, int);

/*------------------
 Get the distance between the 2 characters.
@param: int a1, actor1's ID
@param: int a2, actor2's ID
@return: float, the distance
-------------------*/
float GetDistance(int a1, int a2) {
	FnCharacter actor;
	actor.ID(a1);
	FnCharacter enemy;
	enemy.ID(a2);
	float actor_pos[3], enemy_pos[3], distance;

	actor.GetPosition(actor_pos);
	enemy.GetPosition(enemy_pos);

	//calculate the distance
	distance = sqrt(pow(actor_pos[0] - enemy_pos[0], 2) + pow(actor_pos[1] - enemy_pos[1], 2) +
		pow(actor_pos[2] - enemy_pos[2], 2));

	return distance;

}

/*------------------
Check if 2 people collide.
@param: int a1, actor1's ID
@param: int a2, actor2's ID
@return: bool, true for collide.
-------------------*/
bool peopleCollide(int a1, int a2) {
	float distance;
	distance = GetDistance(a1, a2);
	if (distance < 100.0f)
		return TRUE;
}


void direction()
{
	if((upArrow)&&(!leftArrow)&&(!rightArrow)&&(!downArrow))
	{
		if(turnF==0){
			 if(arrowFlag==1)
			{
				turnF=1;
				lrF=0;
				count=36;
			}else if(arrowFlag==2)
			 {
				turnF=1;
				lrF=0;
				count=18;
			}else if(arrowFlag==3)
			{
				turnF=1;
				lrF=1;
				count=18;
			}else if(arrowFlag==4)
			 {
				turnF=1;
				lrF=1;
				count=9;
			}else if(arrowFlag==5)
			 {
				turnF=1;
				lrF=0;
				count=9;
			}else if(arrowFlag==6)
			 {
				turnF=1;
				lrF=1;
				count=27;
			}else if(arrowFlag==7)
			 {
				turnF=1;
				lrF=0;
				count=27;
			}
			arrowFlag=0;
		}
	}else if((!upArrow)&&(!leftArrow)&&(!rightArrow)&&(downArrow))
	{
		if(turnF==0){
			 if(arrowFlag==0)
			{
				turnF=1;
				lrF=0;
				count=36;
			}else if(arrowFlag==2)
			 {
				turnF=1;
				lrF=1;
				count=18;
			}else if(arrowFlag==3)
			{
				turnF=1;
				lrF=0;
				count=18;
			}else if(arrowFlag==4)
			 {
				turnF=1;
				lrF=0;
				count=27;
			}else if(arrowFlag==5)
			 {
				turnF=1;
				lrF=1;
				count=27;
			}else if(arrowFlag==6)
			 {
				turnF=1;
				lrF=0;
				count=9;
			}else if(arrowFlag==7)
			 {
				turnF=1;
				lrF=1;
				count=9;
			}
			arrowFlag=1;
		}
	}else if((!upArrow)&&(leftArrow)&&(!rightArrow)&&(!downArrow))
	{
		
		if(turnF==0){
			 if(arrowFlag==0)
			{
				turnF=1;
				lrF=1;
				count=18;
			}else if(arrowFlag==1)
			{
				turnF=1;
				lrF=0;
				count=18;
			}else if(arrowFlag==3)
			 {
				turnF=1;
				lrF=1;
				count=36;
			}else if(arrowFlag==4)
			 {
				turnF=1;
				lrF=1;
				count=27;
			}else if(arrowFlag==5)
			 {
				turnF=1;
				lrF=1;
				count=9;
			}else if(arrowFlag==6)
			 {
				turnF=1;
				lrF=0;
				count=27;
			}else if(arrowFlag==7)
			 {
				turnF=1;
				lrF=0;
				count=9;
			}
			arrowFlag=2;
		}
	}else if((!upArrow)&&(!leftArrow)&&(rightArrow)&&(!downArrow))
	{
		if(turnF==0){
			 if(arrowFlag==0)
			{
				turnF=1;
				lrF=0;
				count=18;
			}else if(arrowFlag==1)
			{
				turnF=1;
				lrF=1;
				count=18;
			}else if(arrowFlag==2)
			 {
				turnF=1;
				lrF=0;
				count=36;
			}else if(arrowFlag==4)
			 {
				turnF=1;
				lrF=0;
				count=9;
			}else if(arrowFlag==5)
			 {
				turnF=1;
				lrF=0;
				count=27;
			}else if(arrowFlag==6)
			 {
				turnF=1;
				lrF=1;
				count=9;
			}else if(arrowFlag==7)
			 {
				turnF=1;
				lrF=1;
				count=27;
			}
			arrowFlag=3;
		}
	}else if((upArrow)&&(!leftArrow)&&(rightArrow)&&(!downArrow))
	{
		if(turnF==0){
			 if(arrowFlag==0)
			{
				turnF=1;
				lrF=0;
				count=9;
			}else if(arrowFlag==1)
			{
				turnF=1;
				lrF=1;
				count=27;
			}else if(arrowFlag==2)
			 {
				turnF=1;
				lrF=0;
				count=27;
			}else if(arrowFlag==3)
			 {
				turnF=1;
				lrF=1;
				count=9;
			}else if(arrowFlag==5)
			 {
				turnF=1;
				lrF=0;
				count=18;
			}else if(arrowFlag==6)
			 {
				turnF=1;
				lrF=1;
				count=18;
			}else if(arrowFlag==7)
			 {
				turnF=1;
				lrF=1;
				count=36;
			}
			arrowFlag=4;
		}
	}else if((upArrow)&&(leftArrow)&&(!rightArrow)&&(!downArrow))
	{
		if(turnF==0){
			 if(arrowFlag==0)
			{
				turnF=1;
				lrF=1;
				count=9;
			}else if(arrowFlag==1)
			{
				turnF=1;
				lrF=0;
				count=27;
			}else if(arrowFlag==2)
			 {
				turnF=1;
				lrF=0;
				count=9;
			}else if(arrowFlag==3)
			 {
				turnF=1;
				lrF=1;
				count=27;
			}else if(arrowFlag==4)
			 {
				turnF=1;
				lrF=1;
				count=18;
			}else if(arrowFlag==6)
			 {
				turnF=1;
				lrF=1;
				count=36;
			}else if(arrowFlag==7)
			 {
				turnF=1;
				lrF=0;
				count=18;
			}
			arrowFlag=5;
		}
	}else if((!upArrow)&&(!leftArrow)&&(rightArrow)&&(downArrow))
	{
		if(turnF==0){
			 if(arrowFlag==0)
			{
				turnF=1;
				lrF=0;
				count=27;
			}else if(arrowFlag==1)
			{
				turnF=1;
				lrF=1;
				count=9;
			}else if(arrowFlag==2)
			 {
				turnF=1;
				lrF=1;
				count=27;
			}else if(arrowFlag==3)
			 {
				turnF=1;
				lrF=0;
				count=9;
			}else if(arrowFlag==4)
			 {
				turnF=1;
				lrF=0;
				count=18;
			}else if(arrowFlag==5)
			 {
				turnF=1;
				lrF=0;
				count=36;
			}else if(arrowFlag==7)
			 {
				turnF=1;
				lrF=1;
				count=18;
			}
			arrowFlag=6;
		}
	}else if((!upArrow)&&(leftArrow)&&(!rightArrow)&&(downArrow))
	{
		if(turnF==0){
			 if(arrowFlag==0)
			{
				turnF=1;
				lrF=1;
				count=27;
			}else if(arrowFlag==1)
			{
				turnF=1;
				lrF=0;
				count=9;
			}else if(arrowFlag==2)
			 {
				turnF=1;
				lrF=1;
				count=9;
			}else if(arrowFlag==3)
			 {
				turnF=1;
				lrF=0;
				count=27;
			}else if(arrowFlag==4)
			 {
				turnF=1;
				lrF=1;
				count=36;
			}else if(arrowFlag==5)
			 {
				turnF=1;
				lrF=1;
				count=18;
			}else if(arrowFlag==6)
			 {
				turnF=1;
				lrF=0;
				count=18;
			}
			arrowFlag=7;
		}
	}
}

/*------------------
the main program
C.Wang 1010, 2014
-------------------*/

void pushCemara()
{
		  float atps[3], cps[3],ffdir[3],uudir[3];
	      actor.GetPosition(atps);
	      atps[2] += 100.0f;
	      actor.GetDirection(ffdir, uudir);
          cp.SetPosition(atps);
          cp.SetDirection(ffdir, uudir);
          cp.MoveForward(-radius);
          cp.MoveUp(height);
          cp.GetPosition(cps);
          for (int i = 0; i < 3; i++){
             ffdir[i] = atps[i] - cps[i];
          }
          cp.SetDirection(ffdir, NULL);
}

void pushCemaraLR()
{
		  float atps[3], cps[3],ffdir[3],uudir[3];
		  actor.MoveForward(-zoneCounter*10.0f);

	      actor.GetPosition(atps);
	      atps[2] += 100.0f;
	      actor.GetDirection(ffdir, uudir);
          cp.SetPosition(atps);
          cp.SetDirection(ffdir, uudir);
          cp.MoveForward(-radius);
          cp.MoveUp(height);
          cp.GetPosition(cps);
          for (int i = 0; i < 3; i++){
             ffdir[i] = atps[i] - cps[i];
          }
          cp.SetDirection(ffdir, NULL);

		  actor.MoveForward(zoneCounter*10.0f);
}

void pushCemaraOrg()
{
		  float atps[3], cps[3],ffdir[3],uudir[3];
	      actor.GetPosition(atps);
	      atps[2] += 100.0f;
	      actor.GetDirection(ffdir, uudir);
          cp.SetPosition(atps);
          cp.SetDirection(ffdir, uudir);
          cp.MoveForward(-500);
          cp.MoveUp(50);
          cp.GetPosition(cps);
          for (int i = 0; i < 3; i++){
             ffdir[i] = atps[i] - cps[i];
          }
          cp.SetDirection(ffdir, NULL);
}

void pushCemaraUp()
{
		  float atps[3], cps[3],ffdir[3],uudir[3];
		  
		  actor.MoveForward(-(zone-10.0f));
		  
	      actor.GetPosition(atps);
	      atps[2] += 100.0f;
	      actor.GetDirection(ffdir, uudir);
          cp.SetPosition(atps);
          cp.SetDirection(ffdir, uudir);
          cp.MoveForward(-radius);
          cp.MoveUp(height);
          cp.GetPosition(cps);
          for (int i = 0; i < 3; i++){
             ffdir[i] = atps[i] - cps[i];
          }
          cp.SetDirection(ffdir, NULL);

		  actor.MoveForward(zone);

}

void pushCemaraUp2()
{
		  float atps[3], cps[3],ffdir[3],uudir[3];
		  
		  actor.MoveForward(-(10.0f*(zoneCounter-1)));
		  
	      actor.GetPosition(atps);
	      atps[2] += 100.0f;
	      actor.GetDirection(ffdir, uudir);
          cp.SetPosition(atps);
          cp.SetDirection(ffdir, uudir);
          cp.MoveForward(-radius);
          cp.MoveUp(height);
          cp.GetPosition(cps);
          for (int i = 0; i < 3; i++){
             ffdir[i] = atps[i] - cps[i];
          }
          cp.SetDirection(ffdir, NULL);

		  actor.MoveForward(10.0f*(zoneCounter-1));

}

int testHit()
{
	 float dirt[3], origint[3];  
     dirt[0] = 0.0f;
     dirt[1] = 0.0f;
     dirt[2] = -1.0f;
	 cp.GetPosition(origint);

	 return(terrain.HitTest(origint, dirt));
}


void FyMain(int argc, char **argv)
{
   // create a new world
   BOOL4 beOK = FyStartFlyWin32("NTU@2014 Homework #01 - Use Fly2", 0, 0, 1024, 768, FALSE);

   // setup the data searching paths
   FySetShaderPath("Data\\NTU5\\Shaders");
   FySetModelPath("Data\\NTU5\\Scenes");
   FySetTexturePath("Data\\NTU5\\Scenes\\Textures");
   FySetScenePath("Data\\NTU5\\Scenes");

   // create a viewport
   vID = FyCreateViewport(0, 0, 1024, 768);
   FnViewport vp;
   vp.ID(vID);

   // create a 3D scene
   sID = FyCreateScene(10);
   FnScene scene;
   scene.ID(sID);

   // load the scene
   scene.Load("gameScene01");
   scene.SetAmbientLights(1.0f, 1.0f, 1.0f, 0.6f, 0.6f, 0.6f);

   // load the terrain
   tID = scene.CreateObject(OBJECT);
   
   terrain.ID(tID);
   BOOL beOK1 = terrain.Load("terrain");
   terrain.Show(FALSE);

   // set terrain environment
   terrainRoomID = scene.CreateRoom(SIMPLE_ROOM, 10);
   FnRoom room;
   room.ID(terrainRoomID);
   room.AddObject(tID);

   // load the character
   FySetModelPath("Data\\NTU5\\Characters");
   FySetTexturePath("Data\\NTU5\\Characters");
   FySetCharacterPath("Data\\NTU5\\Characters");
   actorID = scene.LoadCharacter("Lyubu2");
   enemyID = scene.LoadCharacter("Donzo2");

   // put the character on terrain
   float pos[3], fDir[3], uDir[3];
   
   actor.ID(actorID);
   pos[0] = 3569.0f; pos[1] = -3208.0f; pos[2] = 1000.0f;
   fDir[0] = 0.8f; fDir[1] = -0.5f; fDir[2] = 0.0f;
   uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;
   actor.SetDirection(fDir, uDir);

   actor.SetTerrainRoom(terrainRoomID, 10.0f);
   beOK = actor.PutOnTerrain(pos);

   // put the character on terrain
   float enemy_pos[3], enemy_fDir[3], enemy_uDir[3];

   enemy.ID(enemyID);
   enemy_pos[0] = 3400.0f; enemy_pos[1] = -3000.0f; enemy_pos[2] = 900.0f;
   enemy_fDir[0] = 1.0f; enemy_fDir[1] = 1.0f; enemy_fDir[2] = 0.0f;
   enemy_uDir[0] = 0.0f; enemy_uDir[1] = 0.0f; enemy_uDir[2] = 1.0f;
   enemy.SetDirection(enemy_fDir, enemy_uDir);

   enemy.SetTerrainRoom(terrainRoomID, 10.0f);
   beOK = enemy.PutOnTerrain(enemy_pos);

   // Get two character actions pre-defined at Lyubu2
   idleID = actor.GetBodyAction(NULL, "Idle");
   runID = actor.GetBodyAction(NULL, "Run");
   fightID = actor.GetBodyAction(NULL, "NormalAttack1");
   fightID2 = actor.GetBodyAction(NULL, "NormalAttack2");
   fightID3 = actor.GetBodyAction(NULL, "UltimateAttack");

   // Enemy
   enemy_idleID = enemy.GetBodyAction(NULL, "Idle");
   enemy.SetCurrentAction(NULL, 0, enemy_idleID);
   enemy.Play(START, 0.0f, FALSE, TRUE);


   // set the character to idle action
   curPoseID = idleID;
   actor.SetCurrentAction(NULL, 0, curPoseID);
   actor.Play(START, 0.0f, FALSE, TRUE);
   actor.TurnRight(90.0f);

   // translate the camera
   cID = scene.CreateObject(CAMERA);
   FnCamera camera;
   camera.ID(cID);
   camera.SetNearPlane(5.0f);
   camera.SetFarPlane(100000.0f);

   // camera parent for transmation cnotrol
  
   cpID = scene.CreateObject(OBJECT);
   cp.ID(cpID);
   camera.SetParent(cpID);

   // set camera initial position and orientation
   cp.GetDirection(fDir, uDir);
   camera.SetDirection(fDir, uDir);

   //
   // set cp
   float actpos[3], cppos[3];
   actor.GetPosition(actpos);
   actpos[2] += 100.0f;
   actor.GetDirection(fDir, uDir);
   cp.SetPosition(actpos);
   cp.SetDirection(fDir, uDir);
   radius=500.0f;
   height=50.0f;
   zone=radius*percent;
   cp.MoveForward(-radius);
   cp.MoveUp(height);
   cp.GetPosition(cppos);
   for (int i = 0; i < 3; i++){
      fDir[i] = actpos[i] - cppos[i];
   }
   cp.SetDirection(fDir, NULL);

   // setup a point light
   FnLight lgt;
   lgt.ID(scene.CreateObject(LIGHT));
   lgt.Translate(70.0f, -70.0f, 70.0f, REPLACE);
   lgt.SetColor(1.0f, 1.0f, 1.0f);
   lgt.SetIntensity(1.0f);

   // create a text object for displaying messages on screen
   textID = FyCreateText("Trebuchet MS", 18, FALSE, FALSE);

   // set Hotkeys
   FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);  // escape for quiting the game
   FyDefineHotKey(FY_UP, Movement, FALSE);      // Up for moving forward
   FyDefineHotKey(FY_RIGHT, Movement, FALSE);   // Right for turning right
   FyDefineHotKey(FY_LEFT, Movement, FALSE);    // Left for turning left
   FyDefineHotKey(FY_DOWN, Movement, FALSE);
   FyDefineHotKey(FY_Z, Movement, FALSE); //Attack
   FyDefineHotKey(FY_X, Movement, FALSE); //Attack
   FyDefineHotKey(FY_C, Movement, FALSE); //Attack

   // define some mouse functions
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

   // bind timers, frame rate = 30 fps
   FyBindTimer(0, 30.0f, GameAI, TRUE);
   FyBindTimer(1, 30.0f, RenderIt, TRUE);

   // invoke the system
   FyInvokeFly(TRUE);
}


/*-------------------------------------------------------------
  30fps timer callback in fixed frame rate for major game loop
  C.Wang 1103, 2007
 --------------------------------------------------------------*/
void GameAI(int skip)
{
   FnCharacter actor, enemy;
   FnObject cp;

   // play character pose
   actor.ID(actorID);
   actor.Play(LOOP, (float) skip, FALSE, TRUE);

   // play enemy pose
   enemy.ID(enemyID);
   enemy.Play(LOOP, (float)skip, FALSE, TRUE);


   cp.ID(cpID);
   float fDir[3], afDir[3], tempd[3];
   int walkFlag;

   
   direction();
   
   if((turnF==1)&&(count>0))
   {
	   if(zoneFlag==2)
		 {
			zoneFlag=0;
		 }
	   
	   if(lrF==0)
	   {
	     actor.TurnRight(5.0f);
		 count--;
		 if(count==0)
		 {
			turnF=0;

			if (!FyCheckHotKeyStatus(FY_UP) &&
          !FyCheckHotKeyStatus(FY_LEFT) &&
          !FyCheckHotKeyStatus(FY_RIGHT) &&
		  !FyCheckHotKeyStatus(FY_DOWN)){
			curPoseID = idleID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
			
			}

		 }
	   
	   }else
	   {
		 actor.TurnRight(-5.0f);
		 count--;
		 if(count==0)
		 {
			turnF=0;

			if (!FyCheckHotKeyStatus(FY_UP) &&
          !FyCheckHotKeyStatus(FY_LEFT) &&
          !FyCheckHotKeyStatus(FY_RIGHT) &&
		  !FyCheckHotKeyStatus(FY_DOWN)){
			curPoseID = idleID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
			
			}
		 }
	   }
   }

  

   
   if ((arrowFlag==0)&&((upArrow)&&(!leftArrow)&&(!rightArrow)&&(!downArrow))){
	 if((turnF==0)&&(upingF==0)&&(!peopleCollide(actorID, enemyID))){
      walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, TRUE);
	  if (peopleCollide(actorID, enemyID)) 
		  actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
      if (walkFlag == WALK){

		  if(zoneFlag==0){
			zoneCounter++;
			if(zoneCounter>=(int)(zone/10.0f))
			{
				zoneFlag=1;
			}
		  }

	  }else
	  {
		  //if(zoneCounter>=(int)(zone/10.0f))
			zoneFlag=2;
	  }


	  if(zoneFlag==1)
	  {
		if(upF==1){
				
					if(radius<=490.0f){
					radius+=10.0f;
						if(radius==500.0f)
							upF=0;
					}
					height=sqrt((constant)*(constant)-(radius)*(radius));

					pushCemaraUp();
		  }else{
					pushCemaraUp();
		  }
	  }else if (zoneFlag==2)
	  {
		if(zoneCounter>0)
		{
			if(upF==1){
				
					if(radius<=490.0f){
					radius+=10.0f;
						if(radius==500.0f)
							upF=0;
					}
					height=sqrt((constant)*(constant)-(radius)*(radius));

					pushCemaraUp2();
			}else{
					pushCemaraUp2();
			}
			zoneCounter--;
		}else{
			zoneFlag=0;
		}
	 }
		  
	}
   }

   if(upingF==1){
		if(radius>10.0f){
			 radius-=10.0f;
		}
		height=sqrt((constant)*(constant)-(radius)*(radius));
		if(upingDir==0)
		{
			actor.TurnRight(90.0f);
		}else if(upingDir==1)
		{
			actor.TurnRight(-90.0f);
		}else if(upingDir==2)
		{
			actor.TurnRight(-45.0f);
		}else if(upingDir==3)
		{
			actor.TurnRight(45.0f);
		}else if(upingDir==4)
		{
			actor.TurnRight(-135.0f);
		}else if(upingDir==5)
		{
			actor.TurnRight(135.0f);
		}

		pushCemaraLR();
		 
		if(upingDir==0)
		{
			actor.TurnRight(-90.0f);
		}else if(upingDir==1)
		{
			actor.TurnRight(90.0f);
		}else if(upingDir==2)
		{
			actor.TurnRight(45.0f);
		}else if(upingDir==3)
		{
			actor.TurnRight(-45.0f);
		}else if(upingDir==4)
		{
			actor.TurnRight(135.0f);
		}else if(upingDir==5)
		{
			actor.TurnRight(-135.0f);
		}

		if (testHit()>0) {
			upingF=0;
		}
		upF=1;
   }



   if ((arrowFlag==2)&&((!upArrow)&&(leftArrow)&&(!rightArrow)&&(!downArrow))){
     if((turnF==0)&&(upingF==0)){

		
		  if(zoneFlag==2){
			if(zoneCounter<(int)(zone/10.0f))
			 {
				zoneFlag=0;
			 }
		 }

			float angle, leng;
			if ((radius>30.0f) && (!peopleCollide(actorID, enemyID))){
				leng = radius * 2.0f * 3.1415926f;
				angle = 360.0f / (leng / 10.0f);
      
				actor.TurnRight(-angle);
				int flg = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, TRUE);
				if (peopleCollide(actorID, enemyID))
					actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
			}
			else {
				actor.TurnRight(-10.0f);
			}
		
		  actor.TurnRight(90.0f);
		  
		  pushCemaraLR();

		  actor.TurnRight(-90.0f);

		if (testHit()<=0) {
			upingF=1;
			upingDir=0;
		}
	   
     }
   }
  
   if ((arrowFlag==3)&&((!upArrow)&&(!leftArrow)&&(rightArrow)&&(!downArrow))){
     if((turnF==0)&&(upingF==0)){

		  if(zoneFlag==2){
			if(zoneCounter<(int)(zone/10.0f))
			 {
				zoneFlag=0;
			 }
		 }

			float angle, leng;
			if ((radius>30.0f) && (!peopleCollide(actorID, enemyID))){
				leng = radius * 2.0f * 3.1415926f;
				angle = 360.0f / (leng / 10.0f);
      
				actor.TurnRight(angle);
				int flg = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
				if (peopleCollide(actorID, enemyID))
					actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
			}else{
				actor.TurnRight(10.0f);
			}
		 
		
		  actor.TurnRight(-90.0f);
		  
		  pushCemaraLR();

		  actor.TurnRight(90.0f);

		if (testHit()<=0) {
			upingF=1;
			upingDir=1;
		}
	   
     }
   }

   if ((arrowFlag==1)&&((!upArrow)&&(!leftArrow)&&(!rightArrow)&&(downArrow))){
	   if ((turnF == 0) && (upingF == 0) && (!peopleCollide(actorID, enemyID))){


		    
		    walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
			if (peopleCollide(actorID, enemyID))
				actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
			if (walkFlag == WALK){
				if(upF==1){
				
					int localFlag;
					
					float tmpD[3],tmpP[3];
					cp.GetDirection(tmpD, NULL);
					cp.GetPosition(tmpP, NULL);
					
					actor.TurnRight(180.0f);
		  
					pushCemaraOrg();

					if (testHit() <= 0) {
						localFlag=0;
					}else
					{
						localFlag=1;
					}
					
					cp.SetDirection(tmpD, NULL);
					cp.SetPosition(tmpP, NULL);
					
					if(localFlag==0)
					{
						if(radius>10.0f){
							radius-=10.0f;
						}
						height=sqrt((constant)*(constant)-(radius)*(radius));
					}else
					{
						if(radius<=490.0f){
							radius+=10.0f;
							if(radius==500.0f)
								upF=0;
						}
						height=sqrt((constant)*(constant)-(radius)*(radius));
					}
					
					if(zoneCounter>0){
						pushCemaraUp2();
						zoneCounter--;
					}else
					{
						pushCemara();
					}

					actor.TurnRight(-180.0f);

				}else{

					float tmpD[3],tmpP[3];
					int localF;
					cp.GetDirection(tmpD, NULL);
					cp.GetPosition(tmpP, NULL);

					actor.TurnRight(180.0f);
		  
					if(zoneCounter>0){
						localF=1;
						pushCemaraUp2();
						zoneCounter--;
					}else
					{
						localF=0;
						pushCemara();
					}

					if ((testHit() <= 0) && (!peopleCollide(actorID, enemyID))){

						cp.SetDirection(tmpD, NULL);
						cp.SetPosition(tmpP, NULL);
						actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
						if (peopleCollide(actorID, enemyID))
							actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
						upF=1;
						if(localF==1){
							zoneCounter++;
						}
					}

					actor.TurnRight(-180.0f);
				}
			}
	   }
   }

   if ((arrowFlag==4)&&((upArrow)&&(!leftArrow)&&(rightArrow)&&(!downArrow))){
	   if ((turnF == 0) && (upingF == 0) && (!peopleCollide(actorID, enemyID))){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, TRUE);
			if (peopleCollide(actorID, enemyID))
				actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
			if (walkFlag == WALK){
			
				actor.TurnRight(-45.0f);
				pushCemaraLR();
				actor.TurnRight(45.0f);
				if (testHit()<=0) {
					upingF=1;
					upingDir=2;
				}
			}
	   }
   }

    //左+上
    if ((arrowFlag==5)&&((upArrow)&&(leftArrow)&&(!rightArrow)&&(!downArrow))){
		if ((turnF == 0) && (upingF == 0) && (!peopleCollide(actorID, enemyID))){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, TRUE);
			if (peopleCollide(actorID, enemyID))
					actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
			if (walkFlag == WALK){
			
				actor.TurnRight(45.0f);
				pushCemaraLR();
				actor.TurnRight(-45.0f);

				if (testHit()<=0) {
					upingF=1;
					upingDir=3;
				}
			}
	   }
   }

	 if ((arrowFlag==6)&&((!upArrow)&&(!leftArrow)&&(rightArrow)&&(downArrow))){
		 if ((turnF == 0) && (upingF == 0) && (!peopleCollide(actorID, enemyID))){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, TRUE);
			if (peopleCollide(actorID, enemyID))
				actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
			if (walkFlag == WALK){
			
				actor.TurnRight(-135.0f);
				pushCemaraLR();
				actor.TurnRight(135.0f);
				if (testHit()<=0) {
					upingF=1;
					upingDir=4;
				}
			}
	   }
   }

	  if ((arrowFlag==7)&&((!upArrow)&&(leftArrow)&&(!rightArrow)&&(downArrow))){
		  if ((turnF == 0) && (upingF == 0) && (!peopleCollide(actorID, enemyID))){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, TRUE);
			if (peopleCollide(actorID, enemyID))
				actor.MoveForward(-10.0f, TRUE, FALSE, FALSE, TRUE);
			if (walkFlag == WALK){
			
				actor.TurnRight(135.0f);
				pushCemaraLR();
				actor.TurnRight(-135.0f);
				if (testHit()<=0) {
					upingF=1;
					upingDir=5;
				}
			}
	   }
   }



}


/*----------------------
  perform the rendering
  C.Wang 0720, 2006
 -----------------------*/
void RenderIt(int skip)
{
   FnViewport vp;

   // render the whole scene
   vp.ID(vID);
   vp.Render3D(cID, TRUE, TRUE);

   // get camera's data
   FnCamera camera;
   camera.ID(cID);

   float pos[3], fDir[3], uDir[3];
   camera.GetPosition(pos);
   camera.GetDirection(fDir, uDir);

   // show frame rate
   static char string[128];
   if (frame == 0) {
      FyTimerReset(0);
   }

   if (frame/10*10 == frame) {
      float curTime;

      curTime = FyTimerCheckTime(0);
      sprintf(string, "Fps: %6.2f", frame/curTime);
   }

   frame += skip;
   if (frame >= 1000) {
      frame = 0;
   }

   FnText text;
   text.ID(textID);

   text.Begin(vID);
   text.Write(string, 20, 20, 255, 0, 0);

   char posS[256], fDirS[256], uDirS[256],rtest[256];
   sprintf(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
   sprintf(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
   sprintf(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);
   sprintf(rtest, "zoneFlag %d zoneCounter %d" ,zoneFlag,zoneCounter);

   text.Write(posS, 20, 35, 255, 255, 0);
   text.Write(fDirS, 20, 50, 255, 255, 0);
   text.Write(uDirS, 20, 65, 255, 255, 0);
   text.Write(rtest, 20, 85, 255, 255, 0);

   text.End();

   // swap buffer
   FySwapBuffers();
}


/*------------------
  movement control
  C.Wang 1103, 2006
 -------------------*/
void Movement(BYTE code, BOOL4 value)
{
   FnCharacter actor;
   actor.ID(actorID);
  

   if (value) {
	   if (code == FY_Z) {
		   curPoseID = fightID;
		   actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
	   }
	   else if (code == FY_X) {
		   curPoseID = fightID2;
		   actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
	   }
	   else if (code == FY_C) {
		   curPoseID = fightID3;
		   actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
	   }

	   else if (curPoseID != runID){
         curPoseID = runID;
         actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
      }
   }
   else if (!FyCheckHotKeyStatus(FY_UP) &&
          !FyCheckHotKeyStatus(FY_LEFT) &&
          !FyCheckHotKeyStatus(FY_RIGHT) &&
          !FyCheckHotKeyStatus(FY_DOWN)&&
		  !FyCheckHotKeyStatus(FY_Z)&&
		  !FyCheckHotKeyStatus(FY_X)&&
		  !FyCheckHotKeyStatus(FY_C)&&!turnF) {
      curPoseID = idleID;
      actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
   }

   
   if (code == FY_UP){
      if (value) {

		  upArrow=1;
		  zone=radius*percent;
		  if(zoneCounter>=(int)(zone/10.0f))
			{
				zoneFlag=1;
		  }else
		  {
			if(zoneFlag!=2){	
			  zoneFlag=0;
			}
		  }
      }
   }

   if (code == FY_UP){
      if (!value) {
		  upArrow=0;
		 
      }
   }

   if (code == FY_DOWN){
      if (value) {
		downArrow=1;
      }
   }

   if (code == FY_DOWN){
      if (!value) {
		downArrow=0;
      }
   }

   if (code == FY_RIGHT){
      if (value) {
		 rightArrow=1;
      }
   }

    if (code == FY_RIGHT){
      if (!value) {
		 rightArrow=0;
      }
   }

   if (code == FY_LEFT){
      if (value) {
		 leftArrow=1;
      }
   }

   if (code == FY_LEFT){
      if (!value) {
		 leftArrow=0;
      }
   }
  
}


/*------------------
  quit the demo
  C.Wang 0327, 2005
 -------------------*/
void QuitGame(BYTE code, BOOL4 value)
{
   if (code == FY_ESCAPE) {
      if (value) {
         FyQuitFlyWin32();
      }
   }
}



/*-----------------------------------
  initialize the pivot of the camera
  C.Wang 0329, 2005
 ------------------------------------*/
void InitPivot(int x, int y)
{
   oldX = x;
   oldY = y;
   frame = 0;
}


/*------------------
  pivot the camera
  C.Wang 0329, 2005
 -------------------*/
void PivotCam(int x, int y)
{
   FnObject model;

   if (x != oldX) {
      model.ID(cID);
      model.Rotate(Z_AXIS, (float) (x - oldX), GLOBAL);
      oldX = x;
   }

   if (y != oldY) {
      model.ID(cID);
      model.Rotate(X_AXIS, (float) (y - oldY), GLOBAL);
      oldY = y;
   }
}


/*----------------------------------
  initialize the move of the camera
  C.Wang 0329, 2005
 -----------------------------------*/
void InitMove(int x, int y)
{
   oldXM = x;
   oldYM = y;
   frame = 0;
}


/*------------------
  move the camera
  C.Wang 0329, 2005
 -------------------*/
void MoveCam(int x, int y)
{
   if (x != oldXM) {
      FnObject model;

      model.ID(cID);
      model.Translate((float)(x - oldXM)*2.0f, 0.0f, 0.0f, LOCAL);
      oldXM = x;
   }
   if (y != oldYM) {
      FnObject model;

      model.ID(cID);
      model.Translate(0.0f, (float)(oldYM - y)*2.0f, 0.0f, LOCAL);
      oldYM = y;
   }
}


/*----------------------------------
  initialize the zoom of the camera
  C.Wang 0329, 2005
 -----------------------------------*/
void InitZoom(int x, int y)
{
   oldXMM = x;
   oldYMM = y;
   frame = 0;
}


/*------------------
  zoom the camera
  C.Wang 0329, 2005
 -------------------*/
void ZoomCam(int x, int y)
{
   if (x != oldXMM || y != oldYMM) {
      FnObject model;

      model.ID(cID);
      model.Translate(0.0f, 0.0f, (float)(x - oldXMM)*10.0f, LOCAL);
      oldXMM = x;
      oldYMM = y;
   }
}

