#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <U8g2lib.h>
//test
//初始化和配置I2C通信
#include <Wire.h>

#include <DHT.h>
#include <Adafruit_Sensor.h> // 确保包含这个库

#define DHTPIN 4      // DHT11 数据引脚连接到 GPIO 4
#define DHTTYPE DHT11 // DHT 11

const char* ssid = "CMCC-5D7J";
const char* password = "wu1209932698";
const char* mqtt_server = "test.mosquitto.org";

DHT dht(DHTPIN, DHTTYPE);

//wifi模块和MQTT
WiFiClient espClient;
PubSubClient client(espClient);
//OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* SCL=*/ 22, /* SDA=*/ 21, /* reset=*/ U8X8_PIN_NONE);

//wifi设置
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    
    String messageTemp;
    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];
    }
    Serial.println(messageTemp);

    // 在OLED上显示接收到的消息
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr); // 设置字体
    u8g2.drawStr(0, 10, "Message:");
    u8g2.drawStr(0, 30, messageTemp.c_str());
    u8g2.sendBuffer();
}


void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        //测试
        Serial.print(clientId);
        
        // Attempt to connect without username and password
        if (client.connect(clientId.c_str())) {
            Serial.println("--------");
            Serial.print(client.state());
            Serial.println("--------");
            // Subscribe to a topic
            Serial.println("connected");
            // Subscribe to a topic
            client.subscribe("go");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(500);
        }
    }
}
//test
void test()
{
    byte error, address;
  int nDevices;
  
  Serial.println("Scanning...");
  nDevices = 0;
  
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) 
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) 
        Serial.print("0");
      Serial.println(address, HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  
  delay(5000); // wait 5 seconds for next scan
}
void setup() {
    Serial.begin(9600);
    //test

    // 初始化 I2C，总线上的 SDA 和 SCL 分别连接到 21 和 22 引脚
    Wire.begin(21, 22);  // SDA, SCL
    //while (!Serial); // Wait for Serial to be ready
    //Serial.println("\nI2C Scanner");
    Serial.println("DHT11 测试程序启动");
    dht.begin();
    Serial.println("DHT11 初始化完成");


    setup_wifi();
    client.setServer(mqtt_server, 1883);
    
    client.setCallback(callback);

    // 初始化OLED显示屏
    u8g2.begin();
    // 清除显示缓冲区
    u8g2.clearBuffer();
    // 设置字体
    u8g2.setFont(u8g2_font_ncenB08_tr);
    // 在显示屏上绘制字符串
    u8g2.drawStr(3, 10, "Hello, OLED!");
    // 发送缓冲区内容到显示屏
    u8g2.sendBuffer();

}

void resetDHT11() {
    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(20); // 拉低至少 18 ms
    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(30); // 拉高 20-40 微秒
    pinMode(DHTPIN, INPUT);
}

void readT()
{
    resetDHT11(); // 每次读取前复位传感器
     Serial.println("复位！");
    delay(2000);
  // 读取温度和湿度
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.println(h);

    // 检查是否读取失败
    if (isnan(h) || isnan(t)) {
        Serial.println("读取 DHT11 失败！");
        return;
    }

    // 打印温度和湿度
    Serial.print("湿度: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("温度: ");
    Serial.print(t);
    Serial.println(" *C");
}

void loop() {
    readT();
    //MQTT重新连接机制
    if (!client.connected()) {
        reconnect();
        // 清除显示缓冲区
      u8g2.clearBuffer();
      // 设置字体
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(1,10, "MQTT ERROR!");
    }
    client.loop();
    if (client.connected()) {
    String message = "Hello, MQTT";
    delay(5000);
    if (client.publish("go", message.c_str())) {
        Serial.println("Message sent successfully");
        // 在OLED上显示发送的消息
        //test
            u8g2.clearBuffer(); // 清除屏幕内容
            u8g2.setFont(u8g2_font_ncenB08_tr);
            u8g2.drawStr(3, 10, "Hello, OLED!");
            u8g2.drawStr(3, 30, "Hello, OLED!");
            u8g2.sendBuffer(); // 发送更新到屏幕
            delay(5000);
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_ncenB08_tr);
            u8g2.drawStr(2, 10, "Sent:");
            u8g2.drawStr(2, 30, message.c_str());
            u8g2.sendBuffer();
    } else {
        Serial.println("Message send failed");
    }
}
}
