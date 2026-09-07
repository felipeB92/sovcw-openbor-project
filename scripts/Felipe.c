#include "data/scripts/player.c"

/* 
 * Jump to a branch if combined score goes above/bellow determined value
 * You can put that on a character death animation to i.e. go for secret stage
 * created by Don Drago
 */
void changeBranchByTotalScore(int score, int extraScorePerPlayer, int goNow, char branchHigh, char branchLow) {
	int players=openborvariant("count_players");
	int totalscore = 0;
	int scoreMultiplier = players-1;
	int x;
	
	for(x=0;x<players;x++) {
		totalscore+=getplayerproperty(x, "score",0);
	}
	scoreMultiplier*=extraScorePerPlayer;
	score+=scoreMultiplier;
	
	if(totalscore < score) {
		if(branchLow!= NULL()) { jumptobranch(branchLow, goNow); }
	}
	else {
		if(branchHigh!= NULL()) { jumptobranch(branchHigh, goNow); }		
	}
}

/* 
 * Jump to a branch if a player's score goes above determined value.
 * If no player fulfils the score requirement, you may use it to send the
 * players to ANOTHER branch
 * You can put that on a character death animation to i.e. go for secret stage
 * created by Don Drago
 */
void changeBranchByHighestScore(int score, int goNow, char branchHigh, char branchLow) {
	int players=openborvariant("count_players");
	int playerScore = 0;
	int maxScore = 0;
	int x;
	
	for(x=0;x<players;x++) {
		playerScore = getplayerproperty(x, "score",0);
		if(maxScore < playerScore) { maxScore =  playerScore; }
	}
	if(maxScore < score) {
		if(branchLow!= NULL()) { jumptobranch(branchLow, goNow);}
	}
	else {
		if(branchHigh!= NULL()) { jumptobranch(branchHigh, goNow);}		
	}
}

/* 
 * Add hits to your hit counter without really "hit" the opponent
 * Useful in mult hit throws
 * created by Don Drago
 */
void addhits(int hits) {
	void self = getlocalvar("self");
	int previous = getentityproperty(self, "rush_count");
	changeentityproperty(self, "rush_count", previous+hits);
}


/* 
 * sets MP
 * created by Don Drago
 */
void setMp(int mp) {
	void self = getlocalvar("self");
	changeentityproperty(self, "mp", mp);
}

/* 
 * guided jump to the nearest opponent, with desired time (reachAt) to reach it
 * and position adjusts(X, Y and Z). If there's no opponent, uses whiff* values
 * to jump. Use flipMe = 1 to face the target, if desired you may use this
 * with ajustY=NULL() and whiffY=NULL() while jumping, to make a "chasing jump"
 * (look at Super Final Fight Gold PLUS: Won Won has a chasing jump attack)
 * created by Don Drago
 */
void guidedJump(int reachAt, int adjustX, int adjustY, int adjustZ, float whiffX, float whiffY, float whiffZ, int flipMe) {
	void self = getlocalvar("self");
	void opponent = findtarget(self);
	int facing = (getentityproperty(self,"direction")==0) ? (-1) : (1);

	float gravity = -(getlevelproperty("gravity")*(1.0-getentityproperty(self,"antigravity")));
	float pX = getentityproperty(self, "x");
	float pY = getentityproperty(self, "y");
	float pZ = getentityproperty(self, "z");
	float x, y, z;
	
	reachAt--;

	if(opponent!= NULL()) {
		float eX = getentityproperty(opponent, "x");
		float eY = getentityproperty(opponent, "y");
		float eZ = getentityproperty(opponent, "z");
		
		adjustX *= (eX < pX) ?(-1) : (1);
		
		if(adjustX !=NULL()) {
			x = (eX-pX+adjustX)*facing;
			x = (x/reachAt)*facing;			
		}
		else { x = NULL(); }
		
		if(adjustY !=NULL()) {
			y = eY-pY+adjustY;
			y = ((reachAt*gravity/2)+(y/reachAt));
		}
		else { y = NULL(); }
		
		if(adjustZ !=NULL()) {
			z = eZ-pZ+adjustZ;
			z = z/reachAt;
		}
		else { z = NULL(); }
		
		if(eX < pX) {
			if(flipMe != 0){ changeentityproperty(self, "direction", 0); }
		}
		else {
			if(flipMe != 0){ changeentityproperty(self, "direction", 1); }
		}			
	}
	else {
		x = whiffX*facing;
		y = whiffY;
		z = whiffZ;
	}
	
	 
    tossentity(self, y, x, z);
}

