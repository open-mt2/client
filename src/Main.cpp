#include <Client/UI/ClientMain.h>
#include <Platform/Platform.h>
#include <SDL2/SDL.h>
#include <string>

int main(int argc, char **argv) {
  std::string commandLine;
  for (int i = 1; i < argc; ++i) {
    if (i > 1)
      commandLine += ' ';
    commandLine += argv[i];
  }
  return ClientMain_Run(commandLine);
}