#include <SPI.h>
#include <WebSocketClient.h>

#if PLATFORM_ARCH == PLATFORM_ARCHITECTURE_SAMD21
# define CONSOLE SerialUSB
#else
# define CONSOLE Serial
#endif

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
WebSocketClient client;

void onOpen(WebSocket &ws) {
  CONSOLE.println(F("Connected"));
  
  char message[] = "Hello from Arduino client!";
  ws.send(TEXT, message, strlen(message));
}

void onClose(WebSocket &ws, const eWebSocketCloseEvent code, const char *reason, uint16_t length) {
  CONSOLE.println(F("Disconnected"));
}

void onMessage(WebSocket &ws, const eWebSocketDataType dataType, const char *message, uint16_t length) {
  switch (dataType) {
    case TEXT:
      CONSOLE.print(F("Received: ")); CONSOLE.println(message);
      break;
    case BINARY:
      CONSOLE.println(F("Received binary data"));
      break;
  }
}

void onError(const eWebSocketError code) {
  CONSOLE.print("Error: "); CONSOLE.println(code);
}

void setup() {
  CONSOLE.begin(115200);
  while (!CONSOLE) ;
  
  CONSOLE.println(F("Initializing ... "));
  
  if (Ethernet.begin(mac) == 0) {
    CONSOLE.println(F("Can't open ethernet device"));
    while (true) ;
  }

  CONSOLE.print(F("Client IP: "));
  CONSOLE.println(Ethernet.localIP());

  // ---

  client.setOnOpenCallback(onOpen);
  client.setOnCloseCallback(onClose);
  client.setOnMessageCallback(onMessage);

  if (!client.open("192.168.46.9", 3000)) {
    CONSOLE.println(F("Connection failed!"));
    while (true) ;
  }
}

void loop() {
  client.listen();
}