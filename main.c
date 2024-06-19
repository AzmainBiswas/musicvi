// #include "raylib/include/raylib.h"
#include "raylib-5.0_win64_mingw-w64/include/raylib.h"
#include <assert.h>
#include <complex.h>
#include <math.h>
#include <stdio.h>

#define N 158
float pi;
float in[N];
float complex out[N];
float max_amp;

void fft(float in[], size_t stride, float complex out[], size_t n);
float amp(float complex z);
static void callback(void *buffer, unsigned int frames);

int main(int argc, char **argv)
{
  pi = atan2f(1, 1) * 4; // define pi
  char *music_path;

  if (argc > 2)
  {
    printf("only one argument is accepted");
  }

  if (argv[1] == NULL)
    music_path = "Burden of Truth [Idgc7Kr0VQY].mp3";
  else
    music_path = argv[1];

  // initializing audio device
  InitAudioDevice();
  // lodding the music
  Music music = LoadMusicStream(music_path);
  assert(music.stream.channels == 2);

  PlayMusicStream(music);
  AttachAudioStreamProcessor(music.stream, callback);

  SetTargetFPS(25);
  InitWindow(1000, 600, "MusicVc");

  while (!WindowShouldClose())
  {
    // close the window
    if (IsKeyPressed(KEY_Q))
      return 0;

    UpdateMusicStream(music);

    if (IsMusicStreamPlaying(music))
    {
      if (IsKeyPressed(KEY_SPACE))
        PauseMusicStream(music);
    }
    else
    {
      if (IsKeyPressed(KEY_SPACE))
        PlayMusicStream(music);
    }

    // TODO: Drag and drop feature

    if (IsFileDropped())
    {
      printf("A File is Just Droped.\n");
      UnloadMusicStream(music);

      FilePathList droppedFiles = LoadDroppedFiles(); // loading filepath
      printf("File Path: %s\n", droppedFiles.paths[0]);
      music = LoadMusicStream(droppedFiles.paths[0]); // load new music.
      assert(music.stream.channels == 2);

      PlayMusicStream(music);
      AttachAudioStreamProcessor(music.stream, callback);
      UnloadDroppedFiles(droppedFiles); // Unloading filepath form memory
    }

    int width = GetRenderWidth();
    int hight = GetRenderHeight();

    BeginDrawing();
    ClearBackground((Color){40, 44, 52, 255});

    float cell_width = (float)width / N;

    for (size_t i = 0; i < N; ++i)
    {
      float t = (float)amp(out[i]) / max_amp;
      float rect_hight = (float)hight/1.4 * t;
      float y_position = hight - rect_hight - 5;
      DrawRectangle((i + 1) * cell_width, y_position, cell_width, rect_hight, (Color){198, 120, 221, 255});
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

// Ported from https://rosettacode.org/wiki/Fast_Fourier_transform#Python
// fast fourier transform
void fft(float in[], size_t stride, float complex out[], size_t n)
{
  assert(n > 0);

  if (n == 1)
  {
    out[0] = in[0];
    return;
  }

  fft(in, stride * 2, out, n / 2);
  fft(in + stride, stride * 2, out + n / 2, n / 2);

  for (size_t k = 0; k < n / 2; ++k)
  {
    float t = (float)k / n;
    float complex v = cexp(-2 * I * pi * t) * out[k + n / 2];
    float complex e = out[k];
    out[k] = e + v;
    out[k + n / 2] = e - v;
  }
}

// finding amplitude.
float amp(float complex z)
{
  float a = fabsf(crealf(z));
  float b = fabsf(cimagf(z));
  if (a < b)
    return b;
  return a;
}

static void callback(void *bufferData, unsigned int frames)
{
  if (frames < N)
    return;
  if (frames > N)
    frames = N;

  float(*fs)[2] = bufferData;

  for (size_t i = 0; i < frames; ++i)
  {
    in[i] = fs[i][0];
  }

  fft(in, 1, out, frames);

  max_amp = 0.0f;
  for (size_t i = 0; i < frames; ++i)
  {
    float a = amp(out[i]);
    if (max_amp < a)
      max_amp = a;
  }
}
