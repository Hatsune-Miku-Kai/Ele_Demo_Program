#include "Connect.h"
#include <WiFi.h>

static volatile bool bleConnected = false;   // 全局连接标志
WiFiServer server(80);

const char* ssid = "Elephant_P1";
const char* wifi_password = "Elephant";

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        bleConnected = true;
    }
    void onDisconnect(BLEServer* pServer) override {
        bleConnected = false;
        // 断开后继续广播，便于再次连接
        pServer->getAdvertising()->start();
    }
};

Connect::Connect(TFT_eSPI &display) : tft(display)
{

}

Connect::~Connect()
{

}

void Connect::Draw_UI()
{
    Internet_Init();
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("Connection");



    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 80);
    tft.print("WiFi");
    tft.setCursor(10, 120);
    tft.print("USB");
    tft.setCursor(10, 160);
    tft.print("BlueTooth");

}


void Connect::Update_UI()
{
    if(server.available())
    {
        //这里加载WiFi连接图标
    }else
    {
        //这里加载WiFi未连接图标
    }


    if(bleConnected)
    {
        //这里加载BlueTooth连接图标
    }else
    {
        //这里加载BlueTooth未连接图标
    }


    if(Serial)
    {
        //这里加载USB连接图标
    }else
    {
        //这里加载未连接图标
    }

}


void Connect::Internet_Init()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, wifi_password);
    server.begin();

    // --- BLE 低功耗蓝牙 ---
    BLEDevice::init("Elephant_P1");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // 建一个最简单的服务和特征
    BLEService *pService = pServer->createService(BLEUUID((uint16_t)0x180F));
    pService->createCharacteristic(
        BLEUUID((uint16_t)0x2A19),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    pService->start();
    pServer->getAdvertising()->start();
}

void Connect::Internet_Close()
{
    // 关闭 Wi-Fi AP 和 Web 服务器
    WiFi.softAPdisconnect(true);
    server.end();
    WiFi.mode(WIFI_OFF);

    if (pServer) {
        pServer->getAdvertising()->stop();
        BLEDevice::deinit(true); // 关闭 BLE
        pServer = nullptr;
    }
}



