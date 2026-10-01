#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

//for platforms the prefer to be in charge of the main function, runs normally otherwise

#include<iostream>
#define _USE_MATH_DEFINES 
#include <cmath>
#include <vector> //probably better than arrays

SDL_Window * window;
SDL_Renderer* renderer;

float windowWidth = 200;
float windowHeight = 200;
//xGrid,Ygrid,0(dead)or 1(alive)
int xgrid;
int ygrid;
int life;



SDL_Point point;

struct Cell
{
    int xGrid;
    int yGrid;
    bool alive;
};

std::vector<Cell> lifeInfo;
std::vector<Cell> exaustiveList;
std::vector<SDL_Point> dyingLives; 
std::vector<Cell> births;




//later used
void drawLife(SDL_Point point);
void lifeDeath();
void redrawLife();
void lifeBirth();
void applyDeath();
void applyBirth();
void lifeContinue();
class life{
    public:
        std::vector<int> location;

        int neighbourCountCheck(std::vector<int> location){
            int neighbor = 0;
            return neighbor;
        }
        void reproduce(std::vector<int> location){

        }
};


//mod 11 get 10x10 boxes
void drawGrid(int windowWidth,int windowHeight){
    exaustiveList.clear();
    for(int x = 0; x<windowWidth/10;x++){
        for(int y = 0; y < windowHeight/10;y++){
        exaustiveList.push_back({x,y,false});
        }
    }
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    int widthGrids = std::floor(windowWidth/10);
    float widthTemp = windowWidth;
    int heightGrids = std::floor(windowHeight/10);
    
    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    //horrizontal
    for(int i=0;i < heightGrids; i++){
        SDL_RenderLine(renderer,0,i*10,windowWidth,i*10);
        //std::cout << "horrizontal: y: " << heightGrids << " i: " << i << '\n'; 
    }
    //vertical
    for(int i=0; i < widthGrids ;i++){
        SDL_RenderLine(renderer,i*10,0,i*10,windowHeight);
        //std::cout << "vertical: x: " << widthGrids << " i: " << i << '\n'; 
    }

    for(int j=0;j < heightGrids; j++){
         for(int k=0; k < widthGrids ;k++){
        //specs[j+k].push_back(windowWidth/10 - k, windowHeight/10 - j, 0);
        //exaustiveList[j][k].pushback(i,j,0);
        }
    }
    //SDL_RenderPresent(renderer);
}

void spawnLife(int x,int y){
    SDL_Point point;
    point.x=x;
    point.y=y;

    Cell cell{
        point.x/10,
        point.y/10,
        true
    };
    
    //avoiding repeats
    for(const auto& existing : lifeInfo){
        if(existing.xGrid == cell.xGrid && existing.yGrid == cell.yGrid) return;
    }
    lifeInfo.push_back(cell);
    for (const auto& existing : lifeInfo ){
       std::cout << "xGrid: " << existing.xGrid << " yGrid: " << existing.yGrid << " life check: " << existing.alive  << '\n';
    }
    redrawLife();
    //drawLife(point); 
}
       


void drawLife(SDL_Point point){
    drawGrid(windowWidth,windowHeight);
    SDL_Rect rect = {0,0,10,10};
    SDL_FRect frect;

    rect.x = std::floor(point.x/10);
    //std::cout << "topx grid: " << rect.x;
    rect.x *= 10;
    
    rect.y = std::floor(point.y/10);
    //std::cout<< " topy grid: " << rect.y << '\n';
    rect.y *= 10;
    

    //rect.h += std::floor(point.y); width and height are the width and height of the rectangle
    //std::cout << "topx: " << rect.x << " topy: " << rect.y << " botx: " << rect.w << " bopy: " << rect.h <<'\n';
    
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RectToFRect(&rect, &frect);
    SDL_RenderFillRect(renderer,&frect);
    SDL_RenderPresent(renderer); 
}

