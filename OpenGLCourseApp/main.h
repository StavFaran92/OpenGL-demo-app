#pragma once

int main(int argc, char* argv[]);

void handleEvents(SDL_Event& e, bool& quit, Camera& camera, double deltaTime);
