#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10
#define RST_PIN  9

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("*****"));
}

void loop() {
  static MFRC522::StatusCode status;

  static unsigned char buffer[50];
  static unsigned char size = 18;
  static unsigned char pageOffset = 0x04, data[] = "Eletrogate-teste-mifare-ultralight-escrita-teste";

  if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    return;

  for(unsigned char pagePointer = 0; pageOffset + pagePointer < 16; pagePointer ++) {
    status = mfrc522.MIFARE_Ultralight_Write(pageOffset + pagePointer, &data[pagePointer * 4], 4);
    if(status != MFRC522::STATUS_OK) {
      Serial.print(F("Escrita falhou: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }      
  }
  Serial.println(F("Escrita ok"));

  for(unsigned char pagePointer = 0; pageOffset + pagePointer < 16; pagePointer += 4) {
    status = mfrc522.MIFARE_Read(pageOffset + pagePointer, &buffer[pagePointer * 4], &size);
    if(status != MFRC522::STATUS_OK) {
      Serial.print(F("Leitura falhou: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  }
  Serial.print(F("Dados lidos: "));
  for(size_t i = 0; i < 48; i ++)
    Serial.print((char) buffer[i]);
  Serial.println();
  mfrc522.PICC_HaltA();
}
