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
#include <string.h>
#define PI 3.14159265





VIEWPORTid vID;                 // the major viewport
SCENEid sID;                    // the 3D scene
OBJECTid cID, tID, cpID;              // the main camera and the terrain for terrain following
CHARACTERid actorID;            // the major character
ACTIONid idleID, runID,attackID1,attackID2,attackID3,curPoseID; // two actions
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
int AttackCounter=0;
int Attack1Flag=0;
int Attack2Flag=0;
int Attack3Flag=0;

float angleAB=0.0f;

float radius;
float height;
float constant=502.49f;
float zone;
float percent=0.25f;
int upingF=0;
int upingDir=0;
int zoneFlag = 0;
int zoneCounter = 0;
FnCharacter actor;
FnObject cp;
FnObject terrain;
FnScene scene;
BOOL4 beOK;

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

/*------------------
  the main program
  C.Wang 1010, 2014
 -------------------*/

float GetDistance(int a1, int a2) {
	FnCharacter actorLocal;
	actorLocal.ID(a1);
	FnCharacter enemy;
	enemy.ID(a2);
	float actor_pos[3], enemy_pos[3], distance;

	actorLocal.GetPosition(actor_pos);
	enemy.GetPosition(enemy_pos);

	//calculate the distance
	distance = sqrt(pow(actor_pos[0] - enemy_pos[0], 2) + pow(actor_pos[1] - enemy_pos[1], 2) +
		pow(actor_pos[2] - enemy_pos[2], 2));

	return distance;

}

void getResultFdir(int a1, int a2, float *ary) {
	FnCharacter actorLocal;
	actorLocal.ID(a1);
	FnCharacter enemy;
	enemy.ID(a2);
	float resultFdir[3],actor2Pos[3],actor1Pos[3];
	actorLocal.GetPosition(actor1Pos);
	enemy.GetPosition(actor2Pos);
	resultFdir[0]=actor2Pos[0]-actor1Pos[0];
	resultFdir[1]=actor2Pos[1]-actor1Pos[1];
	resultFdir[2]=actor2Pos[2]-actor1Pos[2];
	
	ary[0]=-(resultFdir[0]);
	ary[1]=-(resultFdir[1]);
	ary[2]=-(resultFdir[2]);
}


float GetAngle(int a1, int a2) {
	FnCharacter actorLocal;
	actorLocal.ID(a1);
	FnCharacter enemy;
	enemy.ID(a2);
	float angle;
	float actor1Fdir[3],actor1Pos[3];
	float resultFdir[3],actor2Pos[3];
	float cross,lengthA,lengthB;

	actorLocal.GetPosition(actor1Pos);
	enemy.GetPosition(actor2Pos);
	resultFdir[0]=actor2Pos[0]-actor1Pos[0];
	resultFdir[1]=actor2Pos[1]-actor1Pos[1];
	resultFdir[2]=actor2Pos[2]-actor1Pos[2];

	actorLocal.GetDirection(actor1Fdir,NULL);

	cross=resultFdir[0]*actor1Fdir[0]+resultFdir[1]*actor1Fdir[1]+resultFdir[2]*actor1Fdir[2];
	lengthA=sqrt(pow(resultFdir[0],2)+pow(resultFdir[1],2)+pow(resultFdir[2],2));
	lengthB=sqrt(pow(actor1Fdir[0],2)+pow(actor1Fdir[1],2)+pow(actor1Fdir[2],2));
		
	
	angle=(float)acos(cross/lengthA/lengthB)*180.0/PI;

	return angle;

}

bool peopleCollide(int a1, int a2) {
   float distance;
   float angle;
	distance = GetDistance(a1, a2);
	angle=GetAngle(a1, a2);
	if ((distance < 50.0f)&&(angle<45.0f))
		return true;
	else
		return false;
}

bool testIFforward(int a1, int a2){
	FnCharacter actorLocal;
	bool FlagLocal;
	int walkFlag;

	actorLocal.ID(a1);
	walkFlag=actorLocal.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);

	if(walkFlag==WALK){
		FlagLocal=peopleCollide(a1,a2);
		actorLocal.MoveForward(-10.0f, TRUE, FALSE, FALSE, FALSE);
		return !FlagLocal;
	}
	else{
		return true;
	}
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

