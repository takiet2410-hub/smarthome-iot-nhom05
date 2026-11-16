#pragma once

void phatSetup();
void phatLoop();
void phatHandleMqttMessage(const char* topic, const char* payload);
