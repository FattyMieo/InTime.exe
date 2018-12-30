#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <string>
#include <unistd.h>
#include <conio.h>
#include <exception>
#include <algorithm>
#include "conio_nt.h"
#include "render.h"
#include "mmsystem.h"

using namespace std;

#define NODE 254
#define KEY 236
#define GATE 254
#define BATTERY 240
#define PROGRAM_CHIP 233
#define OBJECTIVE 234

const int windowSizeX = 160, windowSizeY = 40;
const int row = 20, col = 20;

int line = 0;
bool isDrawingAsync = false;
bool UIOpening = false;
bool finalScenario = false;

char pause(int startX = windowSizeX / 2, int startY = windowSizeY - 5);
void inputString(string &str, int maxLength = 16, int startX = GetCursorPositionX(), int startY = GetCursorPositionY(), bool centerX = false);
void draw(string words, int startX, int startY, bool centerX = false, COLOR color = GRAY, bool bypass = false);
void draw(string image[], int length, int height, int startX, int startY, bool centerX = false, bool centerY = false, COLOR color = GRAY, bool bypass = false);
void typing(string words, int startX, int startY, bool centerX = false, int timeInterval = 50, COLOR color = GRAY);

struct Player;
struct Enemy;

struct Timer
{
	int time;
	
	bool countdown;
	
	Timer()
	{
		time = 0;
		countdown = true;
		for(int i = 0; i < 8; i++)
			lastDisplayed[i] = -1;
		startX = windowSizeX / 2;
		startY = windowSizeX / 4;
	}
	
	void adjustTime()
	{
		if(time < 0)				//Min = 00:00:00
			time = 0;
		else if(time >= 1048576)	//Max = F:FF:FF
			time = 1048575;
	}
	
	void setTimer(int h, int m, int s)
	{
		time = h * 3600 + m * 60 + s;
		
		adjustTime();
	}
	
	void addTime(int h, int m, int s)
	{
		time += h * 3600 + m * 60 + s;
		
		adjustTime();
	}
	
	void setHexTimer(string hex)
	{
		char *hexTime = new char[hex.length() + 1];
		strcpy(hexTime, hex.c_str());
		
		time = 0;
		
		for(int i = 5; i >= 0; i--)
		{
			int tmp = hexToInt(hexTime[i]);
			for(int j = 5; j > i; j--)
				tmp *= 16;
			time += tmp;
		}
		
		adjustTime();
	}
	
	void addHexTime(string hex)
	{
		char *hexTime = new char[hex.length() + 1];
		strcpy(hexTime, hex.c_str());
		
		for(int i = 5; i >= 0; i--)
		{
			int tmp = hexToInt(hexTime[i]);
			for(int j = 5; j > i; j--)
				tmp *= 16;
			time += tmp;
		}
		
		adjustTime();
	}
	
	void tick()
	{
		if(countdown)
			decrement();
		else
			increment();
		
		adjustTime();
	}
	
	int getHours()		{ return time / 3600; }
	
	int getHexHours()	{ return time / 65536; }
	
	int getMinutes()	{ return time % 3600 / 60; }
	
	int getHexMinutes()	{ return time % 65536 / 256; }
	
	int getSeconds()	{ return time % 3600 % 60; }
	
	int getHexSeconds()	{ return time % 65536 % 256; }
	
	string getHexTime()
	{
		return
		string()
		+ intToHex(getHexHours() / 16 <= 15 ? getHexHours() / 16 : 15)
		+ intToHex(getHexHours() % 16 <= 15 ? getHexHours() % 16 : 15)
		+ ':'
		+ intToHex(getHexMinutes() / 16 <= 15 ? getHexMinutes() / 16 : 15)
		+ intToHex(getHexMinutes() % 16 <= 15 ? getHexMinutes() % 16 : 15)
		+ ':'
		+ intToHex(getHexSeconds() / 16 <= 15 ? getHexSeconds() / 16 : 15)
		+ intToHex(getHexSeconds() % 16 <= 15 ? getHexSeconds() % 16 : 15)
		;
	}
	
	void increment()	{ time++; }
	
	void decrement()	{ time--; }
	
	//Drawing
	
	int lastDisplayed[8];
	int displayed[8];
	
	int startX, startY;
	
	void drawWindow(int x = windowSizeX / 2, int y = windowSizeY / 4)
	{
		startX = x;
		startY = y;
		draw(TIMER_WINDOW_50_5, 50, 5, startX, startY, true, true, GRAY, false);
		
		displayed[2] = displayed[5] = 16;
		
		for(int i = 0; i < 8; i++)
			lastDisplayed[i] = -1;
	}
	
	void updateWindow()
	{
		while(isDrawingAsync){while(isDrawingAsync){}}
		isDrawingAsync = true;
		int previousCursor[2] = {GetCursorPositionX(), GetCursorPositionY()};
		
		displayed[0] = (getHexHours() / 16 <= 15 ? getHexHours() / 16 : 15);
		displayed[1] = (getHexHours() % 16 <= 15 ? getHexHours() % 16 : 15);
		displayed[3] = (getHexMinutes() / 16 <= 15 ? getHexMinutes() / 16 : 15);
		displayed[4] = (getHexMinutes() % 16 <= 15 ? getHexMinutes() % 16 : 15);
		displayed[6] = (getHexSeconds() / 16 <= 15 ? getHexSeconds() / 16 : 15);
		displayed[7] = (getHexSeconds() % 16 <= 15 ? getHexSeconds() % 16 : 15);
				
		for(int i = 1; i < 8; i++)
		{
			if(lastDisplayed[i] != displayed[i])
			{
				lastDisplayed[i] = displayed[i];
				draw(NUMBER_6_3[lastDisplayed[i]], 6, 3, startX - 25 + 1 + (i * 6), startY - 3 + 2, false, false, GRAY, true);
			}
		}
		
		GoToXY(previousCursor[0], previousCursor[1]);
		isDrawingAsync = false;
	}
};

struct Player
{
	string name;
	
	int x, y, previousX, previousY;
	
	int keys;
	int batteries;
	
	static bool inBattle;
	Enemy *facedEnemy;
	
	int visionRange;
	
	static const int weaponCount = 4;
	
	int weaponLevel[weaponCount]; // Level 0 - Haven't obtained the program
	
	//Beam Cannon
	int cannonTarget;
	bool beamCannon_status;
	
	//ID Wall
	int idWall;
	bool idWall_status;
	
	//Bettle
	int leechTarget;
	int leechBank;
	bool bettle_status;
	
	//Shocker
	int shockAwakeTime;
	int shockCooldown;
	
	//Battering Ram
	int rams;
	bool ramDiscovered;
	
	Timer hextimer;
	
	Player(int x = 0, int y = 0)
	{
		this->previousX = x;
		this->x = x;
		this->previousY = y;
		this->y = y;
		
		keys = 0;
		batteries = 0;
		
		visionRange = 1;
		
		for(int i = 0; i < weaponCount; i++)
			weaponLevel[i] = 0;
		
		beamCannon_status = false;
		cannonTarget = -1;
		
		idWall = 0;
		idWall_status = false;
		
		leechBank = 0;
		bettle_status = false;
		leechTarget = -1;
		
		shockAwakeTime = 0;
		shockCooldown = weaponLevel[3] * weaponLevel[3];
		
		rams = 0;
		ramDiscovered = false;
	}
	
	string getName();
	
	void move(int x, int y);
	
	void putToMap(char (&map)[row][col]);
	
	int receiveDamage(int damage, int severity = 0);
	
	void castBeamCannon();
	
	void idWallCharge();
	
	void leeching(bool ambient = false);
	
	void shocking(bool ambient = false);
	
	void useBeamCannon(int enemy);
	
	void useIDWall();
	
	void useIDWall(bool toggle);
	
	void useBettle(int enemy);
	
	void useShocker();
	
	void useBatteringRam(int enemy);
	
	void useBattery();
};

enum ENEMY_TYPE
{
	TURRET,			//Armed with Integrity Ravager, breaks ICE Wall and Hextime mechanics
	GHOST,			//Improved Turret, has higher severity compared to Turret
	BLACK_ICE,		//Firewall that divides all incoming damage locally
	ISO_GENERATOR,	//Improvises its random algorithms for local Hextime regeneration (can increase Hextime)
};

struct Enemy
{
	static int enemyFound;
	
	int x, y;
	
	int level;
	int enemyCount = 0;
	static const int maxEnemyCount = 8;
	
	bool isShocked;
	
	Timer hextimer[maxEnemyCount];
	
