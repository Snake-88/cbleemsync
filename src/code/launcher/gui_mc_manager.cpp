#include "gui_mc_manager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../gui/gui.h"
#include "../lang.h"

void GuiMcManager::loadAssets() {
    string fileToLoad = sonyStuffPath+"/MC/Dot_Matrix.png";
    mcGrid = IMG_LoadTexture(renderer, fileToLoad.c_str());
    fileToLoad = sonyStuffPath+"/MC/Pencil_Carsor.png";
    mcPencil = IMG_LoadTexture(renderer,fileToLoad.c_str());
    pencilPos.w=42;
    pencilPos.h=42;
    pencilPos.x=0;
    pencilPos.y=0;
}

void GuiMcManager::loadMemoryCard() {
    memcard1.loadFile(memcardFolder+"memcards/card1.mcd");
    memcard2.loadFile(memcardFolder+"memcards/card2.mcd");
}
void GuiMcManager::render() {
    shared_ptr<Gui> gui(Gui::getInstance());
    const int xStartMC1 = 80;
    const int xStartMC2 = 940;
    const int yStart = 80;
    const int xDecal = 10;
    const int yDecal = 10;
    const int xShift = 80;
    const int yShift = 80;
    int line = 0;
    int col = 0;
    int w = 256;
    int h = 420;
    SDL_Rect input, output;
    gui->renderBackground();
    gui->renderTextBar();
    gui->renderTextLine("Memory Card Manager",1,1,true);
    gui->renderStatus("|@O| " + _("Go back") + "|");
    //Draw Memcard images
    input.h=16;
    input.w=16;
    output.h=75;
    output.w=75;

    //Fill memory card 1
    for(int i = 0; i < 15; i++){
        output.x = xStartMC1 + (xShift*col)+xDecal;
        output.y = yStart + (yShift*line)+yDecal;
        if(memcard1.entries[i].getGameName() != ""){
            SDL_RenderCopy(renderer,memcard1.entries[i].getGameIcon(renderer),&input,&output);
            //printf("%s\n",memcard1.entries[i].getGameName().c_str());
        }
        col++;
        if(col == 3){
            col = 0;
            line++;
        }
    }

    //Fill memory card 2
    line = 0;
    col = 0;
    for(int i = 0; i < 15; i++){
        output.x = xStartMC2 + (xShift*col)+xDecal;
        output.y = yStart + (yShift*line)+yDecal;
        if(memcard2.entries[i].getGameName() != ""){
            SDL_RenderCopy(renderer,memcard2.entries[i].getGameIcon(renderer),&input,&output);
        }
        col++;
        if(col == 3){
            col = 0;
            line++;
        }
    }

    //Draw dot matrix image
    input.x = 0, input.y = 0;
    input.h = 420, input.w = 256;
    output.x = 80;
    output.y = 80;
    output.w = w;
    output.h = h;
    SDL_RenderCopy(renderer,mcGrid,&input, &output);
    output.x = 940;
    output.y = 80;
    SDL_RenderCopy(renderer,mcGrid,&input, &output);

    //Draw the pencil
    input.h = 42;
    input.w = 42;
    input.x = 0;
    input.y = 0;
    SDL_RenderCopy(renderer,mcPencil,&input,&pencilPos);

    SDL_RenderPresent(renderer);
}

void GuiMcManager::loop() {
    shared_ptr<Gui> gui(Gui::getInstance());
    bool menuVisible = true;
    while (menuVisible) {
        render();
        gui->watchJoystickPort();
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            // this is for pc Only
            if (e.type == SDL_QUIT) {
                menuVisible = false;
            }
            switch (e.type) {
                case SDL_JOYBUTTONUP:
                    if (e.jbutton.button == PCS_BTN_CIRCLE) {
                        Mix_PlayChannel(-1, gui->cancel, 0);
                        menuVisible = false;
                    };
                case SDL_JOYAXISMOTION:
                    if(e.jaxis.axis == 0){
                        if(e.jaxis.value < -3200){
                            pencilPos.x-=10;
                        }
                        if(e.jaxis.value > 3200){
                            pencilPos.x+=10;
                        }
                    }
                    if(e.jaxis.axis == 1) {
                        if (e.jaxis.value < -3200) {
                            pencilPos.y -= 10;
                        }
                        if (e.jaxis.value > 3200) {
                            pencilPos.y += 10;
                        }
                    }
            }

        }

    }

    //Draw dot matrix
}