/* 
 * turns to the opposite side
 * created by Don Drago
 */
void turn() {
	void self = getlocalvar("self");
	void facing = getentityproperty(self, "direction");
		
	if(facing) { changeentityproperty(self, "direction", 0); }
	else { changeentityproperty(self, "direction", 1); }
}

/* 
 * auto turn to the nearest target
 * created by Don Drago
 */
void autoTurn() {
	void self = getlocalvar("self");
	void opponent = findtarget(self);

	float pX = getentityproperty(self, "x");

	if(opponent!= NULL()) {
		float eX = getentityproperty(opponent, "x");
		
		if(eX < pX) { changeentityproperty(self, "direction", 0); }
		else { changeentityproperty(self, "direction", 1); }
	}
}


/* 
 * stop in the air.
 * created by Don Drago
 */
void airstop() {
    void self = getlocalvar("self");
    changeentityproperty(self, "velocity", 0, 0, 0); //Stop moving!
	changeentityproperty(self,"antigravity",1);	
}


/* 
 * recover gravity
 * created by Don Drago
 */
void airfollow() {
    void self = getlocalvar("self");
	changeentityproperty(self,"antigravity",0.1);	
}



/* 
 * MUGEN-styled velAdd
 * dirFree: if defined, applies left/right movement
 * instead back/front
 * created by Don Drago
 */
void velAdd (float x, float y, float z, void dirFree) {
    void self = getlocalvar("self");
	int side = 1;
	int facing = getentityproperty(self, "direction");
	if(dirFree == NULL()) {
		side = (facing == 0) ? (-1) : (1);		
	}
	
	x = getentityproperty(self,"xdir")+(x*side);
	y = getentityproperty(self,"tossv")+y;
	z = getentityproperty(self,"zdir")+z;
	
	changeentityproperty(self, "velocity", x, z, y);
}

/* 
 * MUGEN-styled velMul
 * created by Don Drago
 */
void velMul (float x, float y, float z) {
    void self = getlocalvar("self");
	
	if(x != NULL()) { x *= getentityproperty(self,"xdir"); }
	if(y != NULL()) { y *= getentityproperty(self,"tossv"); }
	if(z != NULL()) { z *= getentityproperty(self,"zdir"); }
	
	changeentityproperty(self, "velocity", x, z, y);
}

/* 
 * MUGEN-styled velSet
 * dirFree: if defined, applies left/right movement
 * instead back/front
 * created by Don Drago
 */
void velSet (float x, float y, float z, void dirFree) {
    void self = getlocalvar("self");	
	int side = 1;
	
	if(dirFree == NULL()) {
		side = (getentityproperty(self, "direction")== 0) ? (-1) : (1);		
	}
	
	x = x*side;
	
	changeentityproperty(self, "velocity", x, z, y);
}


/* 
 * reads key press
 * returns 1 if selected key was pressed, 0 otherwise
 * instead back/front
 * created by Don Drago
 */
void readKey(void key) {
	void self = getlocalvar("self");
	int pindex = getentityproperty(self,"playerindex");
	int facing = getentityproperty(self, "direction");
	
	if(key=="left" || key=="l") { return playerkeys(pindex, 0, "moveleft"); }
	else if(key=="right" || key=="r") { return playerkeys(pindex, 0, "moveright"); }
	else if(key=="up" || key=="u") { return playerkeys(pindex, 0, "moveup"); }
	else if(key=="down" || key=="d") { return playerkeys(pindex, 0, "movedown"); }
	else if(key=="jump" || key=="j") { return playerkeys(pindex, 0, "jump"); }
	else if(key=="attack" || key=="atk" || key=="a") { return playerkeys(pindex, 0, "attack"); }
	else if(key=="attack2" || key=="atk2" || key=="a2") { return playerkeys(pindex, 0, "attack2"); }
	else if(key=="attack3" || key=="atk3" || key=="a3") { return playerkeys(pindex, 0, "attack3"); }
	else if(key=="attack4" || key=="atk4" || key=="a4") { return playerkeys(pindex, 0, "attack4"); }
	else if(key=="special" || key=="sp" || key=="s") { return playerkeys(pindex, 0, "special"); }
	else if(key=="backward" || key=="back" || key=="b") {
		if(facing) { return playerkeys(pindex, 0, "moveleft"); }
		else { return playerkeys(pindex, 0, "moveright"); }
	}
	else if(key=="forward" || key=="fwd" || key=="f") {
		if(facing) { return playerkeys(pindex, 0, "moveright"); }
		else { return playerkeys(pindex, 0, "moveleft"); }
	}
	else { return 0; }
}

