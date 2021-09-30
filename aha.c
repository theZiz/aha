/*
 The contents of this file are subject to the Mozilla Public License
 Version 1.1 (the "License"); you may not use this file except in
 compliance with the License. You may obtain a copy of the License at
 http://www.mozilla.org/MPL/

 Software distributed under the License is distributed on an "AS IS"
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 License for the specific language governing rights and limitations
 under the License.

 Alternatively, the contents of this file may be used under the terms
 of the GNU Lesser General Public license version 2 or later (LGPLv2+),
 in which case the provisions of LGPL License are applicable instead of
 those above.

 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , ziz_at_mailbox.org
*/
#define AHA_VERSION "0.5.1"
#define AHA_YEAR "2020"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// table for vt220 character set, see also
// https://whitefiles.org/b1_s/1_free_guides/fg2cd/pgs/c03b.htm
const char ansi_vt220_character_set[256][16] =
{
	"&#x2400;","&#x2401;","&#x2402;","&#x2403;","&#x2404;","&#x2405;","&#x2406;","&#x2407;", //00..07
	"&#x2408;","&#x2409;","&#x240a;","&#x240b;","&#x240c;","&#x240d;","&#x240e;","&#x240f;", //08..0f
	"&#x2410;","&#x2411;","&#x2412;","&#x2413;","&#x2414;","&#x2415;","&#x2416;","&#x2417;", //10..17
	"&#x2418;","&#x2419;","&#x241a;","&#x241b;","&#x241c;","&#x241d;","&#x241e;","&#x241f;", //18..1f
	" "       ,"!"       ,"\""      ,"#"       ,"$"       ,"%"       ,"&"       ,"'"       , //20..27
	"("       ,")"       ,"*"       ,"+"       ,","       ,"-"       ,"."       ,"/"       , //28..2f
	"0"       ,"1"       ,"2"       ,"3"       ,"4"       ,"5"       ,"6"       ,"7"       , //30..37
	"8"       ,"9"       ,":"       ,";"       ,"&lt;"    ,"="       ,"&gt;"    ,"?"       , //38..3f
	"@"       ,"A"       ,"B"       ,"C"       ,"D"       ,"E"       ,"F"       ,"G"       , //40..47
	"H"       ,"I"       ,"J"       ,"K"       ,"L"       ,"M"       ,"N"       ,"O"       , //48..4f
	"P"       ,"Q"       ,"R"       ,"S"       ,"T"       ,"U"       ,"V"       ,"W"       , //50..57
	"X"       ,"Y"       ,"Z"       ,"["       ,"\\"      ,"]"       ,"^"       ,"_"       , //58..5f
	"`"       ,"a"       ,"b"       ,"c"       ,"d"       ,"e"       ,"f"       ,"g"       , //60..67
	"h"       ,"i"       ,"j"       ,"k"       ,"l"       ,"m"       ,"n"       ,"o"       , //68..6f
	"p"       ,"q"       ,"r"       ,"s"       ,"t"       ,"u"       ,"v"       ,"w"       , //70..77
	"x"       ,"y"       ,"z"       ,"{"       ,"|"       ,"}"       ,"~"       ,"&#x2421;", //78..7f
	"&#x25c6;","&#x2592;","&#x2409;","&#x240c;","&#x240d;","&#x240a;","&#x00b0;","&#x00b1;", //80..87
	"&#x2400;","&#x240b;","&#x2518;","&#x2510;","&#x250c;","&#x2514;","&#x253c;","&#x23ba;", //88..8f
	"&#x23bb;","&#x2500;","&#x23bc;","&#x23bd;","&#x251c;","&#x2524;","&#x2534;","&#x252c;", //90..97
	"&#x2502;","&#x2264;","&#x2265;","&pi;    ","&#x2260;","&pound;" ,"&#x0095;","&#x2421;", //98..9f
	"&#x2588;","&#x00a1;","&#x00a2;","&#x00a3;"," "       ,"&yen;"   ," "       ,"&#x00a7;", //a0..a7
	"&#x00a4;","&#x00a9;","&#x00ba;","&#x00qb;"," "       ," "       ," "       ," "       , //a8..af
	"&#x23bc;","&#x23bd;","&#x00b2;","&#x00b3;","&#x00b4;","&#x00b5;","&#x00b6;","&#x00b7;", //b0..b7
	"&#x00b8;","&#x00b9;","&#x00ba;","&#x00bb;","&#x00bc;","&#x00bd;","&#x00be;","&#x00bf;", //b8..bf
	"&#x00c0;","&#x00c1;","&#x00c2;","&#x00c3;","&#x00c4;","&#x00c5;","&#x00c6;","&#x00c7;", //c0..c7
	"&#x00c8;","&#x00c9;","&#x00ca;","&#x00cb;","&#x00cc;","&#x00cd;","&#x00ce;","&#x00cf;", //c8..cf
	" "       ,"&#x00d1;","&#x00d2;","&#x00d3;","&#x00d4;","&#x00d5;","&#x00d6;","&#x0152;", //d0..d7
	"&#x00d8;","&#x00d9;","&#x00da;","&#x00db;","&#x00dc;","&#x0178;"," "       ,"&#x00df;", //d8..df
	"&#x00e0;","&#x00e1;","&#x00e2;","&#x00e3;","&#x00e4;","&#x00e5;","&#x00e6;","&#x00e7;", //e0..e7
	"&#x00e8;","&#x00e9;","&#x00ea;","&#x00eb;","&#x00ec;","&#x00ed;","&#x00ee;","&#x00ef;", //e8..ef
	" "       ,"&#x00f1;","&#x00f2;","&#x00f3;","&#x00f4;","&#x00f5;","&#x00f6;","&#x0153;", //f0..f7
	"&#x00f8;","&#x00f9;","&#x00fa;","&#x00fb;","&#x00fc;","&#x00ff;"," "       ,"&#x2588;", //f8..ff
};