void nextGeneration(){
    lifeDeath();
    lifeBirth();
    applyDeath();
    applyBirth();
    redrawLife();
}
/*
Any live cell with fewer than two live neighbours dies, as if by underpopulation.
Any live cell with more than three live neighbours dies, as if by overpopulation.
*/
void lifeDeath(){
    std::vector<SDL_Point> adgesent;
    std::vector<SDL_Point> reproduction;
    SDL_Point tempPoint;
    int livingNeighbours =0;
    
    for(int k=0;k < lifeInfo.size();k++){
        adgesent.clear();
        tempPoint.x = lifeInfo[k].xGrid;
        tempPoint.y = lifeInfo[k].yGrid;
        
            //top left
            adgesent.push_back({tempPoint.x - 1,tempPoint.y - 1});
            //middle top
            adgesent.push_back({tempPoint.x,tempPoint.y - 1});
            //top right
            adgesent.push_back({tempPoint.x + 1,tempPoint.y - 1});
            //middle left
            adgesent.push_back({tempPoint.x - 1,tempPoint.y});
            //middle right
            adgesent.push_back({tempPoint.x + 1,tempPoint.y});
            //bottom left
            adgesent.push_back({tempPoint.x - 1,tempPoint.y + 1});
            //bottom middle
            adgesent.push_back({tempPoint.x ,tempPoint.y + 1});
            //bottom right
            adgesent.push_back({tempPoint.x + 1,tempPoint.y + 1});

        for(int i =0; i < lifeInfo.size(); i++){
            for(int j = 0; j < 8;j++){
                if(lifeInfo[i].xGrid==adgesent[j].x && lifeInfo[i].yGrid == adgesent[j].y){
                    livingNeighbours++;
                }
            }
                      
        }
        //if there is 2< underpop || >3overpop thus kill him  
        if(livingNeighbours < 2 || livingNeighbours > 3) {
            tempPoint.x = lifeInfo[k].xGrid;
            tempPoint.y = lifeInfo[k].yGrid;
            std::cout << "erasing: " << lifeInfo[k].xGrid <<" , " << lifeInfo[k].yGrid << " over/undercrowding \n" ;
            dyingLives.push_back(tempPoint);
        }
        if(livingNeighbours == 2 || livingNeighbours == 3)//lifeContinue(); do nothing?
        std::cout << "adjesent count: " << livingNeighbours << '\n'; 
        livingNeighbours = 0;
    }

    
}

void applyDeath(){
    int inc=0;
    for(auto it = lifeInfo.begin(); it != lifeInfo.end();){
        bool shouldErase = false;
        
        for(const auto& removeList : dyingLives)
        if(it->xGrid == removeList.x && it->yGrid == removeList.y){
            shouldErase = true;
            break;
        }
        if(shouldErase){
            std::cout << "deleting " << it->xGrid << " , " << it->yGrid << '\n';
            it = lifeInfo.erase(it);
        }
        else it++;
    }
    std::cout << "lifeInfo size Death: " << lifeInfo.size() << '\n';
    dyingLives.clear();
    for (const auto& c : lifeInfo)
    {
        std::cout << c.xGrid << "," << c.yGrid << '\n';
    }
}

//Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
void lifeBirth(){
     std::vector<SDL_Point> adgesent;
     births.clear();
     
    for(const auto& list : exaustiveList){
        int aliveNextToDead = 0;
        bool alreadyAlive = false;
        Cell cell{
        list.xGrid,
        list.yGrid,
        true
    };
        adgesent.clear();
        //top left
        adgesent.push_back({list.xGrid - 1,list.yGrid - 1});
        //middle top
        adgesent.push_back({list.xGrid,list.yGrid - 1});
        //top right
        adgesent.push_back({list.xGrid + 1,list.yGrid - 1});
        //middle left
        adgesent.push_back({list.xGrid - 1,list.yGrid});
        //middle right
        adgesent.push_back({list.xGrid + 1,list.yGrid});
        //bottom left
        adgesent.push_back({list.xGrid - 1,list.yGrid + 1});
        //bottom middle
        adgesent.push_back({list.xGrid ,list.yGrid + 1});
        //bottom right
        adgesent.push_back({list.xGrid + 1,list.yGrid + 1});
        for(const auto& restrictedList : lifeInfo){
            //ensuring its not alive
            if(list.xGrid == restrictedList.xGrid && list.yGrid == restrictedList.yGrid) {
                alreadyAlive = true;
                break;
            }        
                for(int j = 0; j < 8;j++){
                    if(restrictedList.xGrid==adgesent[j].x && restrictedList.yGrid == adgesent[j].y){
                        aliveNextToDead++;
                    }
                }
            
        }
        if(alreadyAlive) continue;

        if(aliveNextToDead == 3){
            bool exists = false;

            for (const auto& c : lifeInfo)
            {
                if (c.xGrid == cell.xGrid && c.yGrid == cell.yGrid)
                {
                    exists = true;
                    break;
                }
            }
            for (const auto& c : births){
                if (c.xGrid == cell.xGrid &&
                    c.yGrid == cell.yGrid)
                {
                    exists = true;
                    break;
                }
            }
            if(!exists){
            //set cell as alive
            births.push_back(cell);
            std::cout << "cell: " << cell.xGrid <<" , " << cell.yGrid << "becomes alive through reproduction \n";
            }
        }
    }
    
}

void applyBirth(){
    lifeInfo.insert(lifeInfo.end(), births.begin(), births.end());
    births.clear();
    std::cout << "lifeInfo size Birth: " << lifeInfo.size() << '\n';
}
//Any live cell with two or three live neighbours lives on to the next generation.
void lifeContinue(){

}
void redrawLife(){
    drawGrid(windowWidth,windowHeight);
    for(const auto& existing : lifeInfo){
        SDL_Rect redraw{
            existing.xGrid*10,
            existing.yGrid*10,
            10,
            10
        };
        SDL_FRect redraw2;
        SDL_SetRenderDrawColor(renderer,0,255,0,255);
        SDL_RectToFRect(&redraw, &redraw2);
        SDL_RenderFillRect(renderer,&redraw2);
       
    }
    std::cout << "redrawing \n" << std::flush;
    SDL_RenderPresent(renderer);
}
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    if(!SDL_Init(SDL_INIT_VIDEO)) std::cout << "error init video: " << SDL_GetError() << '\n';

    window = SDL_CreateWindow("Cellular Automata",windowWidth,windowHeight,SDL_WINDOW_RESIZABLE);
    if(!window) std::cout << "error init window: " << SDL_GetError() << '\n';
    renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer) std::cout << "error init renderer: " << SDL_GetError() << '\n';

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    //draw the grid
    drawGrid(windowWidth,windowHeight);
    //wait until they hit a start button before switching to app iterate
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    if(event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;

    if(event->type == SDL_EVENT_WINDOW_RESIZED){
        windowWidth = event->window.data1;
        windowHeight = event->window.data2;
        std::cout << "Width: " << windowWidth << "\nHeight: " << windowHeight << '\n';
        lifeInfo.clear();
        drawGrid(windowWidth,windowHeight);
        SDL_RenderPresent(renderer);
    } 
    //on pointer click, locate mouse coords/10 round down, populate box
    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        std::cout << "x: " << event->button.x << " y: " << event->button.y << '\n' << std::flush;
        
        spawnLife(static_cast<int>(std::round(event->button.x)),static_cast<int>(std::round(event->button.y)));
    }

    if(event->type == SDL_EVENT_KEY_DOWN && event->key.key ==  SDLK_RETURN){
        nextGeneration();
        //SDL_RenderPresent(renderer);
    }
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){
    //SDL_SetRenderDrawColor(renderer,0,0,255,255);
    //nextGeneration();
    SDL_Delay(50);
    return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void *appstate, SDL_AppResult result){
    std::cout << "quiting" << std::flush;
    SDL_DestroyRenderer(renderer);
    renderer = 0;
    SDL_DestroyWindow(window);
    window = 0;
   
    SDL_Quit();
}


//TODO: Implement death removing from the lifeinfo vector.