	ENEMY_TYPE types[maxEnemyCount];
	
	Enemy(int x = 0, int y = 0, int level = 1)
	{
		this->x = x;
		this->y = y;
		this->level = level;
		isShocked = false;
	}
	
	void addUnit(ENEMY_TYPE type);
	
	void initializeHextimer(int i);
	
	string getName(int i);
	
	void putToMap(char (&map)[row][col]);
	
	bool isAlive();
	
	void ravageIntegrity(Player &p);
	
	int getICELevel();
	
	int receiveDamage(int i, int damage, int severity = 0);
	
	int getISOLevel();
	
	void generateISO();
};

bool Player::inBattle = false;

string Player::getName()
{
	string result = "";
	for(int i = 0; i < name.size() - 1; i++)
		result += (name.substr(i, 1) == " " ? "_" : name.substr(i, 1)) + " ";
	result += (name.substr(name.size() - 1, 1) == " " ? "_" : name.substr(name.size() - 1, 1));
	transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}

void Player::move(int x, int y)
{
	this->previousX = this->x;
	this->x += x;
	
	this->previousY = this->y;
	this->y += y;
}

void Player::putToMap(char (&map)[row][col])
{
	map[previousY][previousX] = ' ';
	map[y][x] = 'P';
}

int Player::receiveDamage(int damage, int severity)
{
	if(idWall_status && idWall > 0)
	{
		idWall -= damage;
		if(idWall < 0)
			damage = -idWall;
		else
			return 0; //No Damage Left
	}
	
	int multiplier = 1;
	for(int i = 1; i < severity; i++)
		multiplier *= 16;
	
	hextimer.time -= damage * multiplier;
	return damage * multiplier;
}

void Player::castBeamCannon()
{
	if(beamCannon_status)
	{
		facedEnemy->receiveDamage(cannonTarget, 1, weaponLevel[0]);
		if(facedEnemy->hextimer[cannonTarget].time <= 0)
			beamCannon_status = false;
	}
}

void Player::idWallCharge()
{
	if(!idWall_status)
	{
		if(idWall < weaponLevel[1] * 10)
			idWall += rand_int(1, weaponLevel[1]);
		else
			idWall = weaponLevel[1] * 10;
	}
}

void Player::leeching(bool ambient)
{
	if(bettle_status)
	{
		if(!ambient)
		{
			if(facedEnemy->hextimer[leechTarget].time > 0)
			{
				leechBank += facedEnemy->receiveDamage(leechTarget, weaponLevel[2], 0);
				if(facedEnemy->hextimer[leechTarget].time <= 0)
				{
					leechBank += facedEnemy->hextimer[leechTarget].time;
					bettle_status = false;
				}
			}
		}
	}
	else
	{
		if(leechBank > 0)
		{
			leechBank -= weaponLevel[2];
			hextimer.time += weaponLevel[2];
			if(leechBank < 0)
				hextimer.time += leechBank;
		}
	}
}

void Player::shocking(bool ambient)
{
	if(shockAwakeTime <= 0 && shockCooldown > 0)
		shockCooldown--;
	else if(!ambient)
		facedEnemy->isShocked = (--shockAwakeTime > 0);
}

void Player::useBeamCannon(int enemy)
{
	if(weaponLevel[0] > 0)
	{
		if(cannonTarget == enemy)
			beamCannon_status = !beamCannon_status;
		else
		{
			cannonTarget = enemy;
			beamCannon_status = true;
		}
	}
}

void Player::useIDWall()
{
	useIDWall(!idWall_status);
}

void Player::useIDWall(bool toggle)
{
	if(weaponLevel[1] > 0)
	{
		idWall_status = toggle;
	}
}

void Player::useBettle(int enemy)
{
	if(weaponLevel[2] > 0)
	{		
		if(leechTarget == enemy)
			bettle_status = !bettle_status;
		else
		{
			leechTarget = enemy;
			bettle_status = true;
		}
	}
}

void Player::useShocker()
{
	if(weaponLevel[3] > 0)
	{
		if(shockCooldown <= 0)
		{
			shockAwakeTime = shockCooldown = weaponLevel[3] * (weaponLevel[3] + 1);
			shockCooldown *= 10 * weaponLevel[3];
		}
	}
}

void Player::useBatteringRam(int enemy)
{
	if(rams > 0)
	{
		if(facedEnemy->hextimer[enemy].time > 0)
		{
			rams--;
			facedEnemy->hextimer[enemy].time = 0;
		}
	}
}

void Player::useBattery()
{
	if(batteries > 0)
	{
		batteries--;
		hextimer.addHexTime("00001E"); // 30 Seconds
	}
}

int Enemy::enemyFound = 0;

void Enemy::addUnit(ENEMY_TYPE type)
{
	if(enemyCount < maxEnemyCount)
	{
		types[enemyCount] = type;
		initializeHextimer(enemyCount++);
	}
}

void Enemy::initializeHextimer(int i)
{
	if(i >= 0 && i < enemyCount)
	{
		switch(types[i])
		{
			case TURRET:
				hextimer[i].time = level * 25;
				break;
			case GHOST:
				hextimer[i].time = level * 20;
				break;
			case BLACK_ICE:
				hextimer[i].time = level * 15;
				break;
			case ISO_GENERATOR:
				hextimer[i].time = level * 10;
				break;
		}
	}
}

string Enemy::getName(int i)
{
	if(i >= 0 && i < enemyCount)
	{
		switch(types[i])
		{
			case TURRET:
				return "T U R R E T";
			case GHOST:
				return "G H O S T";
			case BLACK_ICE:
				return "B L A C K _ I C E";
			case ISO_GENERATOR:
				return "I S O _ G E N";
		}
	}
}

void Enemy::putToMap(char (&map)[row][col])
{
	if(isAlive())
		map[y][x] = 'E';
}

bool Enemy::isAlive()
{
	for(int i = 0; i < enemyCount; i++)
	{
		if(hextimer[i].time > 0)
			return true;
	}
	return false;
}

void Enemy::ravageIntegrity(Player &p)
{
	for(int i = 0; i < enemyCount; i++)
	{
		if(hextimer[i].time > 0)
		{
			if(types[i] == TURRET)
				p.receiveDamage(level, 1);
			else if(types[i] == GHOST)
				p.receiveDamage(level, 2);
		}
	}
}

int Enemy::getICELevel()
{
	int lvl = 0;
	for(int i = 0; i < enemyCount; i++)
	{
		if(types[i] == BLACK_ICE && hextimer[i].time > 0)
		{
			lvl += level;
		}
	}
	return lvl;
}

int Enemy::receiveDamage(int i, int damage, int severity)
{
	if(i >= 0 && i < enemyCount)
	{
		int multiplier = 1;
		for(int s = 1; s < severity; i++)
			multiplier *= 16;
		
		hextimer[i].time -= damage * multiplier / (getICELevel() + 1);
		return damage * multiplier;
	}
	return 0;
}

int Enemy::getISOLevel()
{
	int lvl = 0;
	for(int i = 0; i < enemyCount; i++)
	{
		if(types[i] == ISO_GENERATOR && hextimer[i].time > 0)
		{
			lvl += level;
		}
	}
	return lvl;
}

void Enemy::generateISO()
{
	for(int i = 0; i < enemyCount; i++)
	{
		hextimer[i].time += getISOLevel();
	}
}

Player player;

char pause(int startX, int startY)
{
	draw("Press any key to continue . . . ", startX, startY, true);
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Flush Buffer to prevent previously typed characters(such as newline and alphabets)
	return getch();
}

void inputString(string &str, int maxLength, int startX, int startY, bool centerX)
{
	if(centerX)
		startX -= maxLength / 2;
	
	string typingLine = "";
	for(int i = 0; i < maxLength; i++)
		typingLine += "_";
	
	draw(typingLine, startX, startY, false);
	
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Flush Buffer to prevent previously typed characters(such as newline and alphabets)
	char tmp;
	int len = 0;
	string result = "";
	while(true)
	{
		GoToXY(startX + len, startY);
		tmp = getch();
		if(tmp == '\r')
		{
			if(!result.empty())
				break;
		}
		else if(tmp == '\b')
		{
			if(len > 0)
			{
				len--;
				result.erase(len);
				GoToXY(startX + len, startY);
				cout << '_';
			}
		}
		else if(len < maxLength)
		{
			result += tmp;
			GoToXY(startX + len, startY);
			cout << tmp;
			len++;
		}
	}
	str = result;
}

