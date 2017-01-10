#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "wi-fi";
//const char* pass  = "senha";
String payload;
HTTPClient client;
String msgAtual;
int passo = 0;

#define BOTtoken ""
#define BOTname "Trendt"
#define BOTusername "trendtbot"

TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;
String idc, idu, idcNew, iduNew, order, email;

void Bot_ExecMessages() {
  for (int i = 1; i < bot.message[0][0].toInt() + 1; i++) {
    idc = bot.message[i][4];
    idu = bot.message[i][1];
    bot.message[i][5] = bot.message[i][5].substring(0, bot.message[i][5].length());
    String msg = bot.message[i][5];
    msg.toLowerCase();
    if (msg == "verificar" && passo == 0) {
      bot.sendChatAction(idc, "typing");
      bot.sendMessage(idc, "Legal, digite o código do pedido, somente os números. Por exemplo: 1234", "");
      idcNew = idc;
      iduNew = idu;
      msg = "a";
      passo = 1;
    }
    if (msg.length() > 3 && passo == 1 && idc == idcNew && idu == iduNew) {
      order = msg;
      bot.sendChatAction(idc, "typing");
      bot.sendMessage(idc, "Agora, para verificar digite seu email.", "");
      msg = "a";
      passo = 2;
    }
    if (msg.length() > 3 && passo == 2 && idc == idcNew && idu == iduNew) {
      email = msg;
      bot.sendChatAction(idc, "typing");
      bot.sendMessage(idc, "Ok, seu email é " + email + " e seu número do pedido é " + order + ", vou verificar se está pago agora, aguarde um instante.", "");
      if (paid(order, email) == true) {
        bot.sendChatAction(idc, "typing");
        client.begin("http://token.herokuapp.com/get_order_product.php?order=" + order + "&email=" + email);
        int httpCode = client.GET();
        if (httpCode > 0) {
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
          if (httpCode == HTTP_CODE_OK) {
            payload = client.getString();
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", client.errorToString(httpCode).c_str());
        }
        client.end();
        bot.sendMessage(idc, "Você pediu " + payload + ", correto? Digite liberar para pegar sua peça. Se algo tá incorreto, fale com @renanserrano.", "");
      } else {
        bot.sendChatAction(idc, "typing");
        bot.sendMessage(idc, "Desculpa, mas seu pedido não consta como pago, ou os dados estão errados, tente novamente ou entre em contato @renanserrano", "");
      }
      msg = "a";
      passo = 3;
    }
    if (msg.length() > 3 && passo == 3 && idc == idcNew && idu == iduNew) {
      bot.sendChatAction(idc, "typing");
      freeTheBox(order, email);
      bot.sendMessage(idc, "Sua peça já está liberada, abra a porta que está sinalizada ;)", "");
      bot.sendChatAction(idc, "typing");
      bot.sendMessage(idc, "Por favor pode deixar a porta encostada.", "");
      bot.sendChatAction(idc, "typing");
      bot.sendMessage(idc, "Aproveite sua peça! Você pode devolver ela ou trocar, enviando um email para concierge@trendt.com.br =]", "");
      msg = "a";
      passo = 0;
    }
    if (bot.message[i][5] == "oi" && passo == 0) {
      String wellcome = "mensagem";
      String wellcome1 = "mensagem";
      String wellcome2 = "mensagem";
      bot.sendMessage(bot.message[i][4], wellcome, "");
      bot.sendMessage(bot.message[i][4], wellcome1, "");
      bot.sendMessage(bot.message[i][4], wellcome2, "");
      Start = true;
    }
    if (bot.message[i][5] == "onde" && passo == 0) {
      bot.sendMessage(bot.message[i][4], "A nossa loja está agora aqui", "");
      bot.sendLocation(bot.message[i][4], -23.564983, -46.699168);
    }
    if (bot.message[i][5] == "fechar tudo" && passo == 0) {
      bot.sendMessage(bot.message[i][4], "Vou fechar em 10 segundos.", "");
      delay(10000);
      Serial.println("fecharportas");
    }
  }
  bot.message[0][0] = "";   // All messages have been replied - reset new messages
}


void setup() {
  Serial.begin(9600);
  delay(3000);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  bot.begin();      // launch Bot functionalities
  pinMode(2, OUTPUT); // initialize digital pin 2 as an output.
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    bot.getUpdates(bot.message[0][1], 1);   // launch API GetUpdates up to xxx message
    Bot_ExecMessages();   // reply to message with Echo
    Bot_lasttime = millis();
  }
}

boolean paid(String order, String email) {
  String urlOrder = "http://token.herokuapp.com/get_order.php?order=" + order + "&email=" + email;
  client.begin(urlOrder);
  int httpCode = client.GET();
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      payload = client.getString();
      //Serial.println(urlOrder);
      //Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", client.errorToString(httpCode).c_str());
  }
  client.end();
  if (payload == "1") {
    return true;
  } else {
    return false;
  }
}

void freeTheBox(String order, String email) {
  String urlOrder = "http://token.herokuapp.com/get_order_product.php?see=size&order=" + order + "&email=" + email;
  client.begin(urlOrder);
  int httpCode = client.GET();
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      payload = client.getString();
      //Serial.println(urlOrder);
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", client.errorToString(httpCode).c_str());
  }
  client.end();

  if (payload == "EXTRA SMALL") {
    Serial.println("liberado 1");
    int s = 111111;
    Serial.println(s);
  } else if (payload == "SMALL") {
    Serial.println("liberado 2");
    int s = 222222;
    Serial.println(s);
  } else if (payload == "MEDIUM") {
    Serial.println("liberado 3");
    int s = 333333;
    Serial.println(s);
  } else if (payload == "LARGE") {
    Serial.println("liberado 4");
    int s = 444444;
    Serial.println(s);
  } else if (payload == "EXTRA LARGE") {
    Serial.println("liberado 5");
    int s = 555555;
    Serial.println(s);
  } else if (payload == "SUPER LARGE") {
    Serial.println("liberado 6");
    int s = 666666;
    Serial.println(s);
  }
}


