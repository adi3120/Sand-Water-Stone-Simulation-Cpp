#define s_width 100
#define s_height 80

#include"Fazen.hpp"
#include<vector>

using namespace std;

#define AIR 0
#define ROCK 1
#define SAND 2
#define WATER 3

int rows = 50;
int cols = 50;
float w = s_width/cols;
float h = s_height/rows;

vector<vector<int>> world;
vector<vector<bool>> hasMovedFlags;


bool allowWater=false;
bool allowSand=false;
bool allowRock=false;

void clearWorld(){
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			world[i][j]=AIR;
			hasMovedFlags[i][j]=false;
		}
	}
}

void draw(Fazen2d game,short c=0x2588){

	if(GetAsyncKeyState(0x57)){
		allowWater=!allowWater;
		Sleep(100);
	}
	else if(GetAsyncKeyState(0x53)){
		allowSand=!allowSand;
		Sleep(100);
	}
	else if(GetAsyncKeyState(0x52)){
		allowRock=!allowRock;
		Sleep(100);
	}
	else if(GetAsyncKeyState(VK_RETURN)){
		clearWorld();
		Sleep(200);
	}

	float mx=game.Mouse_X();
	float my=game.Mouse_Y();

	if(mx<rows && my<cols && allowWater && world[my][mx]==AIR){
		world[my][mx]=WATER;
	}
	else if(mx<rows && my<cols && allowSand && world[my][mx]==AIR){
		world[my][mx]=SAND;
	}
	else if(mx<rows && my<cols && allowRock && world[my][mx]==AIR){
		world[my][mx]=ROCK;
	}
	
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			float border=0.0;
			if(world[j][i]==SAND){
				game.Fill(i*w+border,j*h+border,(i+1)*w-border,(j+1)*h-border,'*',yellowF);
			}
			else if(world[j][i]==AIR){
				game.Fill(i*w+border,j*h+border,(i+1)*w-border,(j+1)*h-border,' ',0);
			}
			else if(world[j][i]==ROCK){
				game.Fill(i*w+border,j*h+border,(i+1)*w-border,(j+1)*h-border,'#',whiteF);
			}
			else if(world[j][i]==WATER){
				game.Fill(i*w+border,j*h+border,(i+1)*w-border,(j+1)*h-border,'o',blueF);
			}
		}
	}
}


bool tileIsFree(int x,int y){
	if(x<0 || x>=rows || y<0 || y>=cols){
		return false;
	}
	return world[y][x]==AIR;
}
void move(int fromX,int fromY,int toX,int toY){
	world[toY][toX]=world[fromY][fromX];
	world[fromY][fromX]=AIR;
	hasMovedFlags[fromY][fromX]=true;
	hasMovedFlags[toY][toX]=true;
}
bool tileisWater(int x,int y){
	if(x<0 || x>=rows || y<0 || y>=cols){
		return false;
	}
	return world[y][x]==WATER;
}

void swap_sand_water(int sandx,int sandy,int waterx,int watery){
	swap(world[sandy][sandx],world[watery][waterx]);
}
void update(Fazen2d game){
	for(int y=0;y<rows;y++){
		for(int x=0;x<cols;x++){
			hasMovedFlags[y][x]=false;
		}
	}
	for(int y=rows-1;y>=0;y--){
		for(int x=0;x<cols;x++){
			if(hasMovedFlags[y][x]) continue;
			if(world[y][x]==AIR || world[y][x]==ROCK) continue;
			if(world[y][x]==SAND){
	
				if(tileIsFree(x,y+1)){
					move(x,y,x,y+1);
				}
				else if(tileisWater(x,y+1)){
					swap_sand_water(x,y,x,y+1);
				}
				bool checkLeftFirst=rand()%2;
				if(checkLeftFirst){

					if(tileIsFree(x-1,y+1)){
						move(x,y,x-1,y+1);
					}
					else if(tileIsFree(x+1,y+1)){
						move(x,y,x+1,y+1);
					}
				}
				else{

					if(tileIsFree(x+1,y+1)){
						move(x,y,x+1,y+1);
					}
					else if(tileIsFree(x-1,y+1)){
						move(x,y,x-1,y+1);
					}
				}
			}
			else if(world[y][x]==WATER){
				if(tileIsFree(x,y+1)){
					move(x,y,x,y+1);
				}
				bool checkLeftFirst=rand()%2;
				if(checkLeftFirst){
					if(tileIsFree(x-1,y)){
						if(tileIsFree(x-1,y+1)){
							move(x,y,x-1,y+1);
						}
						else{
							move(x,y,x-1,y);
						}
					}
				}
				else{
					if(tileIsFree(x+1,y)){
						if(tileIsFree(x+1,y+1)){
							move(x,y,x+1,y+1);
						}
						else{
							move(x,y,x+1,y);
						}
					}
					
				}
				
			}

		}
		draw(game);
		// Sleep(10);
		game.display();
	}
}
int main(){

	Fazen2d game;
	game.make2DConsole();
	srand(time(nullptr));

	world.resize(cols);
	hasMovedFlags.resize(cols);

	for (int i = 0; i < cols; i++){
		world[i].resize(rows);
		hasMovedFlags[i].resize(rows);
		for (int j = 0; j < rows; j++){
			world[i][j]=AIR;
			hasMovedFlags[i][j]=false;
		}
	}

	for(int y=cols-5;y<cols;y++){
		for(int x=0;x<rows;x++){
			world[y][x]=ROCK;
		}
	}
	

	while(1){
		game.background(0);
		update(game);
		CHECK_USER_EXIT
		game.display();
	}



}
