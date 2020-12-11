#include <stdio.h>

#define ANSI(str) "\x1b[" str

#define ANSI_RESET ANSI("0m")

#define ANSI_BOLD_ON ANSI("1m")
#define ANSI_BOLD_OFF ANSI("22m")
#define ANSI_FAINT_ON ANSI("2m")
#define ANSI_FAINT_OFF ANSI("22m") // same as ANSI_BOLD_OFF
#define ANSI_ITALIC_ON ANSI("3m")
#define ANSI_ITALIC_OFF ANSI("23m")
#define ANSI_UNDERSCORE_ON ANSI("4m")
#define ANSI_UNDERSCORE_OFF ANSI("24m")
#define ANSI_BLINK_ON ANSI("5m")
#define ANSI_BLINK_OFF ANSI("25m")
#define ANSI_INVERT_ON ANSI("7m")
#define ANSI_INVERT_OFF ANSI("27m")
#define ANSI_CROSSED_OUT_ON ANSI("9m")
#define ANSI_CROSSED_OUT_OFF ANSI("29m")

#define ANSI_FG_DEFAULT ANSI("39m")

#define ANSI_FG_BLACK ANSI("30m")
#define ANSI_FG_RED ANSI("31m")
#define ANSI_FG_GREEN ANSI("32m")
#define ANSI_FG_YELLOW ANSI("33m")
#define ANSI_FG_BLUE ANSI("34m")
#define ANSI_FG_MAGENTA ANSI("35m")
#define ANSI_FG_CYAN ANSI("36m")
#define ANSI_FG_WHITE ANSI("37m")

#define ANSI_FG_LIGHT_BLACK ANSI("90m")
#define ANSI_FG_LIGHT_RED ANSI("91m")
#define ANSI_FG_LIGHT_GREEN ANSI("92m")
#define ANSI_FG_LIGHT_YELLOW ANSI("93m")
#define ANSI_FG_LIGHT_BLUE ANSI("94m")
#define ANSI_FG_LIGHT_MAGENTA ANSI("95m")
#define ANSI_FG_LIGHT_CYAN ANSI("96m")
#define ANSI_FG_LIGHT_WHITE ANSI("97m")

#define ANSI_FG_8BIT(color) ANSI("38;5;" color "m")
#define ANSI_FG_24BIT(color) ANSI("38;2;" color "m")

#define ANSI_BG_DEFAULT ANSI("49m")

#define ANSI_BG_BLACK ANSI("40m")
#define ANSI_BG_RED ANSI("41m")
#define ANSI_BG_GREEN ANSI("42m")
#define ANSI_BG_YELLOW ANSI("43m")
#define ANSI_BG_BLUE ANSI("44m")
#define ANSI_BG_MAGENTA ANSI("45m")
#define ANSI_BG_CYAN ANSI("46m")
#define ANSI_BG_WHITE ANSI("47m")

#define ANSI_BG_LIGHT_BLACK ANSI("100m")
#define ANSI_BG_LIGHT_RED ANSI("101m")
#define ANSI_BG_LIGHT_GREEN ANSI("102m")
#define ANSI_BG_LIGHT_YELLOW ANSI("103m")
#define ANSI_BG_LIGHT_BLUE ANSI("104m")
#define ANSI_BG_LIGHT_MAGENTA ANSI("105m")
#define ANSI_BG_LIGHT_CYAN ANSI("106m")
#define ANSI_BG_LIGHT_WHITE ANSI("107m")

#define ANSI_BG_8BIT(color) ANSI("48;5;" color "m")
#define ANSI_BG_24BIT(color) ANSI("48;2;" color "m")

#define NL ANSI_RESET "\n"
#define RST ANSI_RESET
#define SEP RST " "
#define SEP_FG ANSI_FG_DEFAULT " "
#define SEP_BG ANSI_BG_DEFAULT " "