void draw(string words, int startX, int startY, bool centerX, COLOR color, bool bypass)
{
	int x = (centerX ? startX - (words.size() / 2) : startX);
	
	for(int i = 0; i < words.size(); i++)
	{
		while(isDrawingAsync && !bypass){while(isDrawingAsync){}}
		GoToXY(x + i, startY);
		SetFontColor(color);
		cout << words.substr(i, 1);
		SetFontColor(GRAY);
	}
}

void draw(string image[], int length, int height, int startX, int startY, bool centerX, bool centerY, COLOR color, bool bypass)
{
	int y = (centerY ? startY - (height / 2) : startY);
	
	for(int i = 0; i < height; i++)
	{
		draw(image[i], startX, y + i, centerX, color, bypass);
	}
}

void typing(string words, int startX, int startY, bool centerX, int timeInterval, COLOR color)
{
	for(int i = 0; i < words.size(); i++)
	{
		if(centerX)
			draw(words.substr(0, i + 1), startX, startY, true, color, false);
		else
			draw(words.substr(i, 1), startX + i, startY, false, color, false);
		usleep(timeInterval * 1000);
	}
}

string getRandomClue()
{
	string clue = "";
	switch(rand_int(0, 1))
	{
		case 0:
			clue = "fattymieo";
			break;
		case 1:
			clue = "nicholas";
			break;
		default:
			return "";
	}
	int random = rand_int(0, clue.size() - 1);
	string encrypted = "";
	for(int i = 0; i < random; i++)
		encrypted += ".";
	encrypted += clue.substr(random, 1);
	for(int i = random + 1; i < clue.size(); i++)
		encrypted += ".";
	return encrypted;
}

void updateNodes(char map[row][col], int playerX, int playerY, int previousX, int previousY, int nodeLengthX = 5, int nodeLengthY = 2, int startX = 0, int startY = 0, int visionRange = 1)
{	
	//Checkups
	int p_minX = previousX-visionRange;
	int p_minY = previousY-visionRange;
	int minX = playerX-visionRange;
	int minY = playerY-visionRange;
	if(p_minX < 0) p_minX = 0;
	if(p_minY < 0) p_minY = 0;
	if(minX < 0) minX = 0;
	if(minY < 0) minY = 0;
	
	int p_maxX = previousX+visionRange;
	int p_maxY = previousY+visionRange;
	int maxX = playerX+visionRange;
	int maxY = playerY+visionRange;
	if(p_maxX > col - 1) p_maxX = col - 1;
	if(p_maxY > row - 1) p_maxY = row - 1;
	if(maxX > col - 1) maxX = col - 1;
	if(maxY > row - 1) maxY = row - 1;
	
	//New
	for(int y = (p_minY * nodeLengthY) + startY; y <= (p_maxY * nodeLengthY) + startY; y++)
	{
		for(int x = (p_minX * nodeLengthX) + startX; x <= (p_maxX * nodeLengthX) + startX + 2; x++)
		{
			draw(" ", x, y);
		}
	}
	for(int y = minY; y <= maxY; y++)
	{
		for(int x = minX; x <= maxX; x++)
		{
			if(map[y][x] == 'W') //Wall
				continue;
			else
			{
				char display = char(NODE);
				COLOR color = GRAY;
				
				draw("( )", (x * nodeLengthX) + startX, (y * nodeLengthY) + startY);
				
				if(map[y][x] == ' ')		//Walkable
					color = BLACK;
				else if(map[y][x] == 'P')	//Player
					color = BLUE;
				else if(map[y][x] == 'E')	//Enemy
					color = RED;
				else if(map[y][x] == 'K')	//Key
				{
					color = MAGENTA;
					display = char(KEY);
				}
				else if(map[y][x] == 'G')	//Gate
				{
					color = DARKMAGENTA;
					display = char(GATE);
				}
				else if(map[y][x] == 'B')	//Battery
				{
					color = CYAN;
					display = char(BATTERY);
				}
				else if						//Program Chips
				(
					map[y][x] == '0' ||
					map[y][x] == '1' ||
					map[y][x] == '2' ||
					map[y][x] == '3' ||
					map[y][x] == '4'
				)
				{
					color = GREEN;
					display = char(PROGRAM_CHIP);
				}
				else if(map[y][x] == 'O')	//Objective
				{
					color = YELLOW;
					display = char(OBJECTIVE);
				}
				
				draw(string() + display, (x * nodeLengthX) + 1 + startX, (y * nodeLengthY) + startY, false, color);
				
				if(x < maxX && map[y][x + 1] != 'W') //Wall
				{
					for(int i = 3; i < nodeLengthX; i++)
						draw(string()+char(196), (x * nodeLengthX) + i + startX, (y * nodeLengthY) + startY);
				}
				
				if(y < maxY && map[y + 1][x] != 'W')
				{
					for(int i = 1; i < nodeLengthY; i++)
						draw("|", (x * nodeLengthX) + 1 + startX, (y * nodeLengthY) + i + startY);
				}
			}
		}
	}
}

//Detail of the player
void updateDetails(int startX = 0, int startY = 0)
{
	int l = 20;
	
	//Battery
	draw(string()+"Amount: "+intToHex(player.batteries / 16)+intToHex(player.batteries % 16), startX - 6, startY + l++, false);
	
	l += 2;
	
	//Keys
	draw(string()+"Amount: "+intToHex(player.keys / 16)+intToHex(player.keys % 16), startX - 6, startY + l++, false);
}

void showDetails(int startX = 0, int startY = 0)
{
	int l = 0;
	draw("# -> INDICATORS", startX - 10, startY + l++, false);
	l += 1;
	draw(string()+char(NODE)+string(" -> Player"), startX, startY + l++, false, BLUE);
	draw(string()+char(NODE)+string(" -> Enemy"), startX, startY + l++, false, RED);
	draw(string()+char(KEY)+string(" -> Key"), startX, startY + l++, false, MAGENTA);
	draw(string()+char(GATE)+string(" -> Gate"), startX, startY + l++, false, DARKMAGENTA);
	draw(string()+char(BATTERY)+string(" -> Battery"), startX, startY + l++, false, CYAN);
	draw(string()+char(PROGRAM_CHIP)+string(" -> Program Chips"), startX, startY + l++, false, GREEN);
	draw(string()+char(OBJECTIVE)+string(" -> Objective"), startX, startY + l++, false, YELLOW);
	l += 2;
	draw(">> - Move", startX - 10, startY + l, false);
	draw("[W][A][S][D]", startX + 20, startY + l++, true);
	l += 2;
	draw(string()+char(PROGRAM_CHIP)+" -> PROGRAMS", startX - 10, startY + l, false);
	draw("[E]", startX + 20, startY + l++, true);
	l += 2;
	draw(string()+char(OBJECTIVE)+" -> INVENTORY", startX - 10, startY + l++, false);
	l += 1;
	draw(string()+char(BATTERY)+" Battery - Gain Hextime when used.", startX - 10, startY + l++, false);
	draw("[Q]", startX + 20, startY + l++, true);
	l += 1;
	draw(string()+char(KEY)+" Key - Unlock gates when encountered.", startX - 10, startY + l++, false);
	updateDetails(startX, startY);
}