class enemy { 
public: 
	enemy(char* name,float*pos_c,float*fDir_c,float*uDir_c){

		AttackHitF = 0;
        DeadF = 0;
        HitCounter=0;
		
		actorID_c = scene.LoadCharacter(name);
   
        actor_c.ID(actorID_c);
		actor_c.SetDirection(fDir_c, uDir_c);
		actor_c.SetTerrainRoom(terrainRoomID, 10.0f);
        beOK = actor_c.PutOnTerrain(pos_c);

		if(strcmp(name,"Donzo2")==0){
			enemy_category=0;
			Life=10;
			idleID_c = actor_c.GetBodyAction(NULL, "Idle");
			hurtID_c = actor_c.GetBodyAction(NULL, "DamageH");
			dieID_c = actor_c.GetBodyAction(NULL, "Die");
		}else if(strcmp(name,"Robber02")==0){
		    enemy_category=1;
			Life=5;
			idleID_c = actor_c.GetBodyAction(NULL, "CombatIdle");
			hurtID_c = actor_c.GetBodyAction(NULL, "Damage2");
			dieID_c = actor_c.GetBodyAction(NULL, "Die");
		}

		curPoseID_c = idleID_c;
		actor_c.SetCurrentAction(NULL, 0, curPoseID_c);
		actor_c.Play(START, 0.0f, FALSE, TRUE);
		actor_c.TurnRight(90.0f);

	} 

	void playAction(int skip){
	
		if((AttackHitF!=1)&&(DeadF!=1)){
			actor_c.Play(LOOP, (float) skip, FALSE, TRUE);
		}else if(AttackHitF==1){
			actor_c.Play(ONCE, (float) skip, FALSE, TRUE);
			HitCounter--;
			if(HitCounter==0){
				curPoseID_c = idleID_c;
				actor_c.SetCurrentAction(NULL, 0, curPoseID_c, 0.0f);
				AttackHitF=0;
			}
		}else if(DeadF==1){
			actor_c.Play(ONCE, (float) skip, FALSE, TRUE);
		}
	}

	void beHit(float rangeLength,float rangeAngle,int damage){
		float lengthLocal,angleLocal,resultFdir[3];
	    angleLocal=GetAngle(actorID,actorID_c);
	    lengthLocal=GetDistance(actorID,actorID_c);
		getResultFdir(actorID,actorID_c,resultFdir);

		if((angleLocal<rangeAngle)&&(lengthLocal<rangeLength)){

			if(Life>0){
				Life-=damage;
				actor_c.SetDirection(resultFdir,NULL);
			}
		
			if(Life>0){
				AttackHitF=1;
				curPoseID_c = hurtID_c;
				actor_c.SetCurrentAction(NULL, 0, curPoseID_c, 5.0f);
				if(enemy_category==0){
					HitCounter=35;
				}else if(enemy_category==1){
					HitCounter=25;
				}
			}else if((Life<=0)&&(DeadF!=1)){
				AttackHitF=0;
				DeadF=1;
				curPoseID_c = dieID_c;
				actor_c.SetCurrentAction(NULL, 0, curPoseID_c, 5.0f);
			}
		}
	}

	CHARACTERid getCid(){
		return actorID_c;
	}
    

private:
    int enemy_category;
	FnCharacter actor_c;
	ACTIONid idleID_c,curPoseID_c,dieID_c,hurtID_c;
	CHARACTERid actorID_c;
	int AttackHitF;
    int DeadF;
    int HitCounter;
    int Life;
};

enemy * enemyArray[2];

void AttackHit(int attack)
{
	float rangeLength,rangeAngle;
	int damage;
	int y;

	if(attack==1){
		rangeLength=135.0f;
		rangeAngle=30.0f;
		damage=1;
	}else if(attack==2){
		rangeLength=135.0f;
		rangeAngle=60.0f;
		damage=2;
	}else if(attack==3){
		rangeLength=135.0f;
		rangeAngle=180.0f;
		damage=5;
	}

	for(y=0;y<2;y++){
		enemyArray[y]->beHit(rangeLength,rangeAngle,damage);
	}
}


