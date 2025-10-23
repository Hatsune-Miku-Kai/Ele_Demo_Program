#ifndef _COORDS_H_
#define _COORDS_H_


#include "Screen_Base.h"
#include "UI_Manager.h"


//关联界面
#include "Program.h"
#include "Home.h"
#include "IO.h"

#define COORDS_DATA_START 12 // 坐标数据起始
#define XYZ_DATA_LEN 3
#define RXRYRZ_DATA_LEN 3
#define COORDS_DATA_LEN (XYZ_DATA_LEN + RXRYRZ_DATA_LEN)

class Coords : public Screen_Base
{
public:
    Coords(TFT_eSPI& tft, Button& button);
    ~Coords();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

    struct AngleSprite {
        TFT_eSprite* sprite;
        float value;
        int x;
        int y;
    };


private:
    void Draw_UI();
    void Update_UI();
    void UpdateCoords(float x, float y, float z, float rx, float ry, float rz);
    void Create_Coord_Sprite();
    void Delete_Coord_Sprite();

    void Handle_Data(std::vector<uint8_t>& data);//处理本页面所需数据

    TFT_eSprite Xsprite = TFT_eSprite(&tft);
    TFT_eSprite Ysprite = TFT_eSprite(&tft);
    TFT_eSprite Zsprite = TFT_eSprite(&tft);
    TFT_eSprite Rxsprite = TFT_eSprite(&tft);
    TFT_eSprite Rysprite = TFT_eSprite(&tft);
    TFT_eSprite Rzsprite = TFT_eSprite(&tft);
    TFT_eSprite IP_Coords_sprite = TFT_eSprite(&tft);

    float Coords_Data[COORDS_DATA_LEN] = {0.0};

    std::map<std::string, std::pair<int, int>> Coord_UI = {
        // 左列Coord
        {"X", {30,75}},//{ "X",  {10, 75} },
        {"Y", {30,110}},//{ "Y",  {10, 100} },
        {"Z", {30,145}},//{ "Z",  {10, 125} },

        // 右列Coord
        {"Rx", {180, 75}},//{ "Rx", {150, 75} },
        {"Ry", {180, 110}},//{ "Ry", {150, 100} },
        {"Rz", {180, 145}},//{ "Rz", {150, 125} }
    };

    std::map<std::string, std::pair<int, int>> Coord_Signal = {
        // 左列
        {"X", {110, 75}},//{ "X",  {90, 75} },
        {"Y", {110, 110}},//{ "Y",  {90, 100} },
        {"Z", {110, 145}},//{ "Z",  {90, 125} },

        // 右列
        {"Rx", {270, 65}},//{ "Rx", {240, 65} },
        {"Ry", {270, 100}},//{ "Ry", {240, 90} },
        {"Rz", {270, 135}},//{ "Rz", {240, 115} }
    };

    //ip
    std::string IP_Coords_UI;
};



#endif
