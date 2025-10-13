#ifndef __KEYPAD_H__
#define __KEYPAD_H__

class Keypad {
  int rows[4];
  int cols[4];
  uint16_t last_scan;
public:
  const static uint16_t KEY_1 = 1 << 0;
  const static uint16_t KEY_2 = 1 << 1;
  const static uint16_t KEY_3 = 1 << 2;
  const static uint16_t KEY_A = 1 << 3;
  const static uint16_t KEY_4 = 1 << 4;
  const static uint16_t KEY_5 = 1 << 5;
  const static uint16_t KEY_6 = 1 << 6;
  const static uint16_t KEY_B = 1 << 7;
  const static uint16_t KEY_7 = 1 << 8;
  const static uint16_t KEY_8 = 1 << 9;
  const static uint16_t KEY_9 = 1 << 10;
  const static uint16_t KEY_C = 1 << 11;
  const static uint16_t KEY_STAR = 1 << 12;
  const static uint16_t KEY_0 = 1 << 13;
  const static uint16_t KEY_HASH = 1 << 14;
  const static uint16_t KEY_D = 1 << 15;

  Keypad ::Keypad(uint8_t top_row_pin, uint8_t top_col_pin) {
    Serial.print("Rows: ");
    for (int i = 0; i < 4; i++) {
      rows[i] = top_row_pin - (3 - i);
      pinMode(rows[i], OUTPUT);
      digitalWrite(rows[i], HIGH);
      Serial.print(i);
      Serial.print(":");
      Serial.print(rows[i]);
      Serial.print(" ");
    }
    Serial.print("\nCols: ");
    for (int i = 0; i < 4; i++) {
      cols[i] = top_col_pin - (3-i);
      pinMode(cols[i], INPUT_PULLUP);
      Serial.print(i);
      Serial.print(":");
      Serial.print(cols[i]);
      Serial.print(" ");
    }

    Serial.println("");
  }

  uint16_t Keypad::scan() {
    int ret = 0;
    int o = 1;
    for (int i = 0; i < 4; i++) {
      digitalWrite(rows[i], LOW);
      for (int j = 0; j < 4; j++) {
        if (digitalRead(cols[j]) == LOW) {
          ret += o;
        }
        o <<= 1;
        /*
          Serial.print("i= ");
          Serial.print(i);
          Serial.print(" rows[i]= ");
          Serial.print(rows[i]);

          Serial.print(" j=");
          Serial.print(j);
          Serial.print(" dr=");
          Serial.print(digitalRead(cols[j]));
          Serial.print(" ret=");
          Serial.println(ret);
          */
      }
      //delay(500);
      digitalWrite(rows[i], HIGH);
    }
    //Serial.println("");
    return ret;
  }
};

#endif