/* 
 * gets X velocity
 * created by Don Drago
 */
float getXvel() {
	void self = getlocalvar("self");
	int side = (getentityproperty(self, "direction")== 0) ? (-1) : (1);

	return getentityproperty(self,"xdir")*side;
}

/* 
 * limits X velocity between min and max
 * created by Don Drago
 */
void limitXvel(float min, float max) {
	void self = getlocalvar("self");
	float x = getentityproperty(self,"xdir");
	
	if(x < min) { x = min; }
	else if(x > max) { x = max; }

	changeentityproperty(self, "velocity", x, NULL(), NULL());
}




/* 
 * stores X velocity in a local var,
 * to be retrieved by retrieveXvel()
 * created by Don Drago
 */
void storeXvel() {
	void self = getlocalvar("self");
	int side = (getentityproperty(self, "direction")== 0) ? (-1) : (1);

	setlocalvar("xVel" + self, getentityproperty(self,"xdir")*side);
}

/* 
 * retrieves X velocity in a local var,
 * stored by storeXvel()
 * created by Don Drago
 */
float retrieveXvel() {
	void self = getlocalvar("self");
	return getlocalvar("xVel" + self);
}

/* 
 * MUGEN styled changeState
 * you must use openBOR's anim names, minus ANI_
 * as argument, i.e. "FREESPECIAL2"
 * created by Don Drago
 */
void changeState(void anim) {
	void self = getlocalvar("self");
	changeentityproperty(self, "animation", openborconstant("ANI_"+anim));
}

/* 
 * Calculate damage based on opponent's max life
 * useful for life % based attacks
 * created by Don Drago
 */
int calcRelativeDamage(float percent) { 
	void self = getlocalvar("self");
	void target = target = getentityproperty(self, "opponent");
	int damage = percent;
	float percentRecalc = percent*0.01;

	if(target!=NULL()) {
		percentRecalc *= getentityproperty(target,"maxhealth");
		damage = percentRecalc;
	}
	
	return damage;
}

void adjustGrabDistance(float dx)
{ //adjust normal grab
   void self = getlocalvar("self");
   void target = getentityproperty(self, "grabbing");
   
   if(target!=NULL())
   {
     bindentity(target, self, dx, 0, 0, 0, 0);
     bindentity(target, NULL());
   }
}

void adjustGrabAnim()
{ //adjust normal grab
   void self = getlocalvar("self");
   void target = getentityproperty(self, "grabbing");
   
   if(target!=NULL())
   {
       damageentity(target, self, 0, 0, openborconstant("ATK_NORMAL2")); // Damage target with desired damage
       updateframe(target, 0);
	}
}

int isPlatform(int x, int z, int a) {
	void getPlatform = checkplatformbelow(x,z,a);
	if(getPlatform) {
		if(getentityproperty(getPlatform,"type") == openborconstant("TYPE_NONE") ) { return 1; }
		else { return 0; }
	}
	else { return 0; }
}

int isBlockedFront(int edge) {
	
	void self = getlocalvar("self");
	int facing = getentityproperty(self, "direction");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hf = 0;
	int minX = openborvariant("xpos");
	int maxX = openborvariant("hResolution")+minX;


	// check X walls
	Hf = (facing==1) ? (checkwall(x+edge,z)) : (checkwall(x-edge,z));
	
	// check X platforms
	if(Hf <= 0) {Hf = (facing==1) ? (isPlatform(x+edge,z,1001)) : (isPlatform(x-edge,z,1001));}
	
	// check X corners
	if(Hf <= 0) { Hf = (facing==1) ? (maxX-edge < x) :  (minX+edge > x); }
	  
	return (Hf > 0);
	
}

