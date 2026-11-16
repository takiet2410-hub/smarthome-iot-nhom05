#pragma once

void kietSetup();
void kietLoop();
void kietHandleMqttMessage(const char* topic, const char* payload);