int getNextChar(register FILE* fp)
{
	int c;
	if ((c = fgetc(fp)) != EOF)
		return c;

	perror("Error while parsing input");
	exit(EXIT_FAILURE);
}

typedef struct selem *pelem;
typedef struct selem {
	unsigned char digit[8];
	unsigned char digitcount;
	long int value;
	pelem next;
} telem;

pelem parseInsert(char* s)
{
	pelem firstelem=NULL;
	pelem momelem=NULL;

	unsigned char digit[8];
	unsigned char digitcount=0;
	long int value=0;

	int pos=0;
	for (pos=0;pos<1024;pos++)
	{
		if (s[pos]=='[')
			continue;
		if (s[pos]==';' || s[pos]==':' || s[pos]==0)
		{
			if (digitcount==0)
			{
				digit[0]=0;
				digitcount=1;
			}

			pelem newelem=(pelem)malloc(sizeof(telem));
			if (newelem==NULL)
			{
				perror("Failed to allocate memory for parseInsert()");
				exit(EXIT_FAILURE);
			}

			memcpy(newelem->digit, digit, sizeof(digit));
			newelem->digitcount=digitcount;
			newelem->value=value;
			newelem->next=NULL;

			if (momelem==NULL)
				firstelem=newelem;
			else
				momelem->next=newelem;
			momelem=newelem;

			digitcount=0;
			memset(digit,0,sizeof(digit));
			value=0;

			if (s[pos]==0)
				break;
		}
		else
		if (digitcount < sizeof(digit))
		{
			digit[digitcount]=s[pos]-'0';
			value=(value*10)+digit[digitcount];
			digitcount++;
		}
	}
	return firstelem;
}

void deleteParse(pelem elem)
{
	while (elem!=NULL)
	{
		pelem temp=elem->next;
		free(elem);
		elem=temp;
	}
}

typedef struct ColorScheme_t {
	char* fg_default;
	char* bg_default;
	char* colors[18]; // last two elements are fg, bg
	char* data;
} ColorScheme;

ColorScheme *parseColorScheme(const char* text)
{
	ColorScheme *scheme = malloc(sizeof(struct ColorScheme_t));
	scheme->data = strdup(text);
	int i = -2;

	char *token = scheme->data;
	while(1)
	{
		if(i == 16) // too many elements
		{
			free(scheme->data);
			free(scheme);
			return NULL;
		}

		if(i == -2)
			scheme->fg_default = token;
		else if(i == -1)
			scheme->bg_default = token;
		else
			scheme->colors[i] = token;

		i++;

		token += strcspn(token, ",");
		if(*token != 0)
			*token++ = 0; // insert null terminator
		else
			break; // we're at the end of the string
	}
	if(i < 16) // too few elements
	{
		free(scheme->data);
		free(scheme);
		return NULL;
	}

	scheme->colors[16] = scheme->fg_default;
	scheme->colors[17] = scheme->bg_default;

	return scheme;
}

void deleteColorScheme(ColorScheme *style)
{
	free(style->data);
	free(style);
}

void printHtml(char *text) {
	while(1) {
		switch(*text) {
			case '\0': return;

			case '"': printf("&quot;"); break;
			case '&': printf("&amp;"); break;
			case '<': printf("&lt;"); break;
			case '>': printf("&gt;"); break;

			default:
				putc(*text, stdout);
		}
		++text;
	}
}

struct Options {
	ColorScheme *scheme;
	char* filename;
	FILE *fp;
	int htop_fix;
	int iso;
	int line_break;
	int no_header;
	int stylesheet;
	char *title;
	int word_wrap;
	int no_xml;
	char* lang;
	char* css;
	int ignore_cr;
	char* bodystyle;
};

int divide (int dividend, int divisor){
	div_t result;
	result = div (dividend, divisor);
	return result.quot;
}

void make_rgb (int color_id, char str_rgb[12]){

	if (color_id < 16 || color_id > 255)
		return;
	if (color_id >= 232)
	{
		int index = color_id - 232;
		int grey = index * 256 / 24;
		sprintf(str_rgb, "%02x%02x%02x", grey, grey, grey);
		return;
	}
	int index_R = divide((color_id - 16), 36);
	int rgb_R;
	if (index_R > 0){
		rgb_R = 55 + index_R * 40;
	} else {
		rgb_R = 0;
	}

	int index_G = divide(((color_id - 16) % 36), 6);
	int rgb_G;
	if (index_G > 0){
		rgb_G = 55 + index_G * 40;
	} else {
		rgb_G = 0;
	}

	int index_B = ((color_id - 16) % 6);
	int rgb_B;
	if (index_B > 0){
		rgb_B = 55 + index_B * 40;
	} else {
		rgb_B = 0;
	}
	sprintf(str_rgb, "%02x%02x%02x", rgb_R, rgb_G, rgb_B);
}

#define VERSION_PRINTF_MAKRO \
	printf("\033[1;31mAnsi Html Adapter\033[0m Version "AHA_VERSION"\n");

const char* white_scheme =
		// Foreground, background
		"black,white,"
		// Black, red, green, yellow, blue, magenta, cyan, white
		"dimgray,red,green,olive,blue,purple,teal,gray,"
		// Bright black, red, green, yellow, blue, magenta, cyan, white
		"dimgray,red,green,olive,blue,purple,cyan,white";