void collision(char map[row][col], Player &p, Enemy enemies[], int scene = 1)
{
	int startY = windowSizeY - 5;
	GoToXY(0, startY);
	ClrLine();
	GoToXY(0, startY + 1);
	ClrLine();
	if(map[p.y][p.x] == 'W')
	{
		p.x = p.previousX;
		p.y = p.previousY;
		draw("Inaccessible node!", windowSizeX * 5 / 6, startY, true, DARKRED);
		return;
	}
	
	if(map[p.y][p.x] == 'K')
	{
		p.keys++;
		draw("Key obtained!", windowSizeX * 5 / 6, startY, true, MAGENTA);
		draw("<Press [Q] to use>", windowSizeX * 5 / 6, startY + 1, true);
		return;
	}
	
	if(map[p.y][p.x] == 'G')
	{
		if(p.keys > 0)
		{
			p.keys--;
			draw("Gate unlocked!", windowSizeX * 5 / 6, startY, true, GREEN);
		}
		else
		{
			p.x = p.previousX;
			p.y = p.previousY;
			draw("Gate locked!", windowSizeX * 5 / 6, startY, true, DARKGREEN);
		}
		return;
	}
	
	if(map[p.y][p.x] == 'B')
	{
		p.batteries++;
		draw("Battery obtained!", windowSizeX * 5 / 6, startY, true, CYAN);
		draw("<Press [Q] to use>", windowSizeX * 5 / 6, startY + 1, true);
		return;
	}
	
	if(map[p.y][p.x] == '0')
	{
		if(p.weaponLevel[0]++ <= 0)
			draw("Beam Cannon obtained!", windowSizeX * 5 / 6, startY, true);
		else
			draw("Beam Cannon upgraded!", windowSizeX * 5 / 6, startY, true);
		draw("<Press [E] to open inventory>", windowSizeX * 5 / 6, startY + 1, true);
		return;
	}
	
	if(map[p.y][p.x] == '1')
	{
		if(p.weaponLevel[1]++ <= 0)
			draw("I.C.E Shield obtained!", windowSizeX * 5 / 6, startY, true);
		else
			draw("I.C.E Shield upgraded!", windowSizeX * 5 / 6, startY, true);
		draw("<Press [E] to open inventory>", windowSizeX * 5 / 6, startY + 1, true);
		return;
	}
	
	if(map[p.y][p.x] == '2')
	{
		if(p.weaponLevel[2]++ <= 0)
			draw("Data Leech obtained!", windowSizeX * 5 / 6, startY, true);
		else
			draw("Data Leech upgraded!", windowSizeX * 5 / 6, startY, true);
		draw("<Press [E] to open inventory>", windowSizeX * 5 / 6, startY + 1, true);
		return;
	}
	
	if(map[p.y][p.x] == '3')
	{
		if(p.weaponLevel[3]++ <= 0)
			draw("Shocker obtained!", windowSizeX * 5 / 6, startY, true);
		else
			draw("Shocker upgraded!", windowSizeX * 5 / 6, startY, true);
		draw("<Press [E] to open inventory>", windowSizeX * 5 / 6, startY + 1, true);
		return;
	}
	
	if(map[p.y][p.x] == '4')
	{
		p.ramDiscovered = true;
		p.rams++;
		draw("Battering Ram obtained!", windowSizeX * 5 / 6, startY, true);
		draw("<Press [E] to open inventory>", windowSizeX * 5 / 6, startY + 1, true);
		return;
	}
	
	if(map[p.y][p.x] == 'O')
	{
		draw("Objective reached!", windowSizeX * 5 / 6, startY, true, YELLOW);
		//Get A Clue
		draw(string()+"Found hint ["+getRandomClue()+"]", windowSizeX * 5 / 6, startY + 1, true);
		return;
	}
	
	for(int i = 0; i < Enemy::enemyFound; i++)
	{
		if(enemies[i].x == p.x && enemies[i].y == p.y)
		{
			if(enemies[i].isAlive())
			{
				draw("Enemy collided!", windowSizeX * 5 / 6, startY, true, RED);
				p.facedEnemy = &enemies[i];
				p.inBattle = true;
			}
			return;
		}
	}
}

void enemyTimeTick()
{
	if(!player.facedEnemy->isShocked)
	{
		for(int i = 0; i < player.facedEnemy->enemyCount; i++)
		{
			if(player.facedEnemy->hextimer[i].time > 0)
				player.facedEnemy->hextimer[i].tick();
		}
	}
}

void battleTimeUpdate()
{
	int previousCursor[2] = {GetCursorPositionX(), GetCursorPositionY()};
	
	draw(player.hextimer.getHexTime().substr(1, 7), (windowSizeX / 3), (windowSizeY / 2) - 3, true, GRAY, true);
	for(int i = 0; i < player.facedEnemy->enemyCount; i++)
	{
		string time;
		COLOR color;
		if(player.facedEnemy->hextimer[i].time > 0) //Alive
		{
			time = player.facedEnemy->hextimer[i].getHexTime().substr(3, 5);
			color = GRAY;
		}
		else
		{
			time = "--:--";
			color = RED;
		}
		draw(time, (windowSizeX * 2 / 3) + (1 * player.facedEnemy->enemyCount * (i % 2 == 0 ? -1 : 1)), (windowSizeY * (i + 1) / (player.facedEnemy->enemyCount + 1)) - 3, true, color, true);
	}
	
	GoToXY(previousCursor[0], previousCursor[1]);
}

void drawBattleUnits()
{
	draw(player.getName(), (windowSizeX / 3), (windowSizeY / 2) - 4, true, DARKMAGENTA);
	draw(PLAYER_6_3, 6, 3, (windowSizeX / 3), (windowSizeY / 2), true, true, DARKMAGENTA);
	
	for(int i = 0; i < player.facedEnemy->enemyCount; i++)
	{
		COLOR color;
		string image[3];
		switch(player.facedEnemy->types[i])
		{
			case TURRET:
				color = YELLOW;
				copy(begin(TURRET_6_3), end(TURRET_6_3), begin(image));
				break;
			case GHOST:
				color = CYAN;
				copy(begin(GHOST_6_3), end(GHOST_6_3), begin(image));
				break;
			case BLACK_ICE:
				color = DARKGRAY;
				copy(begin(BLACK_ICE_6_3), end(BLACK_ICE_6_3), begin(image));
				break;
			case ISO_GENERATOR:
				color = DARKGREEN;
				copy(begin(ISO_GENERATOR_6_3), end(ISO_GENERATOR_6_3), begin(image));
				break;
		}
		draw(player.facedEnemy->getName(i), (windowSizeX * 2 / 3) + (1 * player.facedEnemy->enemyCount * (i % 2 == 0 ? -1 : 1)), (windowSizeY * (i + 1) / (player.facedEnemy->enemyCount + 1)) - 4, true, color);
		draw(image, 6, 3, (windowSizeX * 2 / 3) + (1 * player.facedEnemy->enemyCount * (i % 2 == 0 ? -1 : 1)), (windowSizeY * (i + 1) / (player.facedEnemy->enemyCount + 1)), true, true, color);
	}
}

void updateActions()
{
	int previousCursor[2] = {GetCursorPositionX(), GetCursorPositionY()};
	
	int len = 11;
	//ID Wall
	if(player.weaponLevel[1] > 0)
		draw(string()+"<"+intToHex(player.idWall / 16)+intToHex(player.idWall % 16)+">", windowSizeX / 3 + 4, windowSizeY / 2 + len++, false, (player.idWall > 0 ? (player.idWall >= player.weaponLevel[1] * 10 ? GREEN : DARKGREEN) : DARKGRAY), true);
	else len++;
	//Bettle
	if(player.weaponLevel[2] > 0)
		draw(string()+"<"+intToHex(player.leechBank / 16)+intToHex(player.leechBank % 16)+">", windowSizeX / 3 + 4, windowSizeY / 2 + len++, false, (player.leechBank > 0 ? (player.bettle_status ? GREEN : DARKGREEN) : DARKGRAY), true);
	else len++;
	//Shocker
	if(player.weaponLevel[3] > 0)
	{
		if(player.shockAwakeTime <= 0 && player.shockCooldown > 0)
		draw(string()+intToHex(player.shockCooldown / 16)+intToHex(player.shockCooldown % 16)+"  ", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, DARKGRAY, true);
	}
	else len++;
	GoToXY(previousCursor[0], previousCursor[1]);
}

void drawActions()
{
	int len = 4;
	
	draw("[W][A][S][D] Select Target", windowSizeX / 3, windowSizeY / 2 + len++, true);
	
	len += 1;
	
	draw(" [Q] Battery", windowSizeX / 3 - 20, windowSizeY / 2 + len, false);
	draw(string()+intToHex(player.batteries / 16)+intToHex(player.batteries % 16), windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, (player.batteries > 0 ? GREEN : GRAY));
	
	if(player.ramDiscovered)
	{
	draw(" [E] Battering Ram", windowSizeX / 3 - 20, windowSizeY / 2 + len, false);
	draw(string()+intToHex(player.rams / 16)+intToHex(player.rams % 16), windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, (player.rams > 0 ? GREEN : GRAY));
	}
	else len++;
	
	len += 2;
	
	if(player.weaponLevel[0] > 0)
	{
	draw(" [1] BEAM Cannon", windowSizeX / 3 - 20, windowSizeY / 2 + len, false);
	if(player.beamCannon_status)
		draw("RUN ", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, CYAN);
	else
		draw("WAIT", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, DARKCYAN);
	}
	else len++;
	
	if(player.weaponLevel[1] > 0)
	{
	draw(" [2] ID Wall", windowSizeX / 3 - 20, windowSizeY / 2 + len, false);
	if(player.idWall_status)
		draw("RUN   ", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, CYAN);
	else
		draw("WAIT  ", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, DARKCYAN);
	}
	else len++;
	
	if(player.weaponLevel[2] > 0)
	{
	draw(" [3] Bettle", windowSizeX / 3 - 20, windowSizeY / 2 + len, false);
	if(player.bettle_status)
		draw("RUN ", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, CYAN);
	else
		draw("WAIT", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, DARKCYAN);
	}
	else len++;
	
	if(player.weaponLevel[3] > 0)
	{
	draw(" [4] Shocker", windowSizeX / 3 - 20, windowSizeY / 2 + len, false);
	if(player.shockAwakeTime > 0)
		draw("RUN ", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, CYAN);
	else if(player.shockCooldown <= 0)
		draw("WAIT", windowSizeX / 3 + 10, windowSizeY / 2 + len++, false, DARKCYAN);
	}
	else len++;
}

