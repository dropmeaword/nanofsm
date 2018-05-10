#include "Fsm.h"
#include "flasher.h"

#define LED_PIN 10

#define EVENT_GOT_SCARED 1000
#define EVENT_GOT_SLEEPY 1001

int threshold = 160;

Flasher slow(LED_PIN, 800, 2000);
Flasher fast(LED_PIN, 100, 500);

void on_sleeping_enter() {
    Serial.println("on_sleeping_enter");
    digitalWrite(LED_PIN, HIGH);
}

void on_sleeping_leave() {
    Serial.println("on_sleeping_leave");
    digitalWrite(LED_PIN, LOW);
}

void on_sleeping_loop() {
}

void on_stressed_enter() {
    Serial.println("on_stressed_enter");
    digitalWrite(LED_PIN, HIGH);
}

void on_stressed_leave() {
    Serial.println("on_stressed_leave");
    digitalWrite(LED_PIN, LOW);
}

State sleeping("sleeping", &on_sleeping_enter, &on_sleeping_loop, &on_sleeping_leave);
State stressed("stressed", &on_stressed_enter, NULL, &on_stressed_leave);

Fsm fsm(&sleeping);

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
}


void event_loop() {
  // read analog sensor for interaction
  int reading = 0;
  for(int i = 0; i < 4; i++) {
    reading += analogRead(A0);
  }
  reading == reading >> 2;

  // flash if threshold is overflown, stop otherwise
  if(reading > threshold) {
      fsm.trigger(EVENT_GOT_SLEEPY);
  }
}

void loop() {
    event_loop();

    fsm.run_machine();
    delay(200);
}
