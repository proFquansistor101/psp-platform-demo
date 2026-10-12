#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <psprtc.h>
#include <stdio.h>

PSP_MODULE_INFO("PSPPlatformDemo", PSP_MODULE_USER, 0, 2);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define W 34
#define H 16
#define GAME_SECONDS 45

static unsigned int rng = 0x13579BDF;
static unsigned int random_next(void) { rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5; return rng; }
static int player_x, player_y, target_x, target_y, score;
static int state = 0; /* 0=menu, 1=playing, 2=finished */
static u64 started;

static void new_target(void) {
    do {
        target_x = (int)(random_next() % W);
        target_y = (int)(random_next() % H);
    } while (target_x == player_x && target_y == player_y);
}
static void start_game(void) {
    player_x = W / 2; player_y = H / 2; score = 0;
    rng ^= (unsigned int)sceKernelGetSystemTimeLow();
    new_target();
    started = sceKernelGetSystemTimeWide();
    state = 1;
}
static int seconds_left(void) {
    u64 elapsed = (sceKernelGetSystemTimeWide() - started) / 1000000ULL;
    return elapsed >= GAME_SECONDS ? 0 : GAME_SECONDS - (int)elapsed;
}
static void draw(void) {
    int x, y;
    pspDebugScreenClear();
    pspDebugScreenSetXY(2, 1);
    pspDebugScreenSetTextColor(0x00E0B090);
    pspDebugScreenPrintf("PSP PLATFORM  /  DEMO 0.2\n");
    pspDebugScreenSetTextColor(0x00FFFFFF);
    if (state == 0) {
        pspDebugScreenSetXY(4, 7); pspDebugScreenPrintf("SIGNAL COLLECTOR");
        pspDebugScreenSetXY(4, 10); pspDebugScreenPrintf("Collect * with your @ cursor.");
        pspDebugScreenSetXY(4, 12); pspDebugScreenPrintf("D-PAD: Move  |  45 seconds");
        pspDebugScreenSetXY(4, 16); pspDebugScreenPrintf("Press X to play");
        pspDebugScreenSetXY(4, 18); pspDebugScreenPrintf("START: Exit");
        return;
    }
    pspDebugScreenSetXY(2, 3);
    pspDebugScreenPrintf("SCORE: %d    TIME: %d", score, state == 1 ? seconds_left() : 0);
    for (y = 0; y < H; ++y) {
        pspDebugScreenSetXY(2, 5 + y);
        pspDebugScreenPrintf("|");
        for (x = 0; x < W; ++x) {
            if (x == player_x && y == player_y) pspDebugScreenPrintf("@");
            else if (x == target_x && y == target_y) pspDebugScreenPrintf("*");
            else pspDebugScreenPrintf(" ");
        }
        pspDebugScreenPrintf("|");
    }
    pspDebugScreenSetXY(2, 22);
    if (state == 2) pspDebugScreenPrintf("TIME UP!  X: Restart  O: Menu");
    else pspDebugScreenPrintf("D-PAD: Move  O: Menu  START: Exit");
}
int main(int argc, char **argv) {
    SceCtrlData pad;
    unsigned int previous = 0;
    u64 last_draw = 0;
    (void)argc; (void)argv;
    pspDebugScreenInit();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
    draw();
    while (1) {
        unsigned int pressed;
        u64 now;
        sceCtrlPeekBufferPositive(&pad, 1);
        pressed = pad.Buttons & ~previous;
        if (pressed & PSP_CTRL_START) break;
        if (state == 0) {
            if (pressed & PSP_CTRL_CROSS) start_game();
        } else if (state == 1) {
            if (pressed & PSP_CTRL_CIRCLE) state = 0;
            else {
                if ((pressed & PSP_CTRL_LEFT) && player_x > 0) --player_x;
                if ((pressed & PSP_CTRL_RIGHT) && player_x < W - 1) ++player_x;
                if ((pressed & PSP_CTRL_UP) && player_y > 0) --player_y;
                if ((pressed & PSP_CTRL_DOWN) && player_y < H - 1) ++player_y;
                if (player_x == target_x && player_y == target_y) { ++score; new_target(); }
                if (seconds_left() == 0) state = 2;
            }
        } else {
            if (pressed & PSP_CTRL_CROSS) start_game();
            if (pressed & PSP_CTRL_CIRCLE) state = 0;
        }
        now = sceKernelGetSystemTimeWide();
        if (now - last_draw >= 100000ULL || pressed) { draw(); last_draw = now; }
        previous = pad.Buttons;
        sceKernelDelayThread(16000);
    }
    sceKernelExitGame();
    return 0;
}