int isBlockedBack(int edge) {
	
	void self = getlocalvar("self");
	int facing = getentityproperty(self, "direction");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hb = 0;
	int minX = openborvariant("xpos");
	int maxX = openborvariant("hResolution")+minX;

	// check X walls
	Hb = (facing==1) ? (checkwall(x-edge,z)) : (checkwall(x+edge,z));
	
	// check X platforms
	if(Hb <= 0) {Hb = (facing==1) ? (isPlatform(x-edge,z,1001)) : (isPlatform(x+edge,z,1001));}
	
	// check X corners
	if(Hb <= 0) { Hb = (facing==1) ? (minX+edge > x) :  (maxX-edge < x); }
	
	return (Hb > 0);
}

int isBlockedUp(int edge) {
	
	void self = getlocalvar("self");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hu = 0;
	int minZ = openborvariant("PLAYER_MIN_Z");


	// check Z walls
	Hu = checkwall(x,z+edge);
	
	// check Z platforms
	if(Hu <= 0) { Hu = isPlatform(x,z+edge,1001); }

	// check Z corners
	if(Hu <= 0) { Hu = (minZ+edge > z); }

	return (Hu > 0);
}

int isBlockedDown(int edge) {
	
	void self = getlocalvar("self");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hd = 0;
	int maxZ = openborvariant("PLAYER_MAX_Z");


	// check Z walls
	Hd = checkwall(x,z-edge);
	
	// check Z platforms
	if(Hd <= 0) { Hd = isPlatform(x,z-edge,1001); }

	// check Z corners
	if(Hd <= 0) { Hd = (maxZ-edge < z); }

	return (Hd > 0);
}


void screenBound(int frontEdge, int backEdge, int upEdge, int downEdge, int divisor) {
	
	void self = getlocalvar("self");
	int facing = getentityproperty(self, "direction");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hf = 0;
	float Hb = 0;
	float Hu = 0;
	float Hd = 0;
	float newX = 0;

	if(divisor == NULL()|| divisor == 0) {divisor = 1;}

	// check upper and lower obstacles
	Hu = isBlockedUp(upEdge);
	Hd = isBlockedDown(downEdge);

	// apply Z correction
	if(Hu > 0) { changeentityproperty(self, "position", x, z+(upEdge/divisor)); }
	if(Hd > 0) { changeentityproperty(self, "position", x, z-(downEdge/divisor)); }

	// check fwd and back obstacles
	Hf = isBlockedFront(frontEdge);
	Hb = isBlockedBack(backEdge);
	  
	// apply X correction (front)
	if(Hf > 0) {
		newX = (facing==1) ? (x-(frontEdge/divisor)) :  (x+(frontEdge/divisor)); 
		changeentityproperty(self, "position", newX); 
	}
	
	// apply X correction (back)
	if(Hb > 0) {
		newX = (facing==1) ? (x+(backEdge/divisor)) :  (x-(backEdge/divisor)); 
		changeentityproperty(self, "position", newX);
	}
}

void bindSpawn(void vName, float fX, float fY, float fZ)
{
    void self = getlocalvar("self");
    void vSpawn;
    int dir = getentityproperty(self, "direction");

    // 1. Limpa o cache de spawn da engine para evitar conflitos com outros personagens
    clearspawnentry();
    
    // 2. Define o nome da entidade que será criada (ex: "Burn01")
    setspawnentry("name", vName);
    
    // 3. Cria a entidade usando a função nativa universal do OpenBOR
    vSpawn = spawn();

    // 4. Verifica se a entidade foi criada com sucesso antes de tentar grudá-la
    if(vSpawn != NULL())
    {
        // Se o Galsia estiver virado para a esquerda (0), inverte a posição X do fogo
        if(dir == 0){
            fX = -fX;
        }

        // 5. Prende o fogo no Galsia
        // Ordem nativa: bindentity(filho, pai, x, z, y, direcao, trava_animacao)
        bindentity(vSpawn, self, fX, fZ, fY, dir, 0);
    }
}