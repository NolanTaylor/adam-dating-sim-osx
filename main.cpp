#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Sprites.h"

bool quit = false;

SDL_sem* dataLock = false; // semaphore

int main(int argc, char* args[])
{
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	dataLock = SDL_CreateSemaphore(1);

	Sprite Adam("Sprites/Adam.png");
	Sprite coolBook("Sprites/book.png");
	Sprite coolHat("Sprites/coolHat.png");
	Sprite coolShirt("Sprites/coolShirt.png");
	Sprite eyes("Sprites/eyes.png");
	Sprite dialogueBox("Sprites/dialogue.png");
	Sprite bubble("Sprites/SpeechBubble.png");
	Sprite heart("Sprites/Heart.png");
	Sprite egg("Sprites/egg.png");
	Sprite graphFrame("Sprites/graphFrame.png");
	Sprite graph("Sprites/graph.png");
	Sprite radarFrame("Sprites/radarFrame.png");
	Sprite radar("Sprites/radar.png");
	Sprite tensionFrame("Sprites/tensionFrame.png");
	Sprite tensionGraph("Sprites/tensionGraph.png");
	Sprite datePowerFrame("Sprites/datePowerFrame.png");
	Sprite datePowerBar("Sprites/datePowerBar.png");
	Sprite DATE_POWER("Sprites/DATE_POWER.png");
	Sprite present("Sprites/present.png");
	Sprite spaghett("Sprites/spaghett.png");
	Sprite FADE("Sprites/FADE.png");
	Sprite back("Sprites/back.png");
	Sprite DATING("Sprites/DATING.png");
	Sprite START("Sprites/START.png");

	Text dialogue("Sprites/text.png");
	Text choice("Sprites/text.png");
	Text finalText("Sprites/textBlack.png");

	Sound DatingStart; DatingStart.loadFromFile("Audio/DatingStart.wav");
	Sound DatingTense; DatingTense.loadFromFile("Audio/DatingTense.wav");
	Sound DatingFight; DatingFight.loadFromFile("Audio/DatingFight.wav");
	Sound Grab; Grab.loadFromFile("Audio/Grab.wav");
	Sound Oof; Oof.loadFromFile("Audio/oof.wav");

	SDL_Rect eyeClip[7];
	SDL_Rect screen = { 0, 0, 900, 700 };
	SDL_Rect graphRect = { 0, 0, 120, 120 };
	SDL_Rect tensionRect = { 0, 0, 118, 233 };
	SDL_Rect speechBubble = { 540, 70, 240, 145 };
	SDL_Rect choice1Rect = { 270, 590, 45, 698 };
	SDL_Rect choice2Rect = { 270, 650, 44, 698 };
	SDL_Rect finalRect = { 300, 250, 50, 50 };
	SDL_Rect bar = { 0, 0, 0, 45 };

	enum eyeEnum
	{
		normal, suprise, twinkle,
		dare, sus, blush, sheep,
	};

	enum clothing
	{
		shirt = 1, shoulder, hat, watch, hand,
	};

	for (int i = 0; i < 7; i++)
	{
		eyeClip[i].x = (i % 3) * 108;
		eyeClip[i].y = (i / 3) * 66;
		eyeClip[i].w = 107;
		eyeClip[i].h = 65;
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	const int xpos = 200, ypos1 = 700 - 126 + 20, ypos2 = 700 - 126 + 80,
			  choice_y1 = SCREEN_HEIGHT - dialogueBox.getHeight() + 20,
			  choice_y2 = SCREEN_HEIGHT - dialogueBox.getHeight() + 80,
			  choice_x = SCREEN_WIDTH / 2;

	bool button1 = true, button2 = true, speaking = false, book = false,
		 datePower = false, speech = true, select_c = false, datingHUD = false,
		 open = false, coolDude = false, tension = false, white = false;

	int act = 0, select = 0, count = 0, radarAngle = 0, found = NULL,
		eyePos = eyeEnum::normal, heart_x = xpos,
		heart_y = ypos1, hatPos = 19, alpha = 0;

	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 60;
	short timePerFrame = 1000 / 60; // miliseconds

	SDL_Event event;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					if (act != 67) { heart_y = ypos1; }
					break;
				case SDLK_s:
					if (act != 67) { heart_y = ypos2; }
					break;
				case SDLK_c:
					select_c = true;
					break;
				case SDLK_SPACE:
					select = 3;
					if (act != 67)
					{
						switch (heart_y)
						{
						case ypos1:
							select = 1;
							break;
						case ypos2:
							select = 2;
							break;
						}
					}
					else
					{
						if (heart_x > 330 && heart_x < 560 && heart_y > 360 && heart_y < 550 && found != clothing::shirt)
						{ found = clothing::shirt; }
						else if (heart_x > 630 && heart_x < 680 && heart_y > 350 && heart_y < 390)
						{ found = clothing::watch; }
						else if ((heart_x > 300 && heart_x < 340 && heart_y > 260 && heart_y < 330) ||
								 (heart_x > 540 && heart_x < 630 && heart_y > 270 && heart_y < 350) 
								 && found != clothing::shoulder)
						{ found = clothing::shoulder; }
						else if ((heart_x > 85 && heart_x < 220 && heart_y > 230 && heart_y < 350) ||
								 (heart_x > 650 && heart_x < 785 && heart_y > 245 && heart_y < 350)
								 && found != clothing::hand)
						{ found = clothing::hand; }
						else if (heart_x > 280 && heart_x < 490 && heart_y > 40 && heart_y < 200 && found != clothing::hat)
						{ found = clothing::hat; }
						std::cout << heart_x << "," << heart_y << std::endl;
					}
				}
			}
		}

		if (!startTime)
			startTime = SDL_GetTicks();
		else
			delta = endTime - startTime;

		if (delta < timePerFrame)
			SDL_Delay(timePerFrame - delta);
		if (delta > timePerFrame)
			fps = 1000 / delta;

		SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
		SDL_RenderClear(renderer);

		/*----------------------------------------*/

		if (datingHUD)
		{
			egg.render(45, 435);

			graph.render(150, 30, &graphRect);
			graphFrame.render(150, 30, NULL);
			if (graphRect.x >= 360) { graphRect.x = 0; }
			graphRect.x += 1;

			radar.renderEx(710, 380, NULL, 1, radarAngle);
			radarFrame.render(710, 380);
			if (radarAngle > 360) { radarAngle = 0; }
			radarAngle += 1;
		}

		if (tension)
		{
			tensionGraph.render(70, 20, &tensionRect);
			tensionFrame.render(70, 20, NULL);
			if (tensionRect.x >= 381) { tensionRect.x = 0; }
			tensionRect.x += 2;
		}

		Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
		eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260, 175, &eyeClip[eyePos]);
		dialogueBox.render((SCREEN_WIDTH - dialogueBox.getWidth()) / 2, SCREEN_HEIGHT - dialogueBox.getHeight() - 1, NULL);

		// adam overlays
		if (book)
			coolBook.render(80, 111, NULL);
		else if (coolDude)
		{
			if (open)
			{ spaghett.render(330, 40); }
			else if (hatPos < 0)
			{ present.render(330, 40); }

			coolHat.render(279, hatPos);
			coolShirt.render(240, 246, NULL);
		}

		if (datePower)
		{
			DATE_POWER.render(70, 20);
			datePowerFrame.render(70, 50, NULL);
			datePowerBar.render(73, 53, &bar);
		}

		if (speaking)
			bubble.render(500, 50, NULL);

		if (dialogue.getTyping() && !choice.getTyping())
		{
			if (dialogue.type(speechBubble, speech, 2))
			{
				dialogue.setString("");
				speech = false;
			}
			else
				select = 0;
		}

		if (choice.getTyping())
		{
			if (act >= 1096 && act <= 1098)
			{
				if (choice.type(choice1Rect, speech, 2))
				{
					choice.setString("");
					speech = false;
				}
			}
			else
			{
				choice.type(choice1Rect, false, 2);
				dialogue.type(choice2Rect, false, 2);
				choice.setString("");
				dialogue.setString("");
			}
		}

		if (act == 67)
		{
			if (state[SDL_SCANCODE_W]) { heart_y -= 1; }
			if (state[SDL_SCANCODE_A]) { heart_x -= 1; }
			if (state[SDL_SCANCODE_S]) { heart_y += 1; }
			if (state[SDL_SCANCODE_D]) { heart_x += 1; }
			heart.render(heart_x, heart_y, NULL);
		}

		if (finalText.getTyping())
		{
			FADE.render(0, 0);
			if (finalText.type(finalRect, speech, 2))
			{
				finalText.setString("");
				speech = false;
			}
			else
				select = 0;
		}

		if (act == 149)
		{
			back.render(0, 0);
			dialogueBox.render((SCREEN_WIDTH - dialogueBox.getWidth()) / 2, SCREEN_HEIGHT - dialogueBox.getHeight() - 1, NULL);
		}

		switch (act)
		{
		case 0:
			choice.setString("wasd to move");
			dialogue.setString("space to select advance");
			heart.render(heart_x, heart_y, NULL);
			if (select) { act++; select = 0; speaking = true; }
			break;
		case 1:
			dialogue.setString("So umm...");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 2:
			dialogue.setString("If you've seen/everything...");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 3:
			dialogue.setString("Do you want to/start the date?");
			if (select) { dialogue.setString(""); speech = true; select = 0; act++; }
			break;
		case 4:
			choice.setString("yes");
			dialogue.setString("no");
			heart.render(heart_x, heart_y, NULL);
			if (select) { select = 0; act++; heart_y = ypos1; }
			break;
		case 5:
			speaking = true;
			act++;
			break;
		case 6:
			dialogue.setString("Okay!/Dating Start!");
			if (select) { dialogue.setString(""); speech = true; speaking = false; select = 0; act++; }
			break;
		case 7: // DATING START!!! (fixed);
			SDL_Delay(100);
			DATING.render((SCREEN_WIDTH - DATING.getWidth()) / 2, 600);
			SDL_RenderPresent(renderer);
			SDL_Delay(500);
			START.render((SCREEN_WIDTH - START.getWidth()) / 2, 600);
			SDL_RenderPresent(renderer);
			SDL_Delay(500);
			act++;
			eyePos = eyeEnum::normal;
			speaking = true; select = 0;
			DatingStart.play(1, -1);
			SDL_Delay(500);
			break;
		case 8:
			dialogue.setString("Here we are/on our date!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 9:
			dialogue.setString("I've actually/never done this/before");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 10:
			dialogue.setString("But don't worry!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 11:
			dialogue.setString("You can't spell/prepared without/several letters/from my name");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 12:
			Grab.play(2, 0);
			for (int i = 0; i <= 360; i++)
			{
				SDL_RenderClear(renderer);
				Adam.renderEx((SCREEN_WIDTH - Adam.getWidth()) / 2,
					SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight(), NULL, 1, i, NULL);
				dialogueBox.render((SCREEN_WIDTH - dialogueBox.getWidth()) / 2,
					SCREEN_HEIGHT - dialogueBox.getHeight() - 1);
				SDL_RenderPresent(renderer); SDL_Delay(1);
			} act++; book = true;
			break;
		case 13:
			dialogue.setString("I snagged an/official dating/rulebook from/the library!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 14:
			dialogue.setString("We're ready to/have a great/time!!!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 15:
			dialogue.setString("Let's see...");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 16:
			dialogue.setString("Step 1: Press/the 'c' key on/your keyboard/for dating hud");
			if (select_c) { speech = true; datingHUD = true; select_c = false; select = 0; act++; }
			break;
		case 17:
			for (int i = 0; i <= 190; i += 1)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				coolBook.render(80, 111);
				dialogueBox.render((SCREEN_WIDTH - dialogueBox.getWidth()) / 2, SCREEN_HEIGHT - dialogueBox.getHeight() - 1, NULL);
				if (i <= 45) { egg.render(i, 435); }
				else { egg.render(45, 435); }
				if (i <= 150) { graphFrame.render(i, 30); graph.render(i, 30, &graphRect); }
				else { graphFrame.render(150, 30); graph.render(150, 30, &graphRect); }
				if (i <= 190) { radarFrame.render(900 - i, 380); radar.render(900 - i, 380); }
				else { radarFrame.render(710, 380); radar.render(710, 380); }
				SDL_RenderPresent(renderer); SDL_Delay(6);
			} act++; eyePos = eyeEnum::normal;
			break;
		case 18:
			dialogue.setString("Wowie!/I feel so/informed!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 19:
			dialogue.setString("I think we're/ready for/step 2!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 20:
			dialogue.setString("Step 2: Ask/them on a date");
			if (select) { eyePos = eyeEnum::normal; book = false; speech = true; select = 0; act++; }
			break;
		case 21:
			dialogue.setString("Ahem.");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 22:
			dialogue.setString("Human,/I the great/Adam");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 23:
			dialogue.setString("Will go on a/date with you!!");
			if (select) { dialogue.setString(""); speech = true; select = 0; act++; }
			break;
		case 24:
			choice.setString("yes");
			dialogue.setString("no");
			heart.render(heart_x, heart_y, NULL);
			if (select == 1) { eyePos = eyeEnum::twinkle; act = 1025; }
			else if (select == 2) { eyePos = eyeEnum::normal; act = 2025; heart_y = ypos1; }
			speech = true; select = 0; break;
		case 1025: // yes choice branch
			dialogue.setString("R-Really?/Wowie!!!");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act = 26; }
			break;
		case 2025: // no choice branch
			dialogue.setString("Fortunately it/only says to ask");
			if (select) { speech = true; select = 0; act = 26; }
			break;
		case 26:
			dialogue.setString("I guess that/means it's time/for part three!");
			if (select) { eyePos = eyeEnum::sus; book = true; speech = true; select = 0; act++; }
			break;
		case 27:
			dialogue.setString("Step 3: Put on/nice clothes to/show you care");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 28:
			dialogue.setString("...");
			if (select) { book = false; speech = true; select = 0; act++; }
			break;
		case 29:
			DatingStart.stop(1);
			dialogue.setString("Wait a second...");
			if (select) { speech = true; select = 0; DatingTense.play(3, -1); act++; }
			break;
		case 30:
			dialogue.setString("'Wear clothing'");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 31:
			dialogue.setString("You're wearing/clothing right/now!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 32:
			dialogue.setString("Not only that...");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 33:
			dialogue.setString("Earlier today/you were also/wearing/clothing!!!");
			if (select) { eyePos = eyeEnum::blush; speech = true; select = 0; act++; }
			break;
		case 34:
			dialogue.setString("No!!!/Could it be!?!?");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 35:
			dialogue.setString("You've wanted/to date me from/the very/beginning?!?!");
			if (select) { dialogue.setString(""); speech = true; select = 0; act++; }
			break;
		case 36:
			choice.setString("yes");
			dialogue.setString("no");
			heart.render(heart_x, heart_y, NULL);
			if (select == 1) {
				act++; DatingTense.stop(3);
				eyePos = eyeEnum::suprise; datingHUD = false;
			} // also play shake sfx
			else if (select == 2) {
				act++; DatingTense.stop(3); heart_y = ypos1;
				eyePos = eyeEnum::suprise; datingHUD = false;
			}
			speech = true; break;
		case 37:
			Oof.play(6, 0);
			std::cout << "adam is a big boi shaek";
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer); SDL_Delay(200);
			SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
			for (int i = 1; i < 40; i *= -2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			for (int i = 40; abs(i) > 0; i *= -0.5)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			} SDL_Delay(200);
			switch (select)
			{
			case 1:
				act = 1038; break;
			case 2:
				act = 2038; break;
			}
			select = 0; break;
		case 1038: // yes choice branch
			dialogue.setString("NO!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 1039:
			dialogue.setString("You planned/it all!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 1040:
			dialogue.setString("You're way/better at/dating than I/am");
			if (select) { speech = true; select = 0; act = 41; }
			break;
		case 2038: // no choice branch Music stop datingHUD away Shake
			eyePos = eyeEnum::sus;
			dialogue.setString("Despite that you/chose to wear/clothing today/of all days");
			if (select) { eyePos = eyeEnum::blush; speech = true; select = 0; act++; }
			break;
		case 2039:
			dialogue.setString("Was your/interest in/me...");
			if (select) { eyePos = eyeEnum::twinkle; speech = true; select = 0; act++; }
			break;
		case 2040:
			dialogue.setString("Predestined!?!?");
			if (select) { eyePos = eyeEnum::suprise; speech = true; select = 0; act = 41; }
			break;
		case 41:
			dialogue.setString("N-NOOOO!!!/Your/DATING POWER!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 42:
			Oof.play(6, 0);
			std::cout << "adam is a big boi shaek";
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer); SDL_Delay(200);
			SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
			for (int i = 1; i < 40; i *= -2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 4;
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			for (int i = 40; abs(i) > 0; i *= -0.5)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 4;
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
			eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260, 175, &eyeClip[eyePos]);
			SDL_RenderPresent(renderer);
			for (int i = 0; i < 10; i++)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260, 175, &eyeClip[eyePos]);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 4;
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			SDL_RenderClear(renderer);
			eyePos = eyeEnum::dare;
			SDL_Delay(500); act++; break;
		case 43:
			dialogue.setString("Nyeh");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 44:
			dialogue.setString("Nyeh heh heh");
			if (select) { speech = true; select = 0; DatingFight.play(4, -1); tension = true; act++; }
			break;
		case 45: // DatingFight tension graph and music
			dialogue.setString("Don't think/you've bested/me yet!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 46:
			dialogue.setString("I the great/Adam!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 47:
			dialogue.setString("Have never been/beaten at/dating and I/never will!!!");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 48:
			dialogue.setString("I can easily/keep up with/you!");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 49:
			dialogue.setString("You see,/I, too, can wear/clothing!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 50:
			dialogue.setString("In fact!");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 51:
			dialogue.setString("I always wear my/SPECIAL CLOTHES/underneath my/regular clothes");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 52:
			dialogue.setString("Just in case/somebody happens/to ask me on a/date!");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 53:
			dialogue.setString("Behold!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 54:
			for (int i = 0; i < 900; i+=2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				SDL_RenderPresent(renderer);
				SDL_Delay(1);
			}
			for (int i = 0; i < 900; i+=2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 900 - i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 1160 - i, 175, &eyeClip[eyePos]);
				coolHat.render(279 + 900 - i, hatPos);
				coolShirt.render(240 + 900 - i, 246, NULL);
				SDL_RenderPresent(renderer);
				SDL_Delay(1);
			}
			eyePos = eyeEnum::dare;
			coolDude = true; act++;
			break; // ADAM MOVE OUT!!!
		case 55:
			dialogue.setString("Nyeh!/What do you/think of my/secret style?");
			if (select) { dialogue.setString(""); speaking = false; speech = true; select = 0; act++; }
			break;
		case 56:
			choice.setString("I love it");
			dialogue.setString("I hate it");
			heart.render(heart_x, heart_y, NULL);
			if (select == 1) { act = 1057; eyePos = eyeEnum::suprise; }
			else if (select == 2) { act = 2057; heart_y = ypos1; eyePos = eyeEnum::suprise; }
			speech = true; select = 0; speaking = true;
			break;
		case 1057: // i love it choice branch
			dialogue.setString("NO!!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 1058:
			dialogue.setString("A genuine/compliment!!");
			if (select) { speech = true; select = 0; act = 59; }
			break;
		case 2057: // i hate it choice branch
			dialogue.setString("NO!!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 2058:
			dialogue.setString("Your honesty!!/It shows how/much you really/care!!!");
			if (select) { speech = true; select = 0; act = 59; }
			break;
		case 59:
			Oof.play(6, 0);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer); SDL_Delay(200);
			SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
			for (int i = 1; i < 40; i *= -2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 3;
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			for (int i = 40; abs(i) > 0; i *= -0.5)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 5;
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
			eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260, 175, &eyeClip[eyePos]);
			coolHat.render(279, hatPos, NULL);
			coolShirt.render(240, 246, NULL);
			SDL_RenderPresent(renderer);
			for (int i = 0; i < 10; i++)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260, 175, &eyeClip[eyePos]);
				coolHat.render(279, hatPos, NULL);
				coolShirt.render(240, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 5;
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			SDL_RenderClear(renderer);
			act++; tension = false; datePower = true; eyePos = eyeEnum::sus;
			break; // music stop tension away DATE POWER
		case 60:
			dialogue.setString("However...");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 61:
			dialogue.setString("You don't truly/understand the/HIDDEN POWER/of this outfit!!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 62:
			dialogue.setString("Therefore...");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 63:
			dialogue.setString("What you just/said is/invalid!!!");
			if (bar.w > 110)
			{
				bar.w--;
			}
			else
				if (select) { speech = true; select = 0; act++; }
			break;
		case 64:
			dialogue.setString("This date won't/escalate any/further!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 65:
			dialogue.setString("... Unless/you find my/secret!!");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 66:
			dialogue.setString("But that won't/happen!");
			if (select) { dialogue.setString(""); datePower = false; speech = true; speaking = false; select = 0; act++; }
			break;
		case 67:
			choice.setString("Move and inspect with space");
			switch (found)
			{
			case clothing::shirt:
				speaking = true; speech = true; select = 0; act = 1068;
				break;
			case clothing::shoulder:
				eyePos = eyeEnum::sus;
				speaking = true; speech = true; select = 0; act = 2068;
				break;
			case clothing::hat:
				eyePos = eyeEnum::sus;
				speaking = true; speech = true; select = 0; act = 3068;
				break;
			case clothing::watch:
				eyePos = eyeEnum::sus;
				speaking = true; speech = true; select = 0; act = 4068;
				break;
			case clothing::hand:
				eyePos = eyeEnum::blush;
				speaking = true; speech = true; select = 0; act = 5068;
				break;
			}
			break;
		case 1068:
			dialogue.setString("This shirt/didn't/originally say/'cool' but I/improved it");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 1069:
			dialogue.setString("Expert tip: All/articles of/clothing can be/improved this/way!!");
			if (select) { dialogue.setString(""); found = NULL; speech = true; speaking = false; select = 0; act = 67; }
			break;
		case 2068:
			dialogue.setString("I see. I see.");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 2069:
			dialogue.setString("You like/caressing my/biceps with a/floating heart");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 2070:
			dialogue.setString("But who/doesn't!?");
			if (select) { eyePos = eyeEnum::normal; dialogue.setString(""); found = NULL; speech = true; speaking = false; select = 0; act = 67; }
			break;
		case 3068:
			dialogue.setString("My hat...?");
			if (select) { speech = true; select = 0; act = 69; }
			break;
		case 4068:
			dialogue.setString("They say watches/hold the secret/to time and/space...");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 4069:
			dialogue.setString("But mine just/has pictures/of cats...");
			if (select) { eyePos = eyeEnum::normal; dialogue.setString(""); found = NULL; speech = true; speaking = false; select = 0; act = 67; }
			break;
		case 5068:
			dialogue.setString("Holding my hand/so I'll tell you/the answer...");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 5069:
			dialogue.setString("No!!/I must resist!");
			if (select) { eyePos = eyeEnum::normal; dialogue.setString(""); found = NULL; speech = true; speaking = false; select = 0; act = 67; }
			break;
		case 69:
			dialogue.setString("My hat.");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 70:
			dialogue.setString("My hat!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 71:
			dialogue.setString("Nyeh heh heh!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 72:
			for (int i = 0; i < 150; i++)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260, 175, &eyeClip[eyePos]);
				present.render(330, 40);
				coolHat.render(279, 19 - i, NULL);
				coolShirt.render(240, 246, NULL);
				SDL_RenderPresent(renderer);
				SDL_Delay(20);
			}
			hatPos = -131;
			SDL_Delay(1000);
			act++;
			break;
		case 73:
			dialogue.setString("W-Well then.../You found my/secret!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 74:
			dialogue.setString("I suppose I have/no choice!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 75:
			dialogue.setString("It's a/present...");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 76:
			dialogue.setString("A present/j-just for you!");
			if (select) { dialogue.setString(""); heart_x = xpos; heart_y = ypos1; speech = true; select = 0; act++; }
			break;
		case 77:
			choice.setString("Open it");
			dialogue.setString("Don't");
			heart.render(heart_x, heart_y, NULL);
			if (select == 1) { open = true; select = 0; act = 82; }
			else if (select == 2) { eyePos = eyeEnum::blush; select = 0; heart_y = ypos1; act = 2078; }
			speech = true; break;
		case 2078: //Don't choice branch
			dialogue.setString("You can't even/bring yourself/to harm my/delicate/wrapping??");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 2079:
			dialogue.setString("N-NO!/That technique!!");
			if (select) { speech = true; eyePos = eyeEnum::suprise; select = 0; act++; }
			break;
		case 2080:
			dialogue.setString("It's too strong!");
			if (select) { speech = true; eyePos = eyeEnum::dare; select = 0; act++; } //scowl eyePos?!??!
			break;
		case 2081:
			dialogue.setString("Counterattack!/I'll open the/present myself!!");
			if (select) { eyePos = eyeEnum::normal; open = true; speech = true; select = 0; act = 82; }
			break;
		case 82:
			dialogue.setString("Do you know/what this is?");
			if (select) { dialogue.setString(""); speech = true; select = 0; act++; }
			break;
		case 83:
			choice.setString("Of course");
			dialogue.setString("No idea");
			heart.render(heart_x, heart_y, NULL);
			if (select == 1) { select = 0; act = 1084; }
			else if (select == 2) { select = 0; heart_y = ypos1; act = 2084; }
			speech = true; break;
		case 1084: // Of course choice branch
			dialogue.setString("'Spaghetti'");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 1085:
			dialogue.setString("That's what/you're thinking/isn't it?");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 1086:
			dialogue.setString("Right!");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 1087:
			dialogue.setString("But oh so/wrong!!!");
			if (select) { speech = true; select = 0; act = 88; }
			break;
		case 2084:
			dialogue.setString("Nyeh heh heh!/That's right!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 2085:
			dialogue.setString("You have no/idea!!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 2086:
			dialogue.setString("Though this/appears to be/spaghetti...");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act = 88; }
			break;
		case 88:
			for (int i = 0; i < 70; i += 1)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260, 175, &eyeClip[eyePos]);
				spaghett.render(330, 40);
				coolHat.render(279, hatPos, NULL);
				coolShirt.render(240, 246, NULL);
				DATE_POWER.render(i, 20);
				datePowerFrame.render(i, 50);
				datePowerBar.render(i + 3, 53, &bar);
				dialogueBox.render(SCREEN_WIDTH / 2, SCREEN_HEIGHT - dialogueBox.getHeight());
				SDL_RenderPresent(renderer);
				SDL_Delay(1);
			}
			datePower = true;
			act++; break;
		case 89:
			dialogue.setString("This ain't any/plain ol' pasta!");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 90:
			dialogue.setString("This is an/artisan's work!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 91:
			dialogue.setString("Silken/spaghetti,/finely aged in/an oaken cask...");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 92:
			dialogue.setString("Then cooked by/me, master/chef Adam!!");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 93:
			dialogue.setString("Human!!!/It's time to/end this!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 94:
			dialogue.setString("There's no way/this can go/any further!");
			if (select) { dialogue.setString(""); speech = true; select = 0; act++; }
			break;
		case 95:
			choice.setString("Eat it");
			dialogue.setString("Refuse");
			heart.render(heart_x, heart_y, NULL);
			if (select == 1) { select = 0; act = 1096; }
			else if (select == 2) { eyePos = eyeEnum::sus; select = 0; heart_y = ypos1; act = 2096; }
			speech = true; break;
		case 1096:
			choice.setString("*You take a small bite");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 1097:
			choice.setString("*Your face reflexively/scrunches up");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 1098:
			choice.setString("*The taste is/indescribable...");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 1099:
			dialogue.setString("What a/passionate/expression!!!");
			if (select) { eyePos = eyeEnum::blush; speech = true; select = 0; act++; }
			break;
		case 1100:
			dialogue.setString("You must really/love my cooking!");
			if (select) { eyePos = eyeEnum::twinkle; speech = true; select = 0; act++; }
			break;
		case 1101:
			dialogue.setString("And by extension/me!!!");
			if (select) { eyePos = eyeEnum::suprise; speech = true; select = 0; act++; }
			break;
		case 1102:
			dialogue.setString("Maybe even more/than I do!!");
			if (select) { speech = true; select = 0; act = 103; } // set act!
			break;
		case 2096:
			dialogue.setString("You mean...");
			if (select) { eyePos = eyeEnum::blush; speech = true; select = 0; act++; }
			break;
		case 2097:
			dialogue.setString("You're letting/me have it/instead?");
			if (select) { eyePos = eyeEnum::twinkle; speech = true; select = 0; act++; }
			break;
		case 2098:
			dialogue.setString("Because you/know how much/I love pasta...");
			if (select) { eyePos = eyeEnum::suprise; speech = true; select = 0; act++; }
			break;
		case 2099:
			dialogue.setString("Impossible!!/Taking my/present and/turning it/around on me!");
			if (select) { speech = true; select = 0; act = 103; }
			break;
		case 103: // also shake
			Oof.play(6, 0);
			dialogue.setString("AUGH!!!");
			for (int i = 1; i < 40; i *= -2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				bubble.render(500 + i, 50, NULL);
				speechBubble.x += i;
				dialogue.type(speechBubble, false, 2);
				speechBubble.x = 540;
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 8;
				FADE.setAlpha(alpha);
				FADE.render(0, 0);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			for (int i = 40; abs(i) > 0; i *= -0.5)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				bubble.render(500 + i, 50, NULL);
				speechBubble.x += i;
				dialogue.type(speechBubble, false, 2);
				speechBubble.x = 540;
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 8;
				FADE.setAlpha(alpha);
				FADE.render(0, 0);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}

			SDL_Delay(100);
			Oof.play(6, 0);
			dialogue.setString("URRRGH!!!");
			for (int i = 1; i < 80; i *= -2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				bubble.render(500 + i, 50, NULL);
				speechBubble.x += i;
				dialogue.type(speechBubble, false, 2);
				speechBubble.x = 540;
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 8;
				FADE.setAlpha(alpha);
				FADE.render(0, 0);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			for (int i = 80; abs(i) > 0; i *= -0.5)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				bubble.render(500 + i, 50, NULL);
				speechBubble.x += i;
				dialogue.type(speechBubble, false, 2);
				speechBubble.x = 540;
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 8;
				FADE.setAlpha(alpha);
				FADE.render(0, 0);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}

			SDL_Delay(100);
			Oof.play(6, 0);
			DatingFight.stop(4);
			dialogue.setString("NOOOOOOOOOOOO!!!!");
			for (int i = 1; i < 120; i *= -2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				bubble.render(500 + i, 50, NULL);
				speechBubble.x += i;
				dialogue.type(speechBubble, false, 2);
				speechBubble.x = 540;
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 8;
				FADE.setAlpha(alpha);
				FADE.render(0, 0);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			for (int i = 120; abs(i) > 0; i *= -0.5)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				bubble.render(500 + i, 50, NULL);
				speechBubble.x += i;
				dialogue.type(speechBubble, false, 2);
				speechBubble.x = 540;
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 8;
				FADE.setAlpha(alpha);
				FADE.render(0, 0);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}

			SDL_Delay(100);
			Oof.play(6, 0);
			for (int i = 1; i < 200; i *= -2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				bubble.render(500 + i, 50, NULL);
				speechBubble.x += i;
				dialogue.type(speechBubble, false, 2);
				speechBubble.x = 540;
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 8; alpha += 15;
				FADE.setAlpha(alpha);
				FADE.render(0, 0);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			for (int i = 200; abs(i) > 0; i *= -0.5)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				bubble.render(500 + i, 50, NULL);
				speechBubble.x += i;
				dialogue.type(speechBubble, false, 2);
				speechBubble.x = 540;
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				DATE_POWER.render(70, 20);
				datePowerFrame.render(70, 50, NULL);
				datePowerBar.render(73, 53, &bar);
				bar.w += 8; alpha += 16;
				FADE.setAlpha(alpha);
				FADE.render(0, 0);
				SDL_RenderPresent(renderer); SDL_Delay(100);
			}
			FADE.setAlpha(255);
			FADE.render(0, 0);
			SDL_RenderClear(renderer);
			dialogue.setString("");
			datePower = false;
			white = true; speech = true; act++;
			SDL_Delay(1500);
			break;
		case 104:
			finalText.setString("Human./It's clear now");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 105:
			finalText.setString("You're madly in/love with me");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 106:
			finalText.setString("Everything you do/Everything you say");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 107:
			finalText.setString("It's all been/for my sake");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 108:
			finalText.setString("Human. I want/you to be/happy, too");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 109:
			finalText.setString("It's time for/me to express/my feelings");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 110:
			finalText.setString("It's time that/I told you");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 111:
			finalText.setString("I Adam...");
			if (select) { finalText.setString(""); white = false; speech = true; select = 0; act++; }
			break;
		case 112:
			dialogue.setString("I...");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 113:
			dialogue.setString("Um...");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 114:
			dialogue.setString("Boy, is it hot/in here, or is/it just me??");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 115:
			dialogue.setString("...");
			if (select) { eyePos = eyeEnum::sheep; speech = true; select = 0; act++; }
			break;
		case 116:
			dialogue.setString("Oh, shoot");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 117:
			dialogue.setString("Human, I...");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 118:
			dialogue.setString("I'm sorry");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 119:
			dialogue.setString("I don't like/you the way/you like me");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 120:
			dialogue.setString("Romantically/I mean...");
			if (select) { eyePos = eyeEnum::suprise; speech = true; select = 0; act++; }
			break;
		case 121:
			dialogue.setString("I mean, I tried/very hard to!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 122:
			dialogue.setString("I thought/that because/you flirted/with me...");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 123:
			dialogue.setString("That I was/supposed to/go on a date/with you!");
			if (select) { eyePos = eyeEnum::blush; speech = true; select = 0; act++; }
			break;
		case 124:
			dialogue.setString("Then, on the/date, feelings/would blossom/forth!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 125:
			dialogue.setString("I would be/able to match/your passion/for me!");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 126:
			dialogue.setString("But alas.../I, the great/Adam...");
			if (select) { eyePos = eyeEnum::sheep; speech = true; select = 0; act++; }
			break;
		case 127:
			dialogue.setString("Have failed");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 128:
			dialogue.setString("I feel just/the same as/before");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 129:
			dialogue.setString("And instead, by/dating you...");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 130:
			dialogue.setString("I have only/drawn you/deeper...");
			if (select) { eyePos = eyeEnum::suprise; speech = true; select = 0; act++; }
			break;
		case 131:
			dialogue.setString("Into your/intense love/for me!");
			if (select) { eyePos = eyeEnum::sheep; speech = true; select = 0; act++; }
			break;
		case 132:
			dialogue.setString("A dark prison/of passion, with/no escape...");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 133:
			dialogue.setString("How could I/have done this/to my dear/friend...?");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 134:
			dialogue.setString("...");
			if (select) { eyePos = eyeEnum::suprise; speech = true; select = 0; act++; }
			break;
		case 135:
			dialogue.setString("NO! WAIT!/That's wrong!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 136:
			dialogue.setString("I can't fail/at anything!!!");
			if (select) { eyePos = eyeEnum::dare; speech = true; select = 0; act++; }
			break;
		case 137:
			dialogue.setString("Human!!!/I'll help you/through these/trying times!!!");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 138:
			dialogue.setString("I'll keep being/your cool/friend...");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 139:
			dialogue.setString("And act like/this all never/happened...");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 140:
			dialogue.setString("After all, you/are very great!");
			if (select) { eyePos = eyeEnum::sheep; speech = true; select = 0; act++; }
			break;
		case 141:
			dialogue.setString("It would be/tragic to lose/your friendship");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 142:
			dialogue.setString("So please...");
			if (select) { eyePos = eyeEnum::sheep; speech = true; select = 0; act++; }
			break;
		case 143:
			dialogue.setString("Don't cry/over me");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 144:
			dialogue.setString("Someday you'll/find someone as/great as me");
			if (select) { eyePos = eyeEnum::sus; speech = true; select = 0; act++; }
			break;
		case 145:
			dialogue.setString("Well, no./That's not true");
			if (select) { eyePos = eyeEnum::normal; speech = true; select = 0; act++; }
			break;
		case 146:
			dialogue.setString("But I'll help/you settle for/second best!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 147:
			dialogue.setString("Nyeh heh heh/heh heh!!!");
			if (select) { speech = true; select = 0; act++; }
			break;
		case 148:
			for (int i = 0; i < 900; i += 2)
			{
				SDL_RenderClear(renderer);
				Adam.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + i, SCREEN_HEIGHT - Adam.getHeight() - dialogueBox.getHeight());
				eyes.render((SCREEN_WIDTH - Adam.getWidth()) / 2 + 260 + i, 175, &eyeClip[eyePos]);
				spaghett.render(330 + i, 40);
				coolHat.render(279 + i, hatPos, NULL);
				coolShirt.render(240 + i, 246, NULL);
				dialogueBox.render((SCREEN_WIDTH - dialogueBox.getWidth()) / 2, SCREEN_HEIGHT - dialogueBox.getHeight() - 1, NULL);
				SDL_RenderPresent(renderer); SDL_Delay(1);
			}
			dialogue.setString("");
			act++;
			break;
		case 149:
			break;
		default:
			break;
		}

		/*----------------------------------------*/

		SDL_RenderPresent(renderer);

		startTime = endTime;
		endTime = SDL_GetTicks();
	}

	SDL_DestroySemaphore(dataLock);
	dataLock = NULL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	IMG_Quit();
	SDL_Quit();

	return 0;
}