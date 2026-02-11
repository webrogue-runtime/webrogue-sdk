#include <stdio.h>
#include <stdlib.h>

#if USE_SDL_VERSION == 2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#define SDL_V(v2, v3) v2
#elif USE_SDL_VERSION == 3
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#define SDL_V(v2, v3) v3
#else
#error Unknown USE_SDL_VERSION value
#endif

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT (WINDOW_WIDTH)

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
                       TTF_Font *font, SDL_Texture **texture,
                       SDL_V(SDL_Rect, SDL_FRect) * rect) {
  int text_width;
  int text_height;
  SDL_Surface *surface;
  SDL_Color textColor = {255, 255, 255, 0};

  surface = SDL_V(TTF_RenderText_Solid(font, text, textColor),
                  TTF_RenderText_Solid(font, text, strlen(text), textColor));
  *texture = SDL_CreateTextureFromSurface(renderer, surface);
  text_width = surface->w;
  text_height = surface->h;
  SDL_V(SDL_FreeSurface, SDL_DestroySurface)(surface);
  rect->x = x;
  rect->y = y;
  rect->w = text_width;
  rect->h = text_height;
}

int main(int argc, char **argv) {
  SDL_Event event;
  SDL_V(SDL_Rect, SDL_FRect) rect1, rect2;
  SDL_V(SDL_Rect, SDL_FRect) frect1;
  SDL_Renderer *renderer;
  SDL_Texture *texture1, *texture2;
  SDL_Window *window;
  char *font_path;
  int quit;
  SDL_V(int, bool) ret;
#define CHECK_RET_ERR SDL_V(ret < 0, !ret)

  font_path = "/app/FreeSans.otf";

  /* Inint TTF. */
  SDL_Init(SDL_V(SDL_INIT_VIDEO | SDL_INIT_TIMER, SDL_INIT_VIDEO));
  // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
  // SDL_WINDOW_OPENGL SDL_WINDOW_VULKAN
  ret =
      SDL_V(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH,
                                        0, &window, &renderer),
            SDL_CreateWindowAndRenderer("AAA?", WINDOW_WIDTH, WINDOW_WIDTH,
                                        0, &window, &renderer));
  if (CHECK_RET_ERR) {
    SDL_Log("Couldn't create window or renderer: %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  TTF_Init();
  TTF_Font *font = TTF_OpenFont(font_path, 24);
  if (!font) {
    SDL_Log("Couldn't open font: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  get_text_and_rect(renderer, 0, 0, "hello", font, &texture1, &rect1);
  get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font, &texture2,
                    &rect2);

  frect1.x = 100;
  frect1.y = 100;
  frect1.w = 100;
  frect1.h = 100;

  quit = 0;
  while (!quit) {
    while (SDL_PollEvent(&event) == 1) {
      if (event.type == SDL_V(SDL_QUIT, SDL_EVENT_QUIT)) {
        quit = 1;
      }
      if (event.type == SDL_V(SDL_KEYDOWN, SDL_EVENT_KEY_DOWN)) {
        SDL_Log("Key pressed: %d %d\n",
                SDL_V(event.key.keysym.scancode, event.key.scancode),
                SDL_V(event.key.keysym.sym, event.key.key));
      }
    }

    int window_width, window_height;
    SDL_V(SDL_GetRendererOutputSize,
          SDL_GetRenderOutputSize)(renderer, &window_width, &window_height);
    // SDL_GL_GetAttribute SDL_GL_CONTEXT_DEBUG_FLAG
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // Set renderer color red to draw the square
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

    frect1.x = window_width - 100;
    frect1.y = window_height - 100;
    // Draw filled square
    SDL_RenderFillRect(renderer, &frect1);

    /* Use TTF textures. */
    SDL_V(SDL_RenderCopy, SDL_RenderTexture)(renderer, texture1, NULL, &rect1);
    SDL_V(SDL_RenderCopy, SDL_RenderTexture)(renderer, texture2, NULL, &rect2);

    SDL_RenderPresent(renderer);
  }

  /* Deinit TTF. */
  SDL_DestroyTexture(texture1);
  SDL_DestroyTexture(texture2);
  TTF_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}