const char* black_scheme =
		// Foreground, background
		"white,black,"
		// Black, red, green, yellow, blue, magenta, cyan, white
		"black,red,lime,yellow,#3333FF,fuchsia,aqua,white,"
		// Bright black, red, green, yellow, blue, magenta, cyan, white
		"black,red,lime,yellow,#3333FF,fuchsia,aqua,white";

const char* pink_scheme =
		// Foreground, background
		"black,pink,"
		// Black, red, green, yellow, blue, magenta, cyan, white
		"dimgray,red,green,olive,blue,purple,teal,gray,"
		// Bright black, red, green, yellow, blue, magenta, cyan, white
		"dimgray,red,green,olive,blue,purple,cyan,white";

struct Options parseArgs(int argc, char* args[])
{
	struct Options opts = (struct Options){
		.scheme = parseColorScheme(white_scheme),
		.filename = NULL,
		.fp = stdin,
		.htop_fix = 0,
		.iso = -1,
		.line_break = 0,
		.no_header = 0,
		.stylesheet = 0,
		.title = NULL,
		.word_wrap = 0,
		.no_xml = 0,
		.lang = NULL,
		.ignore_cr = 0,
		.bodystyle = NULL
	};

	//Searching Parameters
	for (int p = 1;p<argc;p++)
	{
		if ((strcmp(args[p],(char*)"--help")==0) || (strcmp(args[p],(char*)"-h")==0) || (strcmp(args[p],(char*)"-?")==0))
		{
			VERSION_PRINTF_MAKRO
			printf("\033[1maha\033[0m takes SGR-colored Input and prints W3C conform HTML-Code\n");
			printf("use: \033[1maha\033[0m <\033[4moptions\033[0m> [\033[4m-f file\033[0m]\n");
			printf("     \033[1maha\033[0m (\033[4m--help\033[0m|\033[4m-h\033[0m|\033[4m-?\033[0m)\n");
			printf("\033[1maha\033[0m reads the Input from a file or stdin and writes HTML-Code to stdout\n");
			printf("\033[4mOptions\033[0m:\n");
			printf("      --black,       -b: \033[1;30m\033[1;47mBlack\033[0m Background and \033[1;37mWhite\033[0m \"standard color\"\n");
			printf("      --pink,        -p: \033[1;35mPink\033[0m Background\n");
			printf("      --colors X       : Set a custom color scheme. X must be a comma-separated\n");
			printf("                         list of 18 CSS colors representing the default colors\n");
			printf("                         and all 16 4-bit color codes: \"\033[1mforeground,background,\n");
			printf("                         black,red,green,yellow,blue,magenta,cyan,white,\n");
			printf("                         (+ bright versions of those eight)\033[0m\"\n");
			printf("      --style X,   -y X: Set the style used in the <body> element\n");
			printf("      --stylesheet,  -s: Use a stylesheet instead of inline styles\n");
			printf("      --iso X,     -i X: Uses ISO 8859-X instead of utf-8. X must be 1..16\n");
			printf("      --title X,   -t X: Gives the html output the title \"X\" instead of\n");
			printf("                         \"stdin\" or the filename\n");
			printf("      --lang X,    -L X: Uses the ISO 639-1 code X for the language\n");
			printf("      --line-fix,    -l: Uses a fix for inputs using control sequences to\n");
			printf("                         change the cursor position like htop. It's a hot fix,\n");
			printf("                         it may not work with any program like htop. Example:\n");
			printf("                         \033[1mecho\033[0m q | \033[1mhtop\033[0m | \033[1maha\033[0m -l > htop.htm\n");
			printf("      --word-wrap,   -w: Wrap long lines in the html file. This works with\n");
			printf("                         CSS3 supporting browsers as well as many older ones.\n");
			printf("      --no-header,   -n: Don't include header into generated HTML,\n");
			printf("                         useful for inclusion in full HTML files.\n");
			printf("      --no-xml,      -x: Don't use doctype xml but html (may useful for old \n");
			printf("                         browsers like IE)\n");
			printf("      --css X,     -c X: Add css file X to the output. In fact just adds \n");
			printf("                         <link rel=\"stylesheet\" href=\"X\" /> to the header.\n");
			printf("      --ignore-cr,   -r: Ignore all carriage-returns (ASCII sign 13, \\r)\n");
			printf("                         which may lead to double new lines in html.\n");
			printf("\033[4mExamples\033[0m:\n");
			printf("          Create an HTML file with a black background, a custom title and\n");
			printf("          a larger font-size using \033[1maha\033[0m's help:\n");
			printf("\n");
			printf("            \033[1;3;33m$\033[0m \033[1maha\033[0m -h | \033[1maha\033[0m -b -t '\033[1maha\033[0m help' -y 'font-size:1.8em' > \033[1maha\033[0m-help.html\n");
			printf("\n");
			printf("          Create an HTML file with a white background using the output of \033[3mdiff\033[0m\n");
			printf("\n");
			printf("            \033[1;3;33m$\033[0m diff -u --color=always oldfile.c newfile.c | \033[1maha\033[0m > diff.html\n");
			printf("\n");
			printf("          Create an HTML file with a white foreground, blue background, and\n");
			printf("          mapping every 4-bit color code to red using \033[1maha\033[0m's help:\n");
			printf("\n");
			printf("            \033[1;3;33m$\033[0m \033[1maha\033[0m -h | \033[1maha\033[0m --colors \"#fff,#00f,red,red,red,red,red,red,red,\\\n");
			printf("              red,red,red,red,red,red,red,red,red\" > \033[1maha\033[0m-help.html\n");
			printf("\n");
			printf("          Create an HTML file with a black background from the output of \033[3mhtop\033[0m.\n");
			printf("          You have to use option -l due the other new-line-commands \033[3mhtop\033[0m uses:\n");
			printf("\n");
			printf("            \033[1;3;33m$\033[0m echo q | htop | \033[1maha\033[0m -b -l > htop.html\n");
			printf("\n");
			printf("          Create an HTML file from the output of this man page. \033[3mman\033[0m uses bold\n");
			printf("          and underline formatting from \033[3mnroff\033[0m, which \033[3mul\033[0m converts to SGR:\n");
			printf("\n");
			printf("            \033[1;3;33m$\033[0m MAN_KEEP_FORMATTING=1 COLUMNS=80 man \033[1maha\033[0m | ul | \033[1maha\033[0m > man-\033[1maha\033[0m.html\n");
			printf("\n");
			printf("Copyleft \033[1;32mAlexander Matthes\033[0m aka \033[4mZiz\033[0m "AHA_YEAR"\n");
			printf("         \033[5;36mziz@mailbox.org\033[0m\n");
			printf("         \033[5;36mhttps://github.com/theZiz/aha\033[0m\n");
			printf("This application is subject to the \033[1;34mMPL1.1\033[0m or \033[1;34mLGPLv2+\033[0m.\n");
			exit(EXIT_SUCCESS);
		}
		else
		if ((strcmp(args[p],(char*)"--version")==0) || (strcmp(args[p],(char*)"-v")==0))
		{
			VERSION_PRINTF_MAKRO
			exit(EXIT_SUCCESS);
		}
		else
		if ((strcmp(args[p],"--title")==0) || (strcmp(args[p],"-t")==0))
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"No title given!\n");
				exit(EXIT_FAILURE);
			}
			opts.title=args[p+1];
			p++;
		}
		else
		if ((strcmp(args[p],"--line-fix")==0) || (strcmp(args[p],"-l")==0))
		{
			opts.htop_fix=1;
		}
		else
		if ((strcmp(args[p],"--no-header")==0) || (strcmp(args[p],"-n")==0))
		{
			opts.no_header=1;
		}
		else
		if ((strcmp(args[p],"--word-wrap")==0) || (strcmp(args[p],"-w")==0))
			opts.word_wrap=1;
		else
		if ((strcmp(args[p],"--black")==0) || (strcmp(args[p],"-b")==0))
		{
			deleteColorScheme(opts.scheme);
			opts.scheme = parseColorScheme(black_scheme);
		}
		else
		if ((strcmp(args[p],"--pink")==0) || (strcmp(args[p],"-p")==0))
		{
			deleteColorScheme(opts.scheme);
			opts.scheme = parseColorScheme(pink_scheme);
		}
		else
		if ((strcmp(args[p],"--colors")==0))
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"No colors given!\n");
				exit(EXIT_FAILURE);
			}
			deleteColorScheme(opts.scheme);
			opts.scheme = parseColorScheme(args[p + 1]);
			if (opts.scheme == NULL)
			{
				fprintf(stderr,"Invalid color scheme '%s'. Scheme must be exactly 18 comma-separated values.\n", args[p+1]);
				exit(EXIT_FAILURE);
			}
			p++;
		}
		else
		if ((strcmp(args[p],"--stylesheet")==0) || (strcmp(args[p],"-s")==0))
			opts.stylesheet=1;
		else
		if ((strcmp(args[p],"--iso")==0) || (strcmp(args[p],"-i")==0))
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"No ISO code given!\n");
				exit(EXIT_FAILURE);
			}
			opts.iso = atoi(args[p+1]);
			if (opts.iso<1 || opts.iso>16)
			{
				fprintf(stderr,"not a valid ISO code: ISO 8859-%s\n",args[p+1]);
				exit(EXIT_FAILURE);
			}
			p++;
		}
		else
		if (strcmp(args[p],"-f")==0)
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"no file to read given after \"-f\"!\n");
				exit(EXIT_FAILURE);
			}
			opts.fp = fopen(args[p+1],"r");
			if (opts.fp==NULL)
			{
				char *errstr = strerror(errno);
				fprintf(stderr,"Failed to open file \"%s\": %s\n",args[p+1],errstr);
				exit(EXIT_FAILURE);
			}
			p++;
			opts.filename=args[p];
		}
		else
		if ((strcmp(args[p],"--no-xml")==0) || (strcmp(args[p],"-x")==0))
			opts.no_xml=1;
		else
		if ((strcmp(args[p],"--lang")==0) || (strcmp(args[p],"-L")==0))
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"No ISO lang code given!\n");
				exit(EXIT_FAILURE);
			}
			opts.lang = args[p+1];
			p++;
		}
		else
		if ((strcmp(args[p],"--css")==0) || (strcmp(args[p],"-c")==0))
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"No css file given!\n");
				exit(EXIT_FAILURE);
			}
			opts.css = args[p+1];
			p++;
		}
		else
		if ((strcmp(args[p],"--ignore-cr")==0) || (strcmp(args[p],"-r")==0))
			opts.ignore_cr=1;
		else
		if ((strcmp(args[p],"--style")==0) || (strcmp(args[p],"-y")==0))
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"No style given!\n");
				exit(EXIT_FAILURE);
			}
			opts.bodystyle=args[p+1];
			p++;
		}
		else
		{
			fprintf(stderr,"Unknown parameter \"%s\"\n",args[p]);
			exit(EXIT_FAILURE);
		}
	}

	return opts;
}