int option = 0;

void drawBattlePtr(char c)
{
	draw(string()+c, (windowSizeX * 2 / 3) + ((1 * player.facedEnemy->enemyCount + 5) * (option % 2 == 0 ? -1 : 1)), (windowSizeY * (option + 1) / (player.facedEnemy->enemyCount + 1)), true);
}

typedef void (*f)();
void load(void (*f)())
{
	ClrScreen();
	line = 0;
	player.hextimer.drawWindow(windowSizeX - 26, 3);
	player.hextimer.updateWindow();
	
    // http://en.cppreference.com/w/cpp/thread/async
    auto future = async(launch::async, (*f));

    // http://en.cppreference.com/w/cpp/thread/future/wait_for
    while(future.wait_for(chrono::seconds(1)) != future_status::ready)
    {
		player.hextimer.tick();
		
		if(player.inBattle)
		{
			enemyTimeTick();
			player.castBeamCannon();
			player.idWallCharge();
			player.leeching(false);
			player.shocking(false);
			player.facedEnemy->ravageIntegrity(player);
			player.facedEnemy->generateISO();
			
			while(isDrawingAsync){while(isDrawingAsync){}}
			isDrawingAsync = true;
			battleTimeUpdate();
			updateActions();
			isDrawingAsync = false;
		}
		else
		{
			player.idWallCharge();
			player.leeching(true);
			player.shocking(true);
			
			if(!UIOpening)
				player.hextimer.updateWindow();
		}
		
		if(player.hextimer.time <= 0)
		{
			isDrawingAsync = true; //Halts all drawing updates
			PlaySound(NULL, NULL, SND_ASYNC);
			system("cls");
			SetFontColor(GREEN);
			for(int y = 0; y < windowSizeY * 2; y++)
			{
				for(int x = 0; x < windowSizeX; x++)
				{
					cout << char(rand_int(32, 126));
				}
				cout << endl;
			}
			cout << "\a" << endl;
			SetFontColor(RED);
			throw exception();
		}
	}
}

void battlePhase()
{
	//Draw Units in the battle
	drawBattleUnits();
	
	//Draw & Update Action List
	drawActions();
	updateActions();
	
	//Initialize and Show Pointer
	option = 0;
	drawBattlePtr(FBLOCK[3]);
	
	//Show Time
	battleTimeUpdate();
	
	while(player.inBattle)
	{
		//Put the Cursor out of the screen
		while(isDrawingAsync){while(isDrawingAsync){}}
		GoToXY(windowSizeX - 1, windowSizeY - 1);
		
		//If player presses a key, do the respective action
		while(kbhit()) //If a key is hit
		{
			char input = getch();
			drawBattlePtr(' ');
			//Select Target
			if(input == 'w' || input == 'W' || input == 'a' || input == 'A')
			{
				option -= 1;
				if(option < 0)
					option += 1;
			}
			else if(input == 's' || input == 'S' || input == 'd' || input == 'D')
			{
				option += 1;
				if(option >= player.facedEnemy->enemyCount)
					option -= 1;
			}
			//Tools
			else if(input == 'q' || input == 'Q')
			{
				player.useBattery();
			}
			else if(input == 'e' || input == 'E')
			{
				player.useBatteringRam(option);
			}
			//Programs
			else if(input == '1')
			{
				player.useBeamCannon(option);
			}
			else if(input == '2')
			{
				player.useIDWall();
			}
			else if(input == '3')
			{
				player.useBettle(option);
			}
			else if(input == '4')
			{
				player.useShocker();
			}
			drawBattlePtr(FBLOCK[3]);
			
			drawActions();
			updateActions();
			battleTimeUpdate();
		}
		
		player.inBattle = player.facedEnemy->isAlive();
	}
	
	//Player Status
	player.beamCannon_status = false;
	player.cannonTarget = -1;
	player.idWall_status = false;
	player.bettle_status = false;
	player.leechTarget = -1;
	player.shockAwakeTime = 0;
	
	//Rewards
	int rewardBatteries = 0;
	for(int i = 0; i < player.facedEnemy->enemyCount; i++)
	{
		rewardBatteries += player.facedEnemy->level;
	}
	player.batteries += rewardBatteries;
}

