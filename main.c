#include "main.h"
#include "lady.h"
#include "startlady.h"
#include "table.h"
#include "jessica.h"
#include "isbael.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
  LEVEL2
};

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
	REG_DISPCNT = MODE3 | BG2_ENABLE;

  int time = 0;
  int score = 0;
  int startBackgroundDrawn = 0;
  int looseBackgroundDrawn = 0;
  int playBackgroundDrawn = 0;
  int play2BackgroundDrawn = 0;
  int winBackgroundDrawn = 0;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;
  struct state cs, ps;
  struct player *pp, *opp;
  struct ball *bp, *obp;

//initalize Player
pp = &cs.players;
pp->row = SCREEN_HEIGHT/2 - LADY_HEIGHT/2;
pp->col = SCREEN_WIDTH/2 - LADY_WIDTH/2;

// initialize the parameters for bouncing
bp = &cs.balls;
bp->row = randint(10,150);
bp->col = randint(10,230);
bp->size = 10;
bp->rd = 1;
bp->cd = 1;
bp->color = RED;

  // Load initial application state
  enum gba_state state = START;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw

    waitForVBlank();

    switch (state) {
      case START:
        playBackgroundDrawn = 0;
        

        if (!startBackgroundDrawn) {
          pp->row = SCREEN_HEIGHT/2 - LADY_HEIGHT/2;
          pp->col = SCREEN_WIDTH/2 - LADY_WIDTH/2;
          drawImageDMA(0,0,ISBAEL_WIDTH, ISBAEL_HEIGHT, isbael);
          startBackgroundDrawn = 1;
          drawImageDMA(SCREEN_HEIGHT/2 - LADY_HEIGHT/2, SCREEN_WIDTH/2 - LADY_WIDTH/2, LADY_WIDTH, LADY_HEIGHT, startlady);
        }

        if (vBlankCounter%60 == 0) {
          time++;
        }

        drawString(120, 60, "Press enter to start", WHITE);
  
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
           state = PLAY;
        }

        // state = ?
        break;

      case PLAY:
    
      startBackgroundDrawn = 0;
      looseBackgroundDrawn = 0;
      winBackgroundDrawn = 0;
      play2BackgroundDrawn = 0;

       ps = cs;
      
       if (!playBackgroundDrawn) {
        drawRectDMA(0,0,SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
        playBackgroundDrawn = 1;
        time = 0;
        score = 0;
        pp->row = SCREEN_HEIGHT/2 - LADY_HEIGHT/2;
        pp->col = SCREEN_WIDTH/2 - LADY_WIDTH/2;
       }

       drawImageDMA(pp->row, pp->col, 25, 25, lady);

       if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
        opp = &ps.players;
        drawRectDMA(opp->row, opp->col,
                     25, 25,
                     0x0000);
        pp->row = pp->row - 5;
        drawImageDMA(pp->row, pp->col, 25, 25, lady);
       }

       if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
        opp = &ps.players;
        drawRectDMA(opp->row, opp->col,
                     LADY_WIDTH, LADY_HEIGHT,
                     0x0000);
        pp->col = pp->col + 5;
        drawImageDMA(pp->row, pp->col, LADY_WIDTH, LADY_HEIGHT, lady);
       }

       if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
        opp = &ps.players;
        drawRectDMA(opp->row, opp->col,
                     LADY_WIDTH, LADY_HEIGHT,
                     0x0000);
        pp->col = pp->col - 5;
        drawImageDMA(pp->row, pp->col, LADY_WIDTH, LADY_HEIGHT, lady);
       }

       if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
        opp = &ps.players;
        drawRectDMA(opp->row, opp->col,
                     LADY_WIDTH, LADY_HEIGHT,
                     0x0000);
        pp->row = pp->row + 5;
        drawImageDMA(pp->row, pp->col, LADY_WIDTH, LADY_HEIGHT, lady);
       }

       if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
        state = START;
       }
       
       if (vBlankCounter%60 == 0) {
          time++;
        }

        char timer[51];
        sprintf(timer, "time: %d", time);
        drawRectDMA(140, 5, 200, 10, 0x0000);
        drawString(140, 5, timer, YELLOW);

       //Check for collision

        //left side top
        int cRow = (pp->row);
        if((cRow < (bp->row + bp->size)) &&
           ((cRow+LADY_WIDTH) > bp->row) &&
           (pp->col < (bp->col + bp->size)) &&
           ((pp->col + LADY_HEIGHT) > bp->col)
          ) {
          score++;

          // Erase the old ball
        obp = &ps.balls;
        drawRectDMA(obp->row, obp->col,
                      obp->size, obp->size,
                      0x0000);

        // Draw the new ball
        bp = &cs.balls;
        bp->row = randint(10,140);
        bp->col = randint(10,230);

        }
       


       drawRectDMA(bp->row, bp->col,
              bp->size, bp->size,
              0x000f);

        // Erase and draw the new score
			char buffer[51];
			sprintf(buffer, "Score: %d", score);
			drawRectDMA(150, 5, 200, 10, 0x0000);
			drawString(150, 5, buffer, YELLOW);

      //check win state
      if (score == 3) {
        state = LEVEL2;
      }

      //check loose state
      if (time == 30) {
        state = LOSE;
      }    

        // state = ?
        break;
      case WIN:
      startBackgroundDrawn = 0;
      
      if (!winBackgroundDrawn) {
          drawImageDMA(0, 0, TABLE_WIDTH, TABLE_HEIGHT, table);
          winBackgroundDrawn = 1;
        }
      drawString(110, 65, "You win!", BLACK);
       if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
        state = START;
       }

        // state = ?
        break;
      case LOSE:
      startBackgroundDrawn = 0;
      if (!looseBackgroundDrawn) {
          drawImageDMA(0, 0, JESSICA_WIDTH, JESSICA_WIDTH, jessica);
          looseBackgroundDrawn = 1;
        }
        drawString(130, 65, "Game over", BLACK);

         if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
        state = START;
       }

        // state = ?
        break;

      case LEVEL2:
    
      startBackgroundDrawn = 0;
      looseBackgroundDrawn = 0;
      winBackgroundDrawn = 0;

       ps = cs;
      
       if (!play2BackgroundDrawn) {
        drawRectDMA(0,0,SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
        play2BackgroundDrawn = 1;
        time = 0;
        pp->row = SCREEN_HEIGHT/2 - LADY_HEIGHT/2;
        pp->col = SCREEN_WIDTH/2 - LADY_WIDTH/2;
       }

       drawImageDMA(pp->row, pp->col, 25, 25, lady);

       if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
        opp = &ps.players;
        drawRectDMA(opp->row, opp->col,
                     25, 25,
                     BLUE);
        pp->row = pp->row - 5;
        drawImageDMA(pp->row, pp->col, 25, 25, lady);
       }

       if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
        opp = &ps.players;
        drawRectDMA(opp->row, opp->col,
                     LADY_WIDTH, LADY_HEIGHT,
                     BLUE);
        pp->col = pp->col + 5;
        drawImageDMA(pp->row, pp->col, LADY_WIDTH, LADY_HEIGHT, lady);
       }

       if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
        opp = &ps.players;
        drawRectDMA(opp->row, opp->col,
                     LADY_WIDTH, LADY_HEIGHT,
                     BLUE);
        pp->col = pp->col - 5;
        drawImageDMA(pp->row, pp->col, LADY_WIDTH, LADY_HEIGHT, lady);
       }

       if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
        opp = &ps.players;
        drawRectDMA(opp->row, opp->col,
                     LADY_WIDTH, LADY_HEIGHT,
                     BLUE);
        pp->row = pp->row + 5;
        drawImageDMA(pp->row, pp->col, LADY_WIDTH, LADY_HEIGHT, lady);
       }

       if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
        state = START;
       }
       
       if (vBlankCounter%60 == 0) {
          time++;
        }

        char clock[51];
        sprintf(clock, "time: %d", time);
        drawRectDMA(140, 5, 200, 10, BLUE);
        drawString(140, 5, clock, YELLOW);

       //Check for collision

        //left side top
        cRow = (pp->row);
        if((cRow < (bp->row + bp->size)) &&
           ((cRow+LADY_WIDTH) > bp->row) &&
           (pp->col < (bp->col + bp->size)) &&
           ((pp->col + LADY_HEIGHT) > bp->col)
          ) {
          score++;

          // Erase the old ball
        obp = &ps.balls;
        drawRectDMA(obp->row, obp->col,
                      obp->size, obp->size,
                      BLUE);

        // Draw the new ball
        bp = &cs.balls;
        bp->row = randint(10,140);
        bp->col = randint(10,230);

        }
       


       drawRectDMA(bp->row, bp->col,
              bp->size, bp->size,
              GREEN);

        // Erase and draw the new score
			char scorer[51];
			sprintf(scorer, "Score: %d", score);
			drawRectDMA(150, 5, 200, 10, BLUE);
			drawString(150, 5, scorer, YELLOW);

      //check win state
      if (score == 8) {
        state = WIN;
      }

      //check loose state
      if (time == 20) {
        state = LOSE;
      }    

        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}