enum ColorMode {
	MODE_4BIT,
	MODE_8BIT,
	MODE_24BIT
};

struct State {
	int fc, bc;
	int bold;
	int italic;
	int underline;
	int blink;
	int crossedout;
	enum ColorMode fc_colormode;
	enum ColorMode bc_colormode;
};

void swapColors(struct State *const state) {
	if (state->bc_colormode == MODE_4BIT && state->bc == -1)
		state->bc = 17;

	if (state->fc_colormode == MODE_4BIT && state->fc == -1)
		state->fc = 16;

	int temp = state->bc;
	state->bc = state->fc;
	state->fc = temp;

	enum ColorMode temp_colormode = state->bc_colormode;
	state->bc_colormode = state->fc_colormode;
	state->fc_colormode = temp_colormode;
}


const struct State default_state = {
	.fc = -1, //Standard Foreground Color //IRC-Color+8
	.bc = -1, //Standard Background Color //IRC-Color+8
	.bold = 0,
	.italic = 0,
	.underline = 0,
	.blink = 0,
	.crossedout = 0,
	.fc_colormode = MODE_4BIT,
	.bc_colormode = MODE_4BIT,
};

int statesDiffer(const struct State *const old, const struct State *const new) {
	return
		(old->fc != new->fc) ||
		(old->bc != new->bc) ||
		(old->bold != new->bold) ||
		(old->italic != new->italic) ||
		(old->underline != new->underline) ||
		(old->blink != new->blink) ||
		(old->crossedout != new->crossedout) ||
		(old->fc_colormode != new->fc_colormode) ||
		(old->bc_colormode != new->bc_colormode);
}