int main(int argc, char *args[]) {
	printf(ANSI_BOLD_ON "===== ANSI debug =====\n" RST);
	printf(ANSI_BOLD_ON "------- Styles -------\n" RST);
	printf("Style: "
		   ANSI_BOLD_ON "Bold" ANSI_BOLD_OFF " "
		   ANSI_ITALIC_ON "Italic" ANSI_ITALIC_OFF " "
		   ANSI_UNDERSCORE_ON "Underscore" ANSI_UNDERSCORE_OFF " "
		   ANSI_BLINK_ON "Blink" ANSI_BLINK_OFF " "
		   ANSI_INVERT_ON "Invert" ANSI_INVERT_OFF " "
		   ANSI_CROSSED_OUT_ON "Crossed-out" ANSI_CROSSED_OUT_OFF
		   "\n");

	printf(ANSI_BOLD_ON "\n---- 3/4-bit color ---\n" RST);

	printf(ANSI_BOLD_ON "FG Color (normal): " RST
		   ANSI_FG_BLACK "Black" SEP_FG
		   ANSI_FG_RED "Red" SEP_FG
		   ANSI_FG_GREEN "Green" SEP_FG
		   ANSI_FG_YELLOW "Yellow" SEP_FG
		   ANSI_FG_BLUE "Blue" SEP_FG
		   ANSI_FG_MAGENTA "Magenta" SEP_FG
		   ANSI_FG_CYAN "Cyan" SEP_FG
		   ANSI_FG_WHITE "White" SEP_FG
		   ANSI_FG_DEFAULT "Default" NL);
	printf(ANSI_BOLD_ON "FG Color (bright): " RST
		   ANSI_FG_LIGHT_BLACK "Black" SEP_FG
		   ANSI_FG_LIGHT_RED "Red" SEP_FG
		   ANSI_FG_LIGHT_GREEN "Green" SEP_FG
		   ANSI_FG_LIGHT_YELLOW "Yellow" SEP_FG
		   ANSI_FG_LIGHT_BLUE "Blue" SEP_FG
		   ANSI_FG_LIGHT_MAGENTA "Magenta" SEP_FG
		   ANSI_FG_LIGHT_CYAN "Cyan" SEP_FG
		   ANSI_FG_LIGHT_WHITE "White" SEP_FG
		   ANSI_FG_DEFAULT "Default" NL);

	printf(ANSI_BOLD_ON "BG Color (normal): " RST
		   ANSI_BG_BLACK "Black" SEP_BG
		   ANSI_FG_BLACK // for contrast
		   ANSI_BG_RED "Red" SEP_BG
		   ANSI_BG_GREEN "Green" SEP_BG
		   ANSI_BG_YELLOW "Yellow" SEP_BG
		   ANSI_BG_BLUE "Blue" SEP_BG
		   ANSI_BG_MAGENTA "Magenta" SEP_BG
		   ANSI_BG_CYAN "Cyan" SEP_BG
		   ANSI_BG_WHITE "White"
		   ANSI_BG_DEFAULT ANSI_FG_DEFAULT " Default" NL);
	printf(ANSI_BOLD_ON "BG Color (bright): " RST
		   ANSI_BG_LIGHT_BLACK "Black" SEP_BG
		   ANSI_FG_BLACK // for contrast
		   ANSI_BG_LIGHT_RED "Red" SEP_BG
		   ANSI_BG_LIGHT_GREEN "Green" SEP_BG
		   ANSI_BG_LIGHT_YELLOW "Yellow" SEP_BG
		   ANSI_BG_LIGHT_BLUE "Blue" SEP_BG
		   ANSI_BG_LIGHT_MAGENTA "Magenta" SEP_BG
		   ANSI_BG_LIGHT_CYAN "Cyan" SEP_BG
		   ANSI_BG_LIGHT_WHITE "White"
		   ANSI_BG_DEFAULT ANSI_FG_DEFAULT " Default" NL);

	printf(ANSI_BOLD_ON "\n----- 8-bit color ----\n" RST);
	printf(ANSI_BOLD_ON "FG Color (standard): " RST);
	for (int i = 0; i < 8; i++) {
		printf(ANSI_FG_8BIT("%d") "%d" SEP_FG, i, i);
	}
	printf("\n");
	printf(ANSI_BOLD_ON "FG Color (high-intensity): " RST);
	for (int i = 8; i < 16; i++) {
		printf(ANSI_FG_8BIT("%d") "%d" SEP_FG, i, i);
	}
	printf("\n");
	printf(ANSI_BOLD_ON "FG Color (216):" RST);
	for (int i = 16; i < 232;) {
		printf("\n    ");
		for (int col = 0; col < 36; col++) {
			printf(ANSI_FG_8BIT("%d") "%3d" SEP_FG, i, i);
			i++;
		}
	}
	printf("\n");
	printf(ANSI_BOLD_ON "FG Color (grayscale): " RST);
	for (int i = 232; i < 256; i++) {
		printf(ANSI_FG_8BIT("%d") "%3d" SEP_FG, i, i);
	}
	printf("\n");

	printf(ANSI_BOLD_ON "BG Color (standard): " RST);
	for (int i = 0; i < 8; i++) {
		printf(ANSI_BG_8BIT("%d") "%d" SEP_BG, i, i);
	}
	printf("\n");
	printf(ANSI_BOLD_ON "BG Color (high-intensity): " RST);
	for (int i = 8; i < 16; i++) {
		printf(ANSI_BG_8BIT("%d") "%d" SEP_BG, i, i);
	}
	printf("\n");
	printf(ANSI_BOLD_ON "BG Color (216):" RST);
	for (int i = 16; i < 232;) {
		printf("\n    ");
		for (int col = 0; col < 36; col++) {
			if (col >= 18)
				printf(ANSI_FG_8BIT("232")); // using ANSI_FG_BLACK is an edge case
			printf(ANSI_BG_8BIT("%d") "%3d" SEP, i, i);
			i++;
		}
	}
	printf("\n");
	printf(ANSI_BOLD_ON "BG Color (grayscale): " RST);
	for (int i = 232; i < 256; i++) {
		if (i >= 244)
			printf(ANSI_FG_8BIT("232")); // using ANSI_FG_BLACK is an edge case
		printf(ANSI_BG_8BIT("%d") "%3d" SEP, i, i);
	}
	printf("\n");

	printf(ANSI_BOLD_ON "\n----- 24-bit color ----\n" RST);
	char *rgb_colors[6] = {
			"0;0;0",
			"128;128;128",
			"255;255;255",
			"255;0;0",
			"0;255;0",
			"0;0;255",
	};

	printf(ANSI_BOLD_ON "FG Color (RGB): " RST);
	for (int i = 0; i < 6;) {
		printf("\n    ");
		for (int col = 0; col < 16 && i < 6; col++) {
			printf(ANSI_FG_24BIT("%s") "%11s" SEP_FG, rgb_colors[i], rgb_colors[i]);
			i++;
		}
	}
	printf("\n");

	printf(ANSI_BOLD_ON "BG Color (RGB): " RST);
	for (int i = 0; i < 6;) {
		printf("\n    ");
		for (int col = 0; col < 16 && i < 6; col++) {
			printf(ANSI_BG_24BIT("%s") "%11s" SEP_BG, rgb_colors[i], rgb_colors[i]);
			i++;
		}
	}
	printf("\n");

	printf(ANSI_BOLD_ON "\n------- Special ------\n" RST);
	printf(ANSI_BOLD_ON "Inverted (3-bit): " RST
		   ANSI_FG_RED ANSI_BG_GREEN "Red on green "
		   ANSI_INVERT_ON "Green on red" NL);
	printf(ANSI_BOLD_ON "Inverted (8-bit): " RST
		   ANSI_FG_8BIT("40") ANSI_BG_8BIT("170") "Green on pink"
		   ANSI_INVERT_ON "Pink on green" NL);
	printf(ANSI_BOLD_ON "Inverted (24-bit): " RST
		   ANSI_FG_24BIT("160;32;240") ANSI_BG_24BIT("111;240;31") "Purple on green "
		   ANSI_INVERT_ON "Green on purple" NL);

	printf(ANSI_BOLD_ON "\n------ Edge cases -----\n" RST);

	printf(ANSI_BOLD_ON "8-bit foreground (blueish), 3-bit background (red):\n" RST);
	printf("    " ANSI_FG_8BIT("63") ANSI_BG_RED "Edge case" NL);
	printf("    (when --stylesheet is enabled, the foreground will be put in a style attribute, and the 3-bit "
		   "background class will end up in that style attribute and not the class attribute)\n");
	printf(ANSI_BOLD_ON "Double-invert: " RST
		   ANSI_FG_RED ANSI_BG_GREEN "Red on green "
		   ANSI_INVERT_ON "Green on red "
		   ANSI_INVERT_ON "Still green on red" NL);
	printf(ANSI_BOLD_ON "Inverted (mixed 8/3-bit): " RST
		   ANSI_FG_8BIT("92") ANSI_BG_GREEN "Purple on green "
		   ANSI_INVERT_ON "Green on purple" NL);
	printf(ANSI_BOLD_ON "Inverted (mixed 24/3-bit): " RST
		   ANSI_FG_GREEN ANSI_BG_24BIT("160;32;240") "Green on purple"
		   ANSI_INVERT_ON "Purple on green" NL);
	printf(ANSI_BOLD_ON "Inverted (mixed 24/8-bit): " RST
		   ANSI_FG_24BIT("160;32;240") ANSI_BG_8BIT("40") "Purple on green "
		   ANSI_INVERT_ON "Green on purple" NL);

	return 0;
}