void FyMain(int argc, char **argv)
{
   int y;
	
	// create a new world
   beOK = FyStartFlyWin32("NTU@2014 Homework #01 - Use Fly2", 0, 0, 1024, 768, FALSE);

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

   // put the character on terrain
   float pos[3], fDir[3], uDir[3];
   
   actor.ID(actorID);
   pos[0] = 3569.0f; pos[1] = -3208.0f; pos[2] = 1000.0f;
   fDir[0] = 1.0f; fDir[1] = 1.0f; fDir[2] = 0.0f;
   uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;
   actor.SetDirection(fDir, uDir);

   actor.SetTerrainRoom(terrainRoomID, 10.0f);
   beOK = actor.PutOnTerrain(pos);

   // Get two character actions pre-defined at Lyubu2
   idleID = actor.GetBodyAction(NULL, "Idle");
   runID = actor.GetBodyAction(NULL, "Run");
   attackID1 = actor.GetBodyAction(NULL, "NormalAttack1");
   attackID2 = actor.GetBodyAction(NULL, "NormalAttack3");
   attackID3 = actor.GetBodyAction(NULL, "NormalAttack2");

   // set the character to idle action
   curPoseID = idleID;
   actor.SetCurrentAction(NULL, 0, curPoseID);
   actor.Play(START, 0.0f, FALSE, TRUE);
   actor.TurnRight(90.0f);

   
   fDir[0] = -1.0f; fDir[1] = -1.0f; fDir[2] = -0.0f;
   for(y=0;y<2;y++){
	   pos[0]+=150.0f;
	   if(y==0){
	     enemyArray[y]=new enemy("Donzo2",pos,fDir,uDir);
	   }else{
		 enemyArray[y]=new enemy("Robber02",pos,fDir,uDir);
	   }
   }



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
   FyDefineHotKey(FY_Z, Movement, FALSE);
   FyDefineHotKey(FY_X, Movement, FALSE);
   FyDefineHotKey(FY_C, Movement, FALSE);

   // define some mouse functions
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

   // bind timers, frame rate = 30 fps
   FyBindTimer(0, 30.0f, GameAI, TRUE);
   FyBindTimer(1, 30.0f, RenderIt, TRUE);

   // invoke the system
   FyInvokeFly(TRUE);

   for(y=0;y<2;y++){
    delete enemyArray[y];
   }
}


/*-------------------------------------------------------------
  30fps timer callback in fixed frame rate for major game loop
  C.Wang 1103, 2007
 --------------------------------------------------------------*/
