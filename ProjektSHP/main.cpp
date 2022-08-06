#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "PolyLineZ.h" //dolaczenie klasy
#include <SDL.h> //dolaczenie biblioteki SDL do rysowania


using namespace std;


int main(int argc, char* args[])
{
    //zainicjowanie zmiennych potrzebnych do otworzenia pliku 
    int32_t filecodes, fileLengths, shapeTypes, versions;
    string shape;
    string filePath = "test.shp";
    char* fileBuf;			
    FILE* file = NULL;
    //inicjacja obiektu klasy, ktory bedzie przechowywal dane potrzebne do rysowania jak i wyswietlenia informacji o pliku.shp
    PolyLineZ poly;
    
    //otwarcie pliku w odpowiednim formacie
    if ((file = fopen("test.shp", "rb")) == NULL)
        cout << "Nie mozna otworzyc pliku" << endl;
    else
        cout << "Plik zaladowany pomyslnie" << endl;
    // Przypisanie rozmiaru pliku
    poly.fileSize = PolyLineZ::sizeOfFiles(file);

    //przypisanie miejsca na buffer
    fileBuf = new char[poly.fileSize];

    // Odczyt pliku i jednoczesne zaladowanie go do buffera
    fread(fileBuf, poly.fileSize, 1, file);

    //przypisanie wartosci do pozniejszego wyswietlenia
    //wartosci sa pobierane z odpowiednich miejsc zgodnie z dokumentacja

    cout << "File size = " << poly.fileSize;

    poly.filecodes = PolyLineZ::fileCode(fileBuf, 0);
    poly.fileLengths = PolyLineZ::fileLength(fileBuf, 24);
    poly.versions = PolyLineZ::version(fileBuf, 28);
    poly.shapeTypes = PolyLineZ::shapeType(fileBuf, 32);
    poly.xmin = PolyLineZ::littleEndianDoubleRead(fileBuf, 36);
    poly.ymin = PolyLineZ::littleEndianDoubleRead(fileBuf, 44);
    poly.xmax = PolyLineZ::littleEndianDoubleRead(fileBuf, 52);
    poly.ymax = PolyLineZ::littleEndianDoubleRead(fileBuf, 60);
    poly.zmin = PolyLineZ::littleEndianDoubleRead(fileBuf, 68);
    poly.zmax = PolyLineZ::littleEndianDoubleRead(fileBuf, 76);


    cout << endl << "/*****************INFORMACJE O PLIKU*********************/";

    cout << endl << "File code = " << poly.filecodes << endl;
    cout << "File Length = " << poly.fileLengths << endl;
    cout << "Version = " << poly.versions << endl;
    cout << "Shape Type = " << poly.shapeTypes << endl;


    cout << "X minimum = " << poly.xmin << endl;
    cout << "Y minimum = " << poly.ymin << endl;
    cout << "X maximum = " << poly.xmax << endl;
    cout << "Y maximum = " << poly.ymax << endl;
    cout << "Z minimum = " << poly.zmin << endl;
    cout << "Z maximum = " << poly.zmax << endl;
    
    //inicjowanie biblioteki SDL do wyrysowania PolyLineZ

    SDL_Init(SDL_INIT_VIDEO);
    //ustawienie parametrow ekranu rysowania
    SDL_Window* window = SDL_CreateWindow("ShapeLine",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_bool done = SDL_FALSE;
    SDL_Event event;
    
    //zmienne wykorzystywane do rysowania, przesuwania i zooming'u
    int x1, y1, x2, y2, Xmoved, Ymoved, xmin1, ymin1, xmin2, ymin2;
    bool drawing = false;
    xmin1 = poly.xmin;
    ymin1 = poly.ymin;
    xmin2 = poly.xmax;
    ymin2 = poly.ymax;
    float zoomIn = 1;
    float zoomOut = 1;

    Xmoved = 0;
    Ymoved = 0;
    int startX, startY, koniecX, koniecY; //te zmienne przechowuj¹ informacjê o trasie myszki od klikniecia lewego przycisku

    while (!done)
    {
        
        SDL_Delay(10); //delikatne opoznienie akcji na ekranie rysowania, moze wplyn¹æ na p³ynnoœæ przesuwania i skalowania
        SDL_PollEvent(&event);
  
        // czyszczenie ekranu 

        SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
        SDL_RenderClear(renderer);

        //ustawienie koloru rysowanych lini, jesli kolor nie bylby ustawiony przed czyszczeniem, caly ekran bylby na kolor ustawiony pozniej
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        switch (event.type) //switch w petli nieskonczonej while na biezaco sprawdza jaka czynnosc wykonuje uzytkownik
        {
        case SDL_QUIT: //wyjscie z ekranu rysowania
            done = SDL_TRUE;
            break;
            
        case SDL_MOUSEBUTTONDOWN: //wcisniecie przycisku myszy
            switch (event.button.button) //sprawdzenie ktory przycisk zostal wcisniety
            {
            case SDL_BUTTON_LEFT: //wcisniecie LPM
                startX = event.button.x;
                startY = event.button.y;               
                drawing = true;
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP: //zwolnienie przycisku
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                drawing = false;
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            if (drawing)
            {
                koniecX = event.motion.x;
                koniecY = event.motion.y;
                Xmoved = koniecX - startX;
                Ymoved = koniecY - startY;
            }
            break;
        case SDL_MOUSEWHEEL:
            
            if (event.wheel.y > 0) //scrollup
            {
                zoomIn += 0.01;
            }
            if (event.wheel.preciseY < 0) //scrolldown
            {
                if(zoomOut==0.05)
                {
                    zoomOut = 0.06; //zabezpieczenie przed zbytnim pomniejszeniem ksztaltu
                }
                zoomOut -= 0.01;
            }
            break;
        }

        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }
            for (int i = 156;i < poly.fileSize;i += 152) //petla zaczyna sie od pojawienia sie wartosci X1, kazdy nastepny X jest co 152 bajty
            {
                //petla do rysowania
                //wartosci Y1, X2, Y2 sa oddalone od siebie o 8 bajtow
                //pobieranie wartosci z pliku

                poly.xmin = PolyLineZ::littleEndianDoubleRead(fileBuf, i);
                poly.ymin = PolyLineZ::littleEndianDoubleRead(fileBuf, i + 8);
                poly.xmax = PolyLineZ::littleEndianDoubleRead(fileBuf, i + 16);
                poly.ymax = PolyLineZ::littleEndianDoubleRead(fileBuf, i + 24);

                //pobrane wartosci nalezalo poddac skalowaniu, gdyz tak duze wartosci nie miescily sie na ekranie
                //trzeba bylo rowniez dopasowac mnozenie i odejmowanie tak, aby calosc wyswietlala sie mniej wiecej na srodku ekranu rysowania
                //w trakcie dalszych prac doda³em rowniez elementy zoomingu, ktore powiekszaja lub pomniejszaja caly obraz
                
                x1 = ((((int)poly.xmin % 1000) * 15) - 8500) * zoomIn * zoomOut;
                y1 = ((((int)poly.ymin % 1000) * 15) - 12840) * zoomIn * zoomOut;
                x2 = ((((int)poly.xmax % 1000) * 15) - 8500) * zoomIn * zoomOut;
                y2 = ((((int)poly.ymax % 1000) * 15) - 12840) * zoomIn * zoomOut;

                //rysowanie lini, Y sa obrocone, do poprawnego wyswietlania trzeba je odjac od wysokosci ekranu ustalonej wczesniej
                //do kazdej wartosci dodana jest rowniez roznica przesuniecia tj. o ile przesunela sie cala figura wzgledem poprzedniej pozycji
                
                SDL_RenderDrawLine(renderer, x1 + Xmoved, 600 - y1 + Ymoved, x2 + Xmoved, 600 - y2 + Ymoved);
            }
            //wyswietlenie wszystkich narysowanych lini
            SDL_RenderPresent(renderer);
    }

    // koniec pracy z SDL

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    //koniec pracy z plikiem
    delete[]fileBuf;
    fclose(file);
    return 0;
}
