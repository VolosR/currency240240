#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <vector> 
#include <HTTPClient.h>
#include <ArduinoJson.h>   //https://github.com/bblanchon/ArduinoJson.git
#include "flags.h" 
#include "flags2.h" 


TFT_eSPI tft = TFT_eSPI();

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 5;

#define GREY 0x5AEB
#define TFT_ORANGE 0xFC64
#define dark 0x2124
#define darkTFT_ORANGE 0x2ACA
#define red 0xFC7F
#define lightBlue 0x8FFE
#define light 0xCF3E

int posX2[4];
int n2=4;
int boxSize2=60;
int space2=3;
int start2=25;
String buttons2[3][4]={{"7","4","1","0"},{"8","5","2","."},{"9","6","3","ok"}};
int cF[3][4]={{0,1,2,3},{4,5,6,7},{8,9,10}};

const char* ssid     = "xxxxxxxxxxxxxx";       ///EDIIIT
const char* password = "xxxxxxxxxxxxx";
float amount=1.00000;
bool calcStarted=0;
bool dataStarted=0;
bool curStarted=0;
String math2="";

int chosen=2;
String payload="";
const String endpoint2 ="https://cdn.jsdelivr.net/gh/fawazahmed0/currency-api@1/latest/currencies/hrk.min.json";
String cur[11]={"eur","hrk","usd","gbp","aud","cad","chf","jpy","cny","nzd","rub"};
String big[22]={"EUR","HRK","USD","GBP","AUD","CAD","CHF","JPY","CNY","NZD","RUB"};
String curNames[11]={"Euro","Croatian kuna","US dollar","Pound sterling","Australian dollar","Canadian dollar","Swiss franc","Japanese yen","Chinese renminbi","New Zealand dollar","Russian ruble"};
StaticJsonDocument<6000> doc;

int fase=0;   //0=display data, //1=set amount //2=setCurency

void setup()
{
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(17, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 100);

for(int i=0;i<n2;i++)
   posX2[i]=(start2+(i*boxSize2)+(space2*i));

WiFi.begin(ssid, password);
tft.print("connecting");

while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    tft.print(".");
   }
tft.print("CONECTED!!");
delay(1000);
tft.fillScreen(TFT_BLACK);
getData(); 

}
int pres=1;
void loop(void)
{
getData();
delay(32000);
  





}

void getData()
{
   tft.setTextColor(TFT_WHITE,TFT_BLACK);
   tft.fillScreen(TFT_BLACK);
   tft.pushImage(0,2,48,48,flags2[chosen]);
   tft.drawString("Currency",54,6,2);
   tft.fillCircle(230,14,3,TFT_ORANGE);
   tft.fillCircle(220,14,3,TFT_ORANGE);
   tft.fillCircle(210,14,3,TFT_ORANGE);
   
    
   tft.drawString(big[chosen],52,24,4);
   tft.drawString("Amount:",118,30,2);
   tft.drawFloat(amount,5,180,30,2);
   
   if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
   HTTPClient http;
   String    endpoint="https://cdn.jsdelivr.net/gh/fawazahmed0/currency-api@1/latest/currencies/"+cur[chosen]+".min.json";
   http.begin(endpoint); //Specify the URL
   int httpCode = http.GET();  //Make the request
   if (httpCode > 0) { //Check for the returning code
   payload = http.getString();
   char inp[payload.length()];
   payload.toCharArray(inp,payload.length());
   deserializeJson(doc,inp);

   String date2=doc["date"];
   tft.setTextColor(TFT_ORANGE,TFT_BLACK);
   tft.drawString(date2,118,24,1);
   tft.setTextColor(TFT_WHITE,TFT_BLACK);
   
   int j=0;
   for(int i=0;i<11;i++)
   {
    if(i!=chosen){
   String res=doc[cur[chosen]][cur[i]];
   float k=res.toFloat();
   float p=k*amount;
   tft.pushImage(0,58+j*26,24,24,flags[i]);
   tft.drawLine(32,80+j*26,240,80+j*26,GREY);
   tft.drawString(curNames[i],32,62+j*26,2);
    tft.setTextColor(lightBlue,TFT_BLACK);
   tft.drawFloat(p,5,180,62+j*26,2);
   tft.setTextColor(TFT_WHITE,TFT_BLACK);
  
   j++;
    }
   }
   }}

          fase=0;
          amount=1.00000;
          math2="";
          curStarted=0;
          dataStarted=0;
          calcStarted=0;
}   

  