void GameAI(int skip)
{
   FnCharacter actor,actor2;
   FnObject cp;
   int y;

   // play character pose
   actor.ID(actorID);
   if((Attack1Flag!=1)&&(Attack2Flag!=1)&&(Attack3Flag!=1)){
		actor.Play(LOOP, (float) skip, FALSE, TRUE);
   }else if(Attack1Flag==1){
		actor.Play(ONCE, (float) skip, FALSE, TRUE);
		AttackCounter--;
		if(AttackCounter==0){
			curPoseID = idleID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
			Attack1Flag=0;
		}
   }else if(Attack2Flag==1){
		actor.Play(ONCE, (float) skip, FALSE, TRUE);
		AttackCounter--;

		if(AttackCounter==20){
			AttackHit(2);
		}

		if(AttackCounter==0){
			curPoseID = idleID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
			Attack2Flag=0;
		}
   }else if(Attack3Flag==1){
		actor.Play(ONCE, (float) skip, FALSE, TRUE);
		AttackCounter--;

		if(AttackCounter==40){
			AttackHit(3);
		}

		if(AttackCounter==0){
			curPoseID = idleID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
			Attack3Flag=0;
		}
   }

   for(y=0;y<2;y++){
	enemyArray[y]->playAction(skip);
   }
	

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
	 if((turnF==0)&&(upingF==0)){
		 bool continueFlag;
		 for(y=0;y<2;y++){
			continueFlag=testIFforward(actorID,enemyArray[y]->getCid());
			if(!continueFlag){
				break;
			}
		 }

	 if(continueFlag){
      walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
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
		 bool continueFlag;
		 for(y=0;y<2;y++){
			continueFlag=testIFforward(actorID,enemyArray[y]->getCid());
			if(!continueFlag){
				break;
			}
		 }
		
		 if(continueFlag){
		  if(zoneFlag==2){
			if(zoneCounter<(int)(zone/10.0f))
			 {
				zoneFlag=0;
			 }
		 }

			float angle, leng;
			if(radius>30.0f){
			leng = radius * 2.0f * 3.1415926f;
			angle = 360.0f / (leng / 10.0f);
      
			actor.TurnRight(-angle);
			int flg = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
			}else{
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
   }
  
   if ((arrowFlag==3)&&((!upArrow)&&(!leftArrow)&&(rightArrow)&&(!downArrow))){
     if((turnF==0)&&(upingF==0)){
		 bool continueFlag;
		 for(y=0;y<2;y++){
			continueFlag=testIFforward(actorID,enemyArray[y]->getCid());
			if(!continueFlag){
				break;
			}
		 }

		 if(continueFlag){
		  if(zoneFlag==2){
			if(zoneCounter<(int)(zone/10.0f))
			 {
				zoneFlag=0;
			 }
		 }

			float angle, leng;
			if(radius>30.0f){
			leng = radius * 2.0f * 3.1415926f;
			angle = 360.0f / (leng / 10.0f);
      
			actor.TurnRight(angle);
			int flg = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
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
   }

   if ((arrowFlag==1)&&((!upArrow)&&(!leftArrow)&&(!rightArrow)&&(downArrow))){
	   if((turnF==0)&&(upingF==0)){
		   bool continueFlag;
		   for(y=0;y<2;y++){
			 continueFlag=testIFforward(actorID,enemyArray[y]->getCid());
			 if(!continueFlag){
				break;
			}
		   }
		  
		   if(continueFlag){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
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
						//pushCemaraUp2();
						zoneCounter--;
					}else
					{
						localF=0;
						pushCemara();
					}

					if (testHit() <= 0) {

						cp.SetDirection(tmpD, NULL);
						cp.SetPosition(tmpP, NULL);
						actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
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
   }

   if ((arrowFlag==4)&&((upArrow)&&(!leftArrow)&&(rightArrow)&&(!downArrow))){
	   if((turnF==0)&&(upingF==0)){
		  bool continueFlag;
		  for(y=0;y<2;y++){
			continueFlag=testIFforward(actorID,enemyArray[y]->getCid());
			if(!continueFlag){
				break;
			}
		  }
		  if(continueFlag){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
			if (walkFlag == WALK){
			
				actor.TurnRight(-45.0f);
				pushCemaraLR();
				actor.TurnRight(45.0f);
				/*if (testHit()<=0) {
					upingF=1;
					upingDir=2;
				}*/
			}
	      }
	   }
   }

    if ((arrowFlag==5)&&((upArrow)&&(leftArrow)&&(!rightArrow)&&(!downArrow))){
	   if((turnF==0)&&(upingF==0)){
		  bool continueFlag;
		  for(y=0;y<2;y++){
			continueFlag=testIFforward(actorID,enemyArray[y]->getCid());
			if(!continueFlag){
				break;
			}
		  } 
		  if(continueFlag){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
			if (walkFlag == WALK){
			
				actor.TurnRight(45.0f);
				pushCemaraLR();
				actor.TurnRight(-45.0f);

				/*if (testHit()<=0) {
					upingF=1;
					upingDir=3;
				}*/
			}
	      }
	   }
   }

	 if ((arrowFlag==6)&&((!upArrow)&&(!leftArrow)&&(rightArrow)&&(downArrow))){
	   if((turnF==0)&&(upingF==0)){
		 bool continueFlag;
		 for(y=0;y<2;y++){
			continueFlag=testIFforward(actorID,enemyArray[y]->getCid());
			if(!continueFlag){
				break;
			}
		 }
		 if(continueFlag){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
			if (walkFlag == WALK){
			
				actor.TurnRight(-135.0f);
				pushCemaraLR();
				actor.TurnRight(135.0f);
				/*if (testHit()<=0) {
					upingF=1;
					upingDir=4;
				}*/
			}
	      }
	   }
   }

	  if ((arrowFlag==7)&&((!upArrow)&&(leftArrow)&&(!rightArrow)&&(downArrow))){
	   if((turnF==0)&&(upingF==0)){
		  bool continueFlag;
		  for(y=0;y<2;y++){
			continueFlag=testIFforward(actorID,enemyArray[y]->getCid());
			if(!continueFlag){
				break;
			}
		  }  
		  if(continueFlag){
			walkFlag = actor.MoveForward(10.0f, TRUE, FALSE, FALSE, FALSE);
			if (walkFlag == WALK){
			
				actor.TurnRight(135.0f);
				pushCemaraLR();
				actor.TurnRight(-135.0f);
				/*if (testHit()<=0) {
					upingF=1;
					upingDir=5;
				}*/
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

   char posS[256], fDirS[256], uDirS[256],rtest[256],angletest[256];
   sprintf(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
   sprintf(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
   sprintf(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);
   sprintf(rtest, "zoneFlag %d zoneCounter %d" ,zoneFlag,zoneCounter);
   sprintf(angletest, "angleAB %8.3f" ,angleAB);

   text.Write(posS, 20, 35, 255, 255, 0);
   text.Write(fDirS, 20, 50, 255, 255, 0);
   text.Write(uDirS, 20, 65, 255, 255, 0);
   text.Write(rtest, 20, 80, 255, 255, 0);
   text.Write(angletest, 20, 95, 255, 255, 0);

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
  
 /* if (code == FY_UP){
	   if (!value) 
	   {
			if(zoneCounter>=(int)(zone/10.0f))	
				zoneFlag=2;
	   }
   }*/
   if (code == FY_Z){
	   if (value) {
		   if((Attack1Flag!=1)&&(Attack2Flag!=1)&&(Attack3Flag!=1)&&(upArrow!=1)&&(downArrow!=1)&&(leftArrow!=1)&&(rightArrow!=1)&&(turnF!=1)){
				Attack1Flag=1;
				AttackHit(1);
				curPoseID = attackID1;
				actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
				AttackCounter=25;
		   }
	   }
   }

   if (code == FY_X){
	   if (value) {
		   if((Attack1Flag!=1)&&(Attack2Flag!=1)&&(Attack3Flag!=1)&&(upArrow!=1)&&(downArrow!=1)&&(leftArrow!=1)&&(rightArrow!=1)&&(turnF!=1)){
				Attack2Flag=1;
				curPoseID = attackID2;
				actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
				AttackCounter=45;
		   }
	   }
   }

   if (code == FY_C){
	   if (value) {
		   if((Attack1Flag!=1)&&(Attack2Flag!=1)&&(Attack3Flag!=1)&&(upArrow!=1)&&(downArrow!=1)&&(leftArrow!=1)&&(rightArrow!=1)&&(turnF!=1)){
				Attack3Flag=1;
				curPoseID = attackID3;
				actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
				AttackCounter=45;
		   }
	   }
   }
   
   
   if (code == FY_UP){
	  if((Attack1Flag!=1)&&(Attack2Flag!=1)&&(Attack3Flag!=1)){
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

		  if (curPoseID != runID){
			curPoseID = runID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
		  }
	  }else{
		  upArrow=0;
		  if ((upArrow!=1)&&(downArrow!=1)&&(leftArrow!=1)&&(rightArrow!=1)&&(turnF!=1)) {
				curPoseID = idleID;
				actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
		  }
	  }
	  }
   }

   if (code == FY_DOWN){
	  if((Attack1Flag!=1)&&(Attack2Flag!=1)&&(Attack3Flag!=1)){
      if (value) {
		downArrow=1;
		 if (curPoseID != runID){
			curPoseID = runID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
		  }
	  }else{
		  downArrow=0;
		  if ((upArrow!=1)&&(downArrow!=1)&&(leftArrow!=1)&&(rightArrow!=1)&&(turnF!=1)) {
				curPoseID = idleID;
				actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
		  }
	  }
	  }
   }

   if (code == FY_RIGHT){
	  if((Attack1Flag!=1)&&(Attack2Flag!=1)&&(Attack3Flag!=1)){
      if (value) {
		 rightArrow=1;
		 if (curPoseID != runID){
			curPoseID = runID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
		  }
	  }else{
		  rightArrow=0;
		  if ((upArrow!=1)&&(downArrow!=1)&&(leftArrow!=1)&&(rightArrow!=1)&&(turnF!=1)) {
				curPoseID = idleID;
				actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
		  }

	  }
	  }
   }

   if (code == FY_LEFT){
	  if((Attack1Flag!=1)&&(Attack2Flag!=1)&&(Attack3Flag!=1)){
      if (value) {
		 leftArrow=1;
		 if (curPoseID != runID){
			curPoseID = runID;
			actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
		  }
	  }else{
		 leftArrow=0;
		 if ((upArrow!=1)&&(downArrow!=1)&&(leftArrow!=1)&&(rightArrow!=1)&&(turnF!=1)) {
				curPoseID = idleID;
				actor.SetCurrentAction(NULL, 0, curPoseID, 5.0f);
		  }

	  }
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