void showPrograms()
{
	int showcaseItems = 0;
	int line = 1;
	
	for(int i = 0; i < player.weaponCount; i++)
	{
		if(player.weaponLevel[i] > 0)
			showcaseItems++;
	}
	
	if(player.ramDiscovered)
		showcaseItems++;
		
	if(showcaseItems <= 0) //Do not show if there is no discovered programs
		return;
	
	UIOpening = true;
	
	if(player.weaponLevel[0] > 0)
	{
		draw("BEAM Cannon - LVL "+to_string(player.weaponLevel[0]), windowSizeX / 4, windowSizeY * line / (showcaseItems + 1) - 3, true);
		draw(BEAM_CANNON_6_3, 6, 3, windowSizeX / 4, windowSizeY * line / (showcaseItems + 1), true, true);
		
		draw("Attack Algorithm, aka \"Base Erase Assignment Module\"", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 3, false);
		draw("-> Send destructive algorithm that minus enemy\'s Hextime.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 1, false);
		draw("-> BEAM Cannon with higher level deals more severe damage.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) + 0, false);
		
		line++;
	}
	
	if(player.weaponLevel[1] > 0)
	{
		draw("ID Wall - LVL "+to_string(player.weaponLevel[1]), windowSizeX / 4, windowSizeY * line / (showcaseItems + 1) - 3, true);
		draw(ID_WALL_6_3, 6, 3, windowSizeX / 4, windowSizeY * line / (showcaseItems + 1), true, true);
		
		draw("Defense Algorithm, aka \"Integrity Defense Wall\"", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 3, false);
		draw("-> Open a temporary shield. Recharge when unused.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 1, false);
		draw("-> Any damage will deplete the shield first instead of Hextime.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) + 0, false);
		draw("-> Receive only plain damage, not affected by severity.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) + 1, false);
		
		line++;
	}
	
	if(player.weaponLevel[2] > 0)
	{
		draw("Bettle - LVL "+to_string(player.weaponLevel[2]), windowSizeX / 4, windowSizeY * line / (showcaseItems + 1) - 3, true);
		draw(BETTLE_6_3, 6, 3, windowSizeX / 4, windowSizeY * line / (showcaseItems + 1), true, true);
		
		draw("Leeching Algorithm", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 3, false);
		draw("-> Implant on an enemy to increase the rate of decay of Hextime.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 1, false);
		draw("-> Transfer stolen Hextime to its master when deactivated.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) + 0, false);
		
		line++;
	}
	
	if(player.weaponLevel[3] > 0)
	{
		draw("Shocker - LVL "+to_string(player.weaponLevel[3]), windowSizeX / 4, windowSizeY * line / (showcaseItems + 1) - 3, true);
		draw(SHOCKER_6_3, 6, 3, windowSizeX / 4, windowSizeY * line / (showcaseItems + 1), true, true);
		
		draw("Paralysis Algorithm", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 3, false);
		draw("-> Disable an enemy for a short amount of time.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 1, false);
		draw("-> However, it halts the decaying of Hextime.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) + 0, false);
		
		line++;
	}
	
	if(player.ramDiscovered)
	{
		draw("Battering Ram", windowSizeX / 4, windowSizeY * line / (showcaseItems + 1) - 3, true);
		draw(BATTERING_RAM_6_3, 6, 3, windowSizeX / 4, windowSizeY * line / (showcaseItems + 1), true, true);
		
		draw("BruteForce Algorithm", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 3, false);
		draw("-> Infiltrate enemy\'s system forcefully.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) - 1, false);
		draw("-> It divides its victim with zeroes.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) + 0, false);
		draw("-> Self-desturct with the victim.", windowSizeX * 2 / 4 - 20, windowSizeY * line / (showcaseItems + 1) + 1, false);
		
		line++;
	}
	
	pause(windowSizeX / 2, windowSizeY - 3);
	UIOpening = false;
}

void beginning()
{
	ClrScreen();
	line = 0;
	typing("> Cleared!", 0, line++, false, 10);
	
	typing("> Welcome, customer!", 0, line++, false, 10);
	typing("> Thank you for using InTime by Time Inc.", 0, line++, false, 10);
	typing("> Searching for online representative . . .", 0, line++, false, 10);
	typing("> Please hold . . .", 0, line++, false, 10);
	sleep(2);
	typing("[WARN] Intrusion detected, shutting down . . . ", 0, line++, false, 10);
	sleep(1);
	typing("[WARN] Core overwritten!", 0, line++, false, 10);
	for(int i = 10; i >= 1; i--)
	{
		typing("[WARN] Core molten!", 0, line++, false, i);
		typing("08 -- Service interrupted! -- 08", 0, line++, false, i);
	}
	sleep(1);
	
	ClrScreen();
	line = 0;
	typing("> Cleared!", 0, line++, false, 10);
	
	SetConsoleTitle("InTime.exe @ system_08");
	typing("> System reinitialized, system_08 registered!", 0, line++, false, 10);
	sleep(1);
	typing("> Welcome, _HACK!", 0, line++, false, 10);
	typing("> Thank @%# for using InTime by @%#$ @%#.", 0, line++, false, 10);
	typing("> Searching @%# online representative . . .", 0, line++, false, 10);
	typing("> Please _HACK . . .", 0, line++, false, 10);
	sleep(2);
	typing("> Representative found!", 0, line++, false, 10);
	typing("> admin08 has joined the network!", 0, line++, false, 10);
	typing("> Connecting . . .", 0, line++, false, 10);
	sleep(1);
	typing("> Connected!", 0, line++, false, 10);
	
	typing("Hello.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Welcome to Eight.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("I\'m your admin to guide you through this system.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("I have overwritten your executable, it\'s now available for rootkit.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	draw("________________", windowSizeX / 2, windowSizeY - 5, true);
	
	typing("See that line below? Name yourself.", windowSizeX / 2, windowSizeY / 2, true);
	
	inputString(player.name, 16, windowSizeX / 2, windowSizeY - 5, true);
	
	GoToXY(0, GetCursorPositionY());
	ClrLine();
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing(player.name + string(", interesting name..."), windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("So you are our new recruit huh?", windowSizeX / 2, windowSizeY / 2, true);
	
	typing("> Initializing TrainingProgram_V2.1B.cpp . . .", 0, line++, false, 10);
	sleep(1);
	typing("> Initialized TrainingProgram_V2.1B.cpp!", 0, line++, false, 10);
	typing("> [TP] tag successfully registered!", 0, line++, false, 10);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Let\'s get started.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	ClrScreen();
	line = 0;
	typing("> Cleared!", 0, line++, false, 10);
	
	typing("[TP] Training Program by admin08.", 0, line++, false, 10);
	typing("[TP] Version: 2.1", 0, line++, false, 10);
	typing("[TP] Revision B", 0, line++, false, 10);
	typing("[TP] Loading . . .", 0, line++, false, 10);
	sleep(1);
	typing("[TP] Initializing Training.cpp . . .", 0, line++, false, 10);
	sleep(1);
	typing("[TP] Initialized Training.cpp!", 0, line++, false, 10);
	typing("[TP] User(s) detected:", 0, line++, false, 10);
	typing(string("[TB] - ") + player.name, 0, line++, false, 10);
	typing("[TP] - admin08", 0, line++, false, 10);
	typing("[TP] --------------------------------------------------", 0, line++, false, 10);
	typing("[TP] Please wait . . .", 0, line++, false, 10);
	sleep(2);
	typing("> Connection established!", 0, line++, false, 10);
	player.hextimer.setHexTimer("00003C");
	typing("> Hextimer is set to 00003C.", 0, line++, false, 10);
	sleep(1);
}

void training()
{
	typing("This is your Hextime.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("The console will disconnect you when you are timeout.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("This is to protect intruders from remotely accessing their files.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Unfortunately I can\'t overwrite that function, I must avert triggering the security.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Restart this console to reconnect Time Inc.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Type in Passcodes at the main menu to recall to memory locations.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Here\'s one for you.", windowSizeX / 2, windowSizeY / 2, true);
	typing("\"code08\"", windowSizeX / 2, windowSizeY / 2 + 1, true);
	sleep(4);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	GoToXY(0, windowSizeY / 2 + 1);
	ClrLine();
	
	typing("Restart this console later, time\'s running out.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Enjoy your journey!", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Remember to look at the guide listed at RIGHT side.", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	typing("> admin08 has left the network!", 0, line++, false, 10);
	sleep(1);
}

void nicholas()
{
	string checkName;
	transform(player.name.begin(), player.name.end(), back_inserter(checkName), ::tolower);
	if(checkName == "nicholas")
	{
		typing("[$] Nicholas detected!", 0, line++, false, 10);
		sleep(1);
		SetFontColor(GREEN);
		typing("[$] Nicholas mode activated!", 0, line++, false, 10);
		player.batteries = 100;
		typing("[$] Gain 100 Batteries!", 0, line++, false, 10);
		player.keys = 100;
		typing("[$] Gain 100 Keys!", 0, line++, false, 10);
		player.rams = 100;
		player.ramDiscovered = true;
		typing("[$] Gain 100 Battering Rams!", 0, line++, false, 10);
		for(int i = 0; i < player.weaponCount; i++)
			player.weaponLevel[i] = 2;
		typing("[$] Unlocked all weapons!", 0, line++, false, 10);
		typing("[$] Upgraded all weapons to level 2!", 0, line++, false, 10);
		player.visionRange = 20;
		typing("[$] Unlocked full vision! Expect rendering lags! >_<", 0, line++, false, 10);
		SetFontColor(GRAY);
		sleep(1);
	}
}

void pre_scenario1()
{
	ClrScreen();
	line = 0;
	typing("> Cleared!", 0, line++, false, 10);
	
	typing("> Initializing TrainingProgram_V2.1B.cpp . . .", 0, line++, false, 10);
	sleep(1);
	typing("> Initialized TrainingProgram_V2.1B.cpp!", 0, line++, false, 10);
	typing("> [TP] tag successfully registered!", 0, line++, false, 10);
	typing("[TP] Initializing Scenario1.cpp . . .", 0, line++, false, 10);
	sleep(1);
	typing("[TP] Initialized Scenario1.cpp!", 0, line++, false, 10);
	sleep(1);
	nicholas();
	typing("> Connection established!", 0, line++, false, 10);
	player.hextimer.setHexTimer("00003C");
	typing("> Hextimer is set to 00003C.", 0, line++, false, 10);
	sleep(1);
	
	ClrScreen();
	line = 0;
	player.hextimer.drawWindow(windowSizeX - 26, 3);
	player.hextimer.updateWindow();
	
	typing("<system_08> [TP]", windowSizeX / 2, windowSizeY / 2 - 1, true, 20);
	typing("Scenario #1: Retrieve all objectives for the next passcode.", windowSizeX / 2, windowSizeY / 2, true, 10);
	pause();
}

void scenario1()
{
	char map[row][col] =
	{
		{'W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W'},
		{'W','P','0','E','1','E','B',' ','E','W','B','0','K','1','B','W','K','G','B','3'},
		{'W',' ','W','B','W','W','W','W',' ','W','W','W','G','W','K','W','B','W','B','W'},
		{'W',' ','W','B','W','W','W','W','B','W','E','B','B','W','W','W','B','W','B','W'},
		{'W','E','W',' ','E','B','K','W','B','W',' ','W','B',' ','E','B','E','W','G','W'},
		{'W','B','W','W','W','W','W','W','K','G',' ',' ','E','W','2','W','W','G','B','W'},
		{'W','K','B','W','E','B','B',' ','E','W','W','W',' ','W','W','W','G','B','W','W'},
		{'W','W','G','W','K','W','W','G','W','W','K','G',' ',' ',' ','W',' ','W','W','W'},
		{'W','B','B','W','E','B','B','E','K','W','0','W','E','W',' ','G',' ',' ','E','W'},
		{'W','G','W','W','W','W','W','G','W','W','W','W','G','W','W','W','W','W','B','W'},
		{'W','E','G','K','K','E','G','E','E','K','B','K','E','E','W','B','W','W','G','W'},
		{'W','B','W','G','W','W','E','W','W','W','W','W','W','G','W','B','W','W','2','W'},
		{'W','W','W','E','K','W','B','W','E','G','B','W',' ','3','E','E',' ',' ','E','W'},
		{'O','O','W','K','E','G','B','W','B','W','B','W',' ','W','W','W',' ',' ','W','W'},
		{'O','O','W','W','W','W','W','W','E','W','B','W',' ','E','E','W','G','W','W','W'},
		{'O','O','W','E','K','G',' ','W',' ','W','B','W',' ','W','1','G','2','W','K','W'},
		{'O','O','W','E','W','W',' ',' ',' ','W','B','W','E','W','B','W','B','W','K','W'},
		{'O','O','W','E','W','W','W','W',' ','W','B','W','B','W','B','W','W','W','B','W'},
		{'O','O','G','E','W','3','G',' ','B','W','B','G','E','W',' ','G','B','B','0','1'},
		{'W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W'}
	};
	
	player.previousX = player.x = 1;
	player.previousY = player.y = 1;
	
	Enemy::enemyFound = 37;
	
	Enemy enemies[37] = 
	{
		//Level 1
		Enemy(3, 1, 1),
		Enemy(5, 1, 1),
		Enemy(1, 4, 1),
		Enemy(4, 4, 1),
		Enemy(8, 1, 1),
		Enemy(8, 6, 1),
		Enemy(4, 6, 1),
		Enemy(4, 8, 1),
		Enemy(7, 8, 1),
		Enemy(10, 3, 1),
		Enemy(12, 5, 1),
		Enemy(14, 4, 1),
		Enemy(16, 4, 1),
		Enemy(12, 8, 1),
		Enemy(18, 8, 1),
		//Level 2
		Enemy(1, 10, 2),
		Enemy(5, 10, 2),
		Enemy(7, 10, 2),
		Enemy(8, 10, 2),
		Enemy(12, 10, 2),
		Enemy(13, 10, 2),
		Enemy(6, 11, 2),
		Enemy(3, 12, 2),
		Enemy(4, 13, 2),
		Enemy(18, 12, 2),
		Enemy(14, 12, 2),
		Enemy(15, 12, 2),
		Enemy(13, 14, 2),
		Enemy(14, 14, 2),
		//Level 3
		Enemy(8, 14, 3),
		Enemy(8, 12, 3),
		Enemy(12, 16, 3),
		Enemy(12, 18, 3),
		//Level 4
		Enemy(3, 15, 4),
		Enemy(3, 16, 4),
		Enemy(3, 17, 4),
		Enemy(3, 18, 4)
	};
	
	//Level 1
	enemies[0].addUnit(TURRET);
	
	enemies[1].addUnit(GHOST);
	
	enemies[2].addUnit(TURRET);
	
	enemies[3].addUnit(TURRET);
	enemies[3].addUnit(TURRET);
	
	enemies[4].addUnit(TURRET);
	enemies[4].addUnit(TURRET);
	
	enemies[5].addUnit(GHOST);
	
	enemies[6].addUnit(TURRET);
	enemies[6].addUnit(ISO_GENERATOR);
	
	enemies[7].addUnit(GHOST);
	enemies[7].addUnit(GHOST);
	
	enemies[8].addUnit(TURRET);
	enemies[8].addUnit(TURRET);
	enemies[8].addUnit(TURRET);
	
	enemies[9].addUnit(TURRET);
	enemies[9].addUnit(BLACK_ICE);
	
	enemies[10].addUnit(TURRET);
	enemies[10].addUnit(BLACK_ICE);
	
	enemies[11].addUnit(TURRET);
	enemies[11].addUnit(TURRET);
	enemies[11].addUnit(BLACK_ICE);
	
	enemies[12].addUnit(GHOST);
	enemies[12].addUnit(BLACK_ICE);
	
	enemies[13].addUnit(TURRET);
	enemies[13].addUnit(GHOST);
	enemies[13].addUnit(BLACK_ICE);
	enemies[13].addUnit(ISO_GENERATOR);
	
	enemies[14].addUnit(TURRET);
	enemies[14].addUnit(GHOST);
	enemies[14].addUnit(BLACK_ICE);
	enemies[14].addUnit(ISO_GENERATOR);
	
	//Level 2
	enemies[15].addUnit(GHOST);
	
	enemies[16].addUnit(GHOST);
	enemies[16].addUnit(GHOST);
	
	enemies[17].addUnit(GHOST);
	enemies[17].addUnit(GHOST);
	
	enemies[18].addUnit(GHOST);
	enemies[18].addUnit(GHOST);
	
	enemies[19].addUnit(TURRET);
	enemies[19].addUnit(TURRET);
	enemies[19].addUnit(BLACK_ICE);
	
	enemies[20].addUnit(BLACK_ICE);
	enemies[20].addUnit(ISO_GENERATOR);
	enemies[20].addUnit(ISO_GENERATOR);
	
	enemies[21].addUnit(TURRET);
	enemies[21].addUnit(TURRET);
	enemies[21].addUnit(ISO_GENERATOR);
	enemies[21].addUnit(ISO_GENERATOR);
	
	enemies[22].addUnit(GHOST);
	enemies[22].addUnit(BLACK_ICE);
	enemies[22].addUnit(ISO_GENERATOR);
	
	enemies[23].addUnit(GHOST);
	enemies[23].addUnit(BLACK_ICE);
	enemies[23].addUnit(ISO_GENERATOR);
	
	enemies[24].addUnit(BLACK_ICE);
	enemies[24].addUnit(ISO_GENERATOR);
	
	enemies[25].addUnit(TURRET);
	enemies[25].addUnit(TURRET);
	
	enemies[26].addUnit(GHOST);
	enemies[26].addUnit(ISO_GENERATOR);
	
	enemies[27].addUnit(ISO_GENERATOR);
	enemies[27].addUnit(ISO_GENERATOR);
	enemies[27].addUnit(ISO_GENERATOR);
	enemies[27].addUnit(ISO_GENERATOR);
	
	enemies[28].addUnit(BLACK_ICE);
	enemies[28].addUnit(BLACK_ICE);
	enemies[28].addUnit(BLACK_ICE);
	enemies[28].addUnit(BLACK_ICE);
	
	//level 3
	enemies[29].addUnit(TURRET);
	enemies[29].addUnit(GHOST);
	enemies[29].addUnit(BLACK_ICE);
	enemies[29].addUnit(ISO_GENERATOR);
	
	enemies[30].addUnit(TURRET);
	enemies[30].addUnit(GHOST);
	enemies[30].addUnit(BLACK_ICE);
	enemies[30].addUnit(ISO_GENERATOR);
	
	enemies[31].addUnit(TURRET);
	enemies[31].addUnit(GHOST);
	enemies[31].addUnit(BLACK_ICE);
	enemies[31].addUnit(ISO_GENERATOR);
	
	enemies[32].addUnit(TURRET);
	enemies[32].addUnit(GHOST);
	enemies[32].addUnit(BLACK_ICE);
	enemies[32].addUnit(ISO_GENERATOR);
	
	//Level 4
	enemies[33].addUnit(TURRET);
	
	enemies[34].addUnit(TURRET);
	enemies[34].addUnit(GHOST);
	
	enemies[35].addUnit(GHOST);
	enemies[35].addUnit(GHOST);
	enemies[35].addUnit(BLACK_ICE);
	
	enemies[36].addUnit(GHOST);
	enemies[36].addUnit(GHOST);
	enemies[36].addUnit(BLACK_ICE);
	enemies[36].addUnit(ISO_GENERATOR);
	
	for(int i = 0; i < Enemy::enemyFound; i++) enemies[i].putToMap(map);
	player.putToMap(map);
	
	updateNodes(map, player.x, player.y, player.x, player.y, 5, 2, 2, 1, player.visionRange);
	showDetails(windowSizeX * 5 / 6 - 8, 8);
	
	while(true)
	{
		//Put the Cursor out of the screen
		while(isDrawingAsync){while(isDrawingAsync){}}
		GoToXY(windowSizeX - 1, windowSizeY - 1);
		
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Flush Buffer to prevent previously typed characters(such as newline and alphabets)
		char tmp = getch();
		
		if(tmp == 'w' || tmp == 'W')
			player.move(0, -1);
		else if(tmp == 's' || tmp == 'S')
			player.move(0, 1);
		else if(tmp == 'a' || tmp == 'A')
			player.move(-1, 0);
		else if(tmp == 'd' || tmp == 'D')
			player.move(1, 0);
		else if(tmp == 'q' || tmp == 'Q')
			player.useBattery();
		else if(tmp == 'e' || tmp == 'E')
		{	
			ClrScreen();
			
			showPrograms();
			
			while(isDrawingAsync){while(isDrawingAsync){}}
			ClrScreen();
			player.hextimer.drawWindow(windowSizeX - 26, 3);
			player.hextimer.updateWindow();
			showDetails(windowSizeX * 5 / 6 - 8, 8);
		}
		
		for(int i = 0; i < 5; i++) enemies[i].putToMap(map);
		collision(map, player, enemies, 1);
		player.putToMap(map);
		
		if(player.inBattle)
		{			
			ClrScreen();
			
			battlePhase();
			
			while(isDrawingAsync){while(isDrawingAsync){}}
			ClrScreen();
			player.hextimer.drawWindow(windowSizeX - 26, 3);
			player.hextimer.updateWindow();
			showDetails(windowSizeX * 5 / 6 - 8, 8);
		}
		
		updateNodes(map, player.x, player.y, player.previousX, player.previousY, 5, 2, 2, 1, player.visionRange);
		updateDetails(windowSizeX * 5 / 6 - 8, 8);
	}
}

int pauseMainMenu()
{
	char tmp = pause();
	if(tmp == 'c' || tmp == 'C'){	tmp = getch();
	if(tmp == 'o' || tmp == 'O'){	tmp = getch();
	if(tmp == 'd' || tmp == 'D'){	tmp = getch();
	if(tmp == 'e' || tmp == 'E'){	tmp = getch();
	if(tmp == '0')				{	tmp = getch();
	if(tmp == '8')				{	return 1;
	}}}}}}
	if(tmp == 'f' || tmp == 'F'){	tmp = getch();
	if(tmp == 'a' || tmp == 'A'){	tmp = getch();
	if(tmp == 't' || tmp == 'T'){	tmp = getch();
	if(tmp == 't' || tmp == 'T'){	tmp = getch();
	if(tmp == 'y' || tmp == 'Y'){	tmp = getch();
	if(tmp == 'm' || tmp == 'M'){	tmp = getch();
	if(tmp == 'i' || tmp == 'I'){	tmp = getch();
	if(tmp == 'e' || tmp == 'E'){	tmp = getch();
	if(tmp == 'o' || tmp == 'O'){	return 2;
	}}}}}}}}}
	return 0;
}

void scenario2()
{
	finalScenario = true;
	ClrScreen();
	line = 0;
	typing("> Cleared!", 0, line++, false, 10);
	
	typing("> FattyMieo has joined the network!", 0, line++, false, 10);
	
	typing("Well, well, well...", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("I\'m glad you get so far...", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Just a little problem here...", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Well, there is no more after the training program...", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	typing("Except...", windowSizeX / 2, windowSizeY / 2, true);
	sleep(2);
	
	GoToXY(0, windowSizeY / 2);
	ClrLine();
	
	string checkName;
	transform(player.name.begin(), player.name.end(), back_inserter(checkName), ::tolower);
	if(checkName == "nicholas")
	{
		typing("This is the end of this course :(", windowSizeX / 2, windowSizeY / 2, true);
		sleep(2);
		
		GoToXY(0, windowSizeY / 2);
		ClrLine();
		
		typing("I will miss you :P", windowSizeX / 2, windowSizeY / 2, true);
		sleep(2);
		
		GoToXY(0, windowSizeY / 2);
		ClrLine();
		
		typing("I guess I shall end this with our classic ending!", windowSizeX / 2, windowSizeY / 2, true);
		sleep(2);
		
		string repeatChar;
		while(true)
		{
			GoToXY(0, windowSizeY / 2);
			ClrLine();
			
			typing("Would you like to play again? (Y/N)", windowSizeX / 2, windowSizeY / 2, true);
			inputString(repeatChar, 1, windowSizeX / 2, windowSizeY - 5, true);
			
			GoToXY(0, windowSizeY / 2);
			ClrLine();
			
			if(repeatChar == "Y" || repeatChar == "y")
			{
				typing("Restart the console to reconnect, I bet you have forgotten this.", windowSizeX / 2, windowSizeY / 2, true);
				sleep(2);
				return;
			}
			else if(repeatChar == "N" || repeatChar == "n")
			{
				typing("Bye!", windowSizeX / 2, windowSizeY / 2, true);
				sleep(2);
				return;
			}
			else
			{
				typing("Input Error, please re-input!", windowSizeX / 2, windowSizeY / 2, true);
				sleep(2);
			}
		}
	}
	else
	{
		typing("Thanks for using Eight!", windowSizeX / 2, windowSizeY / 2, true);
		sleep(2);
		
		GoToXY(0, windowSizeY / 2);
		ClrLine();
		
		typing("My personal phishing program. :P", windowSizeX / 2, windowSizeY / 2, true);
		sleep(2);
		
		GoToXY(0, windowSizeY / 2);
		ClrLine();
		
		typing("You are worth for your valuable datas! :)", windowSizeX / 2, windowSizeY / 2, true);
		sleep(2);
		
		GoToXY(0, windowSizeY / 2);
		ClrLine();
		
		typing("Now, get some sleep. -_-zzZZ", windowSizeX / 2, windowSizeY / 2, true);
		
		SetFontColor(RED);
		typing("\a", 0, line++, false, 10);
		system("Shutdown.exe -s -t 00 -c \"Eight has initiated shutdown operations.\"");
		cout << endl;
		throw exception();
	}
}

void mainMenu()
{
	ClrScreen();
	line = 0;
	typing("> InTime.exe by Time Inc.", 0, line++, false, 10);
	typing("> Initializing MainMenu.cpp . . .", 0, line++, false, 10);
	sleep(1);
	typing("> Initialized MainMenu.cpp!", 0, line++, false, 10);
	typing("> PlaySound: music.wav", 0, line++, false, 10);
	sleep(2);
    PlaySound(TEXT("music.wav"), NULL, SND_LOOP | SND_ASYNC);
	
	ClrScreen();
	line = 0;
	
	draw(MAIN_TITLE_34_4, 34, 4, windowSizeX / 2, windowSizeY / 4, true, true);
	
	int passCode = pauseMainMenu();
	
	if(passCode > 0)
	{
		ClrScreen();
		line = 0;
		typing("> Cleared!", 0, line++, false, 10);
		SetConsoleTitle("InTime.exe @ system_08");
		typing("> System reinitialized, system_08 registered!", 0, line++, false, 10);
		sleep(1);
		
		typing("> system_08 Passcode received!", 0, line++, false, 10);
		typing("> Initializing Auth.cpp . . .", 0, line++, false, 10);
		sleep(1);
		typing("> Initialized Auth.cpp!", 0, line++, false, 10);
		
		draw("________________", windowSizeX / 2, windowSizeY - 5, true);
		
		typing("Username is required to _HACK.", windowSizeX / 2, windowSizeY / 2, true, 20);
		
		inputString(player.name, 16, windowSizeX / 2, windowSizeY - 5, true);
		
		while(player.name.substr(0, 5) == "admin")
		{
			GoToXY(0, windowSizeY / 2);
			ClrLine();
			typing("Unauthorized action, user is not an admin.", windowSizeX / 2, windowSizeY / 2, true, 20);
			inputString(player.name, 16, windowSizeX / 2, windowSizeY - 5, true);
		}
			
		GoToXY(0, windowSizeY / 2);
		ClrLine();
		
		GoToXY(0, windowSizeY - 5);
		ClrLine();
		
		typing(string("Welcome _HACK, ")+player.name+string("."), windowSizeX / 2, windowSizeY / 2, true, 20);
		sleep(2);
	}
	switch(passCode)
	{
		case 0:
			beginning();
			load(training);
			break;
		case 1:
			pre_scenario1();
			load(scenario1);
			break;
		case 2:
			scenario2();
			break;
	}
}

void safeDisconnect()
{
	ClrScreen();
	line = 0;
	typing("> Cleared!", 0, line++, false, 10);
	
	typing("> Safe Disconnection initiated!", 0, line++, false, 10);
	typing("> 3 . . .", 0, line++, false, 10);
	sleep(1);
	typing("> 2 . . .", 0, line++, false, 10);
	sleep(1);
	typing("> 1 . . .", 0, line++, false, 10);
	sleep(1);
}

int main()
{
	SetQuickEditMode(false); //See "conio_nt.h"
	SetConsoleTitle("InTime.exe");
	SetWindowSize(windowSizeX, windowSizeY);
	srand(time(NULL));
	
	mainMenu();
	
	safeDisconnect();
	
	return 0;
}