void printHeader(const struct Options *opts)
{
	char encoding[16] = "UTF-8";
	if(opts->iso>0) snprintf(encoding, sizeof(encoding), "ISO-8859-%i", opts->iso);

	if (opts->no_xml)
		printf("<!DOCTYPE html>\n");
	else
		printf("<?xml version=\"1.0\" encoding=\"%s\" ?>\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n", encoding);
	printf("<!-- This file was created with the aha Ansi HTML Adapter. https://github.com/theZiz/aha -->\n");
	if (opts->no_xml)
	{
		if (opts->lang)
		{
			printf("<html lang=\"");
			printHtml(opts->lang);
			printf("\">\n");
		}
		else
		{
			printf("<html>\n");
		}
		printf("<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=%s\">\n", encoding);
	}
	else
	{
		if (opts->lang)
		{
			printf("<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"");
			printHtml(opts->lang);
			printf("\">\n");
		}
		else
		{
			printf("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
		}
		printf("<head>\n<meta http-equiv=\"Content-Type\" content=\"application/xml+xhtml; charset=%s\"/>\n", encoding);
	}
	printf("<title>");
	printHtml(opts->title ? opts->title : opts->filename ? opts->filename : "stdin");
	printf("</title>\n");

	if (opts->css)
	{
		printf("<link rel=\"stylesheet\" href=\"");
		printHtml(opts->css);
		printf("\">\n");
	}

	int style_tag = 0;
	if (opts->stylesheet)
	{
		printf("<style type=\"text/css\">\n");
		style_tag = 1;

		printf("body         {color: %s; background-color: %s;}\n", opts->scheme->fg_default, opts->scheme->bg_default);
		printf(".reset       {color: %s;}\n", opts->scheme->fg_default);
		printf(".bg-reset    {background-color: %s;}\n", opts->scheme->bg_default);
		printf(".inverted    {color: %s;}\n", opts->scheme->bg_default);
		printf(".bg-inverted {background-color: %s;}\n", opts->scheme->fg_default);

		printf(".dimgray {color: %s;}\n", opts->scheme->colors[0]);
		printf(".red     {color: %s;}\n", opts->scheme->colors[1]);
		printf(".green   {color: %s;}\n", opts->scheme->colors[2]);
		printf(".yellow  {color: %s;}\n", opts->scheme->colors[3]);
		printf(".blue    {color: %s;}\n", opts->scheme->colors[4]);
		printf(".purple  {color: %s;}\n", opts->scheme->colors[5]);
		printf(".cyan    {color: %s;}\n", opts->scheme->colors[6]);
		printf(".white   {color: %s;}\n", opts->scheme->colors[7]);
		printf(".dimgray.highlighted {color: %s;}\n", opts->scheme->colors[8]);
		printf(".red.highlighted     {color: %s;}\n", opts->scheme->colors[9]);
		printf(".green.highlighted   {color: %s;}\n", opts->scheme->colors[10]);
		printf(".yellow.highlighted  {color: %s;}\n", opts->scheme->colors[11]);
		printf(".blue.highlighted    {color: %s;}\n", opts->scheme->colors[12]);
		printf(".purple.highlighted  {color: %s;}\n", opts->scheme->colors[13]);
		printf(".cyan.highlighted    {color: %s;}\n", opts->scheme->colors[14]);
		printf(".white.highlighted   {color: %s;}\n", opts->scheme->colors[15]);

		printf(".bg-black  {background-color: %s;}\n", opts->scheme->colors[0]);
		printf(".bg-red    {background-color: %s;}\n", opts->scheme->colors[1]);
		printf(".bg-green  {background-color: %s;}\n", opts->scheme->colors[2]);
		printf(".bg-yellow {background-color: %s;}\n", opts->scheme->colors[3]);
		printf(".bg-blue   {background-color: %s;}\n", opts->scheme->colors[4]);
		printf(".bg-purple {background-color: %s;}\n", opts->scheme->colors[5]);
		printf(".bg-cyan   {background-color: %s;}\n", opts->scheme->colors[6]);
		printf(".bg-white  {background-color: %s;}\n", opts->scheme->colors[7]);
		printf(".bg-black.bg-highlighted  {background-color: %s;}\n", opts->scheme->colors[8]);
		printf(".bg-red.bg-highlighted    {background-color: %s;}\n", opts->scheme->colors[9]);
		printf(".bg-green.bg-highlighted  {background-color: %s;}\n", opts->scheme->colors[10]);
		printf(".bg-yellow.bg-highlighted {background-color: %s;}\n", opts->scheme->colors[11]);
		printf(".bg-blue.bg-highlighted   {background-color: %s;}\n", opts->scheme->colors[12]);
		printf(".bg-purple.bg-highlighted {background-color: %s;}\n", opts->scheme->colors[13]);
		printf(".bg-cyan.bg-highlighted   {background-color: %s;}\n", opts->scheme->colors[14]);
		printf(".bg-white.bg-highlighted  {background-color: %s;}\n", opts->scheme->colors[15]);


		printf(".underline   {text-decoration: underline;}\n");
		printf(".bold        {font-weight: bold;}\n");
		printf(".italic      {font-style: italic;}\n");
		printf(".blink       {text-decoration: blink;}\n");
		printf(".crossed-out {text-decoration: line-through;}\n");
	}

	if (opts->word_wrap)
	{
		if (!style_tag) {
			printf("<style type=\"text/css\">\n");
			style_tag = 1;
		}

		printf("pre {white-space: pre-wrap; white-space: -moz-pre-wrap !important;\n");
		printf("white-space: -pre-wrap; white-space: -o-pre-wrap; word-wrap: break-word;}\n");
	}

	if (style_tag)
		printf("</style>\n");
	printf("</head>\n");

	if (opts->stylesheet)
	{
		printf("<body>\n");
	}
	else
	{
		printf("<body");
		printf(" style=\"color: %s; background-color: %s;", opts->scheme->fg_default, opts->scheme->bg_default);
		if(opts->bodystyle) {
			printHtml(opts->bodystyle);
		}
		printf("\"");
		printf(">\n");
	}

	printf("<pre>\n");
}

int main(int argc,char* args[])
{
	struct Options opts = parseArgs(argc, args);
	register FILE* fp = opts.fp;

	char* fcclass[18] = {
		"dimgray", // Black
		"red", // Red
		"green", // Green
		"yellow", // Yellow
		"blue", // Blue
		"purple", // Magenta
		"cyan", // Cyan
		"white", // White
		"highlighted dimgray", // Bright Black
		"highlighted red", // Bright Red
		"highlighted green", // Bright Green
		"highlighted yellow", // Bright Yellow
		"highlighted blue", // Bright Blue
		"highlighted purple", // Bright Magenta
		"highlighted cyan", // Bright Cyan
		"highlighted white", // Bright White
		"reset", // Default Foreground
		"inverted" // Default Background
	};

	char* bcclass[18] = {
		"bg-black", // Black
		"bg-red", // Red
		"bg-green", // Green
		"bg-yellow", // Yellow
		"bg-blue", // Blue
		"bg-purple", // Magenta
		"bg-cyan", // Cyan
		"bg-white", // White
		"bg-highlighted bg-black", // Bright Black
		"bg-highlighted bg-red", // Bright Red
		"bg-highlighted bg-green", // Bright Green
		"bg-highlighted bg-yellow", // Bright Yellow
		"bg-highlighted bg-blue", // Bright Blue
		"bg-highlighted bg-purple", // Bright Magenta
		"bg-highlighted bg-cyan", // Bright Cyan
		"bg-highlighted bg-white", // Bright White
		"bg-inverted", // Default Foreground
		"bg-reset" // Default Background
	};

	if (!opts.no_header)
		printHeader(&opts);

	//Begin of Conversion
	struct State state = default_state;
	struct State oldstate;
	int c;
	int negative = 0; //No negative image
	int special_char = 0; //No special characters
	int line=0;
	int momline=0;
	int newline=-1;
	while ((c=fgetc(fp)) != EOF)
	{
		if (c=='\033')
		{
			oldstate = state;
			//Searching the end (a letter) and safe the insert:
			c=getNextChar(fp);
			if ( c == '[' ) // CSI code, see https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
			{
				char buffer[1024];
				buffer[0] = '[';
				int counter=1;
				while ((c<'A') || ((c>'Z') && (c<'a')) || (c>'z'))
				{
					c=getNextChar(fp);
					buffer[counter]=c;
					if (c=='>') //end of htop
						break;
					counter++;
					if (counter>1022)
						break;
				}
				buffer[counter-1]=0;
				pelem elem;
				switch (c)
				{
					case 'm':
						elem=parseInsert(buffer);
						pelem momelem=elem;
						while (momelem!=NULL)
						{
							switch (momelem->value)
							{
								case 0: // 0 - Reset all
									state = default_state;
									negative=0; special_char=0;
									break;

								case 1: // 1 - Enable Bold
									state.bold=1;
									break;

								case 3: // 3 - Enable Italic
									state.italic=1;
									break;

								case 4: // 4 - Enable underline
									state.underline=1;
									break;

								case 5: // 5 - Slow Blink
									state.blink=1;
									break;

								case 7: // 7 - Inverse video
									swapColors(&state);
									negative = !negative;
									break;

								case 9: // 9 - Enable Crossed-out
									state.crossedout=1;
									break;

								case 21: // 21 - Reset bold
								case 22: // 22 - Not bold, not "high intensity" color
									state.bold=0;
									break;

								case 23: // 23 - Reset italic
									state.italic=0;
									break;

								case 24: // 23 - Reset underline
									state.underline=0;
									break;

								case 25: // 25 - Reset blink
									state.blink=0;
									break;

								case 27: // 27 - Reset Inverted
									if (negative)
									{
										swapColors(&state); //7, 7X is not defined (and supported)
										negative = 0;
									}
									break;

								case 29: // 29 - Reset crossed-out
									state.crossedout=0;
									break;

								case 30:
								case 31:
								case 32:
								case 33:
								case 34:
								case 35:
								case 36:
								case 37:
								case 38: // 3X - Set foreground color
									{
										int *dest = &(state.fc);
										if (negative != 0)
											dest=&(state.bc);
										if (momelem->value == 38 &&
											momelem->next &&
											momelem->next->value == 5 &&
											momelem->next->next)// 38;5;<n> -> 8 Bit
										{
											momelem = momelem->next->next;
											state.fc_colormode = MODE_8BIT;
											*dest = momelem->value;
										}
										else
										if (momelem->value == 38 &&
											momelem->next &&
											momelem->next->value == 2 &&
											momelem->next->next)// 38;2;<n> -> 24 Bit
										{
											momelem = momelem->next->next;
											pelem r,g,b;
											r = momelem;
											momelem = momelem->next;
											g = momelem;
											if ( momelem )
												momelem = momelem->next;
											b = momelem;
											if ( r && g && b )
											{
												state.fc_colormode = MODE_24BIT;
												*dest =
													(r->value & 255) * 65536 +
													(g->value & 255) * 256 +
													(b->value & 255);
											}
										}
										else
										{
											state.fc_colormode = MODE_4BIT;
											*dest=momelem->value-30;
										}
									}
									break;
								case 39: // Set foreground color to default
									state.fc_colormode = MODE_4BIT;
									state.fc = -1;
									break;
								case 40:
								case 41:
								case 42:
								case 43:
								case 44:
								case 45:
								case 46:
								case 47:
								case 48: // 4X - Set background color
									{
										int *dest = &(state.bc);
										if (negative != 0)
											dest=&(state.fc);
										if (momelem->value == 48 &&
											momelem->next &&
											momelem->next->value == 5 &&
											momelem->next->next)// 48;5;<n> -> 8 Bit
										{
											momelem = momelem->next->next;
											state.bc_colormode = MODE_8BIT;
											*dest = momelem->value;
										}
										else
										if (momelem->value == 48 &&
											momelem->next &&
											momelem->next->value == 2 &&
											momelem->next->next)// 48;2;<n> -> 24 Bit
										{
											momelem = momelem->next->next;
											pelem r,g,b;
											r = momelem;
											momelem = momelem->next;
											g = momelem;
											if ( momelem )
												momelem = momelem->next;
											b = momelem;
											if ( r && g && b )
											{
												state.bc_colormode = MODE_24BIT;
												*dest =
													(r->value & 255) * 65536 +
													(g->value & 255) * 256 +
													(b->value & 255);
											}
										}
										else
										{
											state.bc_colormode = MODE_4BIT;
											*dest=momelem->value-40;
										}
									}
									break;
								case 49: // Set background color to default
									state.bc_colormode = MODE_4BIT;
									state.bc = -1;
									break;
								case 90:
								case 91:
								case 92:
								case 93:
								case 94:
								case 95:
								case 96:
								case 97: // 9X - Set foreground color highlighted
									{
										int *dest = &(state.fc);
										if (negative != 0)
											dest=&(state.bc);
										state.fc_colormode = MODE_4BIT;
										*dest=momelem->value-90+8;
									}
									break;

								case 100:
								case 101:
								case 102:
								case 103:
								case 104:
								case 105:
								case 106:
								case 107: // 10X - Set background color highlighted
									{
										int *dest = &(state.bc);
										if (negative != 0)
											dest=&(state.fc);
										state.bc_colormode = MODE_4BIT;
										*dest=momelem->value-100+8;
									}
									break;
							}
							momelem=momelem->next;
						}
						deleteParse(elem);
					break;
					case 'H':
						if (opts.htop_fix) //a little dirty ...
						{
							elem=parseInsert(buffer);
							pelem second=elem->next;
							if (second==NULL)
								second=elem;
							newline=second->digit[0]-1;
							if (second->digitcount>1)
								newline=(newline+1)*10+second->digit[1]-1;
							if (second->digitcount>2)
								newline=(newline+1)*10+second->digit[2]-1;
							deleteParse(elem);
							if (newline<line)
								opts.line_break=1;
						}
					break;
				}
				if (opts.htop_fix)
					if (opts.line_break)
					{
						for (;line<80;line++)
							printf(" ");
					}
				//Checking the differences
				if ( statesDiffer(&state, &oldstate) ) //ANY Change
				{
					// If old state was different than the default one, close the current <span>
					if (statesDiffer(&oldstate, &default_state))
						printf("</span>");
					// Open new <span> if current state differs from the default one
					if (statesDiffer(&state, &default_state))
					{
						if (opts.stylesheet)
						{
							printf("<span class=\"");
							if(state.underline)
								printf("underline ");
							if(state.bold)
								printf("bold ");
							if(state.italic)
								printf("italic ");
							if(state.blink)
								printf("blink ");
							if(state.crossedout)
								printf("crossed-out ");

							int needs_style = 0;
							// classes first
							switch (state.fc_colormode)
							{
								case MODE_4BIT:
									if (state.fc>=0 && state.fc<=17)
										printf("%s ", fcclass[state.fc]);
									break;
								case MODE_8BIT:
									if (state.fc>=0 && state.fc<=15)
										printf("%s ", fcclass[state.fc]);
									else
										needs_style = 1;
									break;
								case MODE_24BIT:
									needs_style = 1;
									break;
							}

							switch (state.bc_colormode)
							{
								case MODE_4BIT:
									if (state.bc>=0 && state.bc<=17)
										printf("%s ", bcclass[state.bc]);
									break;
								case MODE_8BIT:
									if (state.bc>=0 && state.bc<=15)
										printf("%s ", bcclass[state.bc]);
									else
										needs_style = 1;
									break;
								case MODE_24BIT:
									needs_style = 1;
									break;
							};

							// print anything that needs to be in a style attribute
							if(needs_style)
							{
								printf("\" style=\"");

								switch (state.fc_colormode)
								{
									case MODE_4BIT:
										break;
									case MODE_8BIT:
										if (state.fc>=0 && state.fc<=15)
											break;
										char rgb[12];
										make_rgb(state.fc,rgb);
										printf("color:#%s;",rgb);
										break;
									case MODE_24BIT:
										printf("color:#%06x;",state.fc);
										break;
								}
								switch (state.bc_colormode)
								{
									case MODE_4BIT:
										break;
									case MODE_8BIT:
										if (state.bc>=0 && state.bc<=15)
											break;
										char rgb[12];
										make_rgb(state.bc,rgb);
										printf("background-color:#%s;",rgb);
										break;
									case MODE_24BIT:
										printf("background-color:#%06x;",state.bc);
										break;
								}
							}
							printf("\">");
						}
						else // !state.stylesheet
						{
							printf("<span style=\"");
							if(state.underline)
								printf("text-decoration:underline;");
							if(state.bold)
								printf("font-weight:bold;");
							if(state.italic)
								printf("font-style:italic;");
							if(state.blink)
								printf("text-decoration:blink;");
							if(state.crossedout)
								printf("text-decoration: line-through;");

							switch (state.fc_colormode)
							{
								case MODE_4BIT:
									if (state.fc>=0 && state.fc<=17)
										printf("color:%s;", opts.scheme->colors[state.fc]);
									break;
								case MODE_8BIT:
									if (state.fc>=0 && state.fc<=15)
										printf("color:%s;", opts.scheme->colors[state.fc]);
									else
									{
										char rgb[12];
										make_rgb(state.fc,rgb);
										printf("color:#%s;",rgb);
									}
									break;
								case MODE_24BIT:
									printf("color:#%06x;",state.fc);
									break;
							};

							switch (state.bc_colormode)
							{
								case MODE_4BIT:
									if (state.bc>=0 && state.bc<=17)
										printf("background-color:%s", opts.scheme->colors[state.bc]);
									break;
								case MODE_8BIT:
									if (state.bc>=0 && state.bc<=15)
										printf("background-color:%s;", opts.scheme->colors[state.bc]);
									else
									{
										char rgb[12];
										make_rgb(state.bc,rgb);
										printf("background-color:#%s;",rgb);
									}
									break;
								case MODE_24BIT:
									printf("background-color:#%06x;",state.bc);
									break;
							};
							printf("\">");
						}
					}
				}
			}
			else
			if ( c == ']' ) //Operating System Command (OSC), ignoring for now
			{
				while (c != 2 && c != 7 && c!= 27) //STX, BEL or ESC end an OSC.
					c = getNextChar(fp);
				if ( c == 27 ) // expecting \ after ESC
					c = getNextChar(fp);
			}
			else
			if ( c == '(' ) //Some VT100 ESC sequences, which should be ignored
			{
				//Reading (and ignoring!) one character should work for "(B"
				//(US ASCII character set), "(A" (UK ASCII character set) and
				//"(0" (Graphic). This whole "standard" is fucked up. Really...
				c = getNextChar(fp);
				if (c == '0') //we do not ignore ESC(0 ;)
					special_char=1;
				else
					special_char=0;
			}
		}
		else
		if (c==13 && opts.htop_fix)
		{
			for (;line<80;line++)
				printf(" ");
			line=0;
			momline++;
			printf("\n");
		}
		else
		if (c==13 && opts.ignore_cr)
		{
		}
		else if (c!=8)
		{
			line++;
			if (opts.line_break)
			{
				printf("\n");
				line=0;
				opts.line_break=0;
				momline++;
			}
			if (newline>=0)
			{
				while (newline>line)
				{
					printf(" ");
					line++;
				}
				newline=-1;
			}
			//I want fall throught, so I ignore the gcc warning for this switch
			#if defined(__GNUC__) && __GNUC__ >= 5
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wimplicit-fallthrough="
			#endif
			switch (c)
			{
				case '&':	printf("&amp;"); break;
				case '\"':	printf("&quot;"); break;
				case '<':	printf("&lt;"); break;
				case '>':	printf("&gt;"); break;
				case '\n':case 13:
					momline++;
					line=0;
				default:
					if (special_char)
						printf("%s",ansi_vt220_character_set[((int)c+32) & 255]);
					else
						printf("%c",c);
			}
			#if defined(__GNUC__) && __GNUC__ >= 5
			#pragma GCC diagnostic pop
			#endif
			if (opts.iso>0) //only at ISOS
				if ((c & 128)==128) //first bit set => there must be followbytes
				{
					int bits=2;
					if ((c & 32)==32)
						bits++;
					if ((c & 16)==16)
						bits++;
					for (int meow=1;meow<bits;meow++)
						printf("%c",getNextChar(fp));

				}
		}
	}

	// If current state is different than the default, there is a <span> open - close it
	if (statesDiffer(&state, &default_state))
		printf("</span>");

	//Footer
	if (opts.no_header == 0)
	{
		printf("</pre>\n");
		printf("</body>\n");
		printf("</html>\n");
	}

	if (fp!=stdin)
		fclose(fp);
	return 0;
}
