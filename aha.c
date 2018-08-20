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
 of the GNU Lesser General Public license version 2 or later (LGPL2+),
 in which case the provisions of LGPL License are applicable instead of
 those above.

 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , ziz_at_mailbox.org
*/
#define AHA_VERSION "0.4.10.6"
#define TEST
#define AHA_YEAR "2017"
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
		if (s[pos]==';' || s[pos]==0)
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

enum ColorScheme {
	SCHEME_WHITE,
	SCHEME_BLACK,
	SCHEME_PINK
};

struct Options {
	enum ColorScheme colorscheme;
	char* filename;
	FILE *fp;
	int htop_fix;
	int iso;
	int line_break;
	int no_header;
	int stylesheet;
	char *title;
	int word_wrap;
};

#define VERSION_PRINTF_MAKRO \
	printf("\033[1;31mAnsi Html Adapter\033[0m Version "AHA_VERSION"\n");

struct Options parseArgs(int argc, char* args[])
{
	struct Options opts = (struct Options){
		.colorscheme = SCHEME_WHITE,
		.filename = NULL,
		.fp = stdin,
		.htop_fix = 0,
		.iso = -1,
		.line_break = 0,
		.no_header = 0,
		.stylesheet = 0,
		.title = NULL,
		.word_wrap = 0
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
			printf("\033[4moptions\033[0m: --black,      -b: \033[1;30m\033[1;47mBlack\033[0m Background and \033[1;37mWhite\033[0m \"standard color\"\n");
			printf("         --pink,       -p: \033[1;35mPink\033[0m Background\n");
			printf("         --stylesheet, -s: Use a stylesheet instead of inline styles\n");
			printf("         --iso X,    -i X: Uses ISO 8859-X instead of utf-8. X must be 1..16\n");
			printf("         --title X,  -t X: Gives the html output the title \"X\" instead of\n");
			printf("                           \"stdin\" or the filename\n");
			printf("         --line-fix,   -l: Uses a fix for inputs using control sequences to\n");
			printf("                           change the cursor position like htop. It's a hot fix,\n");
			printf("                           it may not work with any program like htop. Example:\n");
			printf("                           \033[1mecho\033[0m q | \033[1mhtop\033[0m | \033[1maha\033[0m -l > htop.htm\n");
			printf("         --word-wrap,  -w: Wrap long lines in the html file. This works with\n");
			printf("                           CSS3 supporting browsers as well as many older ones.\n");
			printf("         --no-header,  -n: Don't include header into generated HTML,\n");
			printf("                           useful for inclusion in full HTML files.\n");
			printf("Example: \033[1maha\033[0m --help | \033[1maha\033[0m --black > aha-help.htm\n");
			printf("         Writes this help text to the file aha-help.htm\n\n");
			printf("Copyleft \033[1;32mAlexander Matthes\033[0m aka \033[4mZiz\033[0m "AHA_YEAR"\n");
			printf("         \033[5;36mziz@mailbox.org\033[0m\n");
			printf("         \033[5;36mhttps://github.com/theZiz/aha\033[0m\n");
			printf("This application is subject to the \033[1;34mMPL\033[0m or \033[1;34mLGPL\033[0m.\n");
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
			opts.colorscheme=SCHEME_BLACK;
		else
		if ((strcmp(args[p],"--pink")==0) || (strcmp(args[p],"-p")==0))
			opts.colorscheme=SCHEME_PINK;
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
		{
			fprintf(stderr,"Unknown parameter \"%s\"\n",args[p]);
			exit(EXIT_FAILURE);
		}
	}

	return opts;
}

void printHeader(const struct Options *opts)
{
	char encoding[16] = "UTF-8";
	if(opts->iso>0) snprintf(encoding, sizeof(encoding), "ISO-8859-%i", opts->iso);

	printf("<?xml version=\"1.0\" encoding=\"%s\" ?>\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n", encoding);
	printf("<!-- This file was created with the aha Ansi HTML Adapter. https://github.com/theZiz/aha -->\n");
	printf("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
	printf("<head>\n<meta http-equiv=\"Content-Type\" content=\"application/xml+xhtml; charset=%s\" />\n", encoding);

	printf("<title>");
	printHtml(opts->title ? opts->title : opts->filename ? opts->filename : "stdin");
	printf("</title>\n");

	int style_tag = 0;
	if (opts->stylesheet)
	{
		printf("<style type=\"text/css\">\n");
		style_tag = 1;

		switch (opts->colorscheme)
		{
			case SCHEME_BLACK:  printf("body         {color: white; background-color: black;}\n");
							 printf(".reset       {color: white;}\n");
							 printf(".bg-reset    {background-color: black;}\n");
							 printf(".inverted    {color: black;}\n");
							 printf(".bg-inverted {background-color: white;}\n");
							 break;
			case SCHEME_PINK:  printf("body         {background-color: pink;}\n");
							 printf(".reset       {color: black;}\n");
							 printf(".bg-reset    {background-color: pink;}\n");
							 printf(".inverted    {color: pink;}\n");
							 printf(".bg-inverted {background-color: black;}\n");
							 break;
			default: printf(".reset       {color: black;}\n");
					 printf(".bg-reset    {background-color: white;}\n");
					 printf(".inverted    {color: white;}\n");
					 printf(".bg-inverted {background-color: black;}\n");
		}
		if (opts->colorscheme != SCHEME_BLACK)
		{
			printf(".dimgray     {color: dimgray;}\n");
			printf(".red         {color: red;}\n");
			printf(".green       {color: green;}\n");
			printf(".yellow      {color: olive;}\n");
			printf(".blue        {color: blue;}\n");
			printf(".purple      {color: purple;}\n");
			printf(".cyan        {color: teal;}\n");
			printf(".white       {color: gray;}\n");
			printf(".bg-black    {background-color: black;}\n");
			printf(".bg-red      {background-color: red;}\n");
			printf(".bg-green    {background-color: green;}\n");
			printf(".bg-yellow   {background-color: olive;}\n");
			printf(".bg-blue     {background-color: blue;}\n");
			printf(".bg-purple   {background-color: purple;}\n");
			printf(".bg-cyan     {background-color: teal;}\n");
			printf(".bg-white    {background-color: gray;}\n");
		}
		else
		{
			printf(".dimgray     {color: dimgray;}\n");
			printf(".red         {color: red;}\n");
			printf(".green       {color: lime;}\n");
			printf(".yellow      {color: yellow;}\n");
			printf(".blue        {color: #3333FF;}\n");
			printf(".purple      {color: fuchsia;}\n");
			printf(".cyan        {color: aqua;}\n");
			printf(".white       {color: white;}\n");
			printf(".bg-black    {background-color: black;}\n");
			printf(".bg-red      {background-color: red;}\n");
			printf(".bg-green    {background-color: lime;}\n");
			printf(".bg-yellow   {background-color: yellow;}\n");
			printf(".bg-blue     {background-color: #3333FF;}\n");
			printf(".bg-purple   {background-color: fuchsia;}\n");
			printf(".bg-cyan     {background-color: aqua;}\n");
			printf(".bg-white    {background-color: white;}\n");
		}
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
		switch (opts->colorscheme)
		{
			case SCHEME_BLACK: printf("<body style=\"color:white; background-color:black\">\n"); break;
			case SCHEME_PINK: printf("<body style=\"background-color:pink\">\n");	break;
			case SCHEME_WHITE: printf("<body>\n"); break;
		}
	}

	printf("<pre>\n");
}

int main(int argc,char* args[])
{
	struct Options opts = parseArgs(argc, args);
	register FILE* fp = opts.fp;

	char* fcstyle[10] = {
		opts.stylesheet ? "dimgray " : "color:dimgray;", //Black
		opts.stylesheet ? "red " : "color:red;", //Red
		opts.stylesheet ? "green " : opts.colorscheme==SCHEME_BLACK ? "color:lime;" : "color:green;", //Green
		opts.stylesheet ? "yellow " : opts.colorscheme==SCHEME_BLACK ? "color:yellow;" : "color:olive;", //Yellow
		opts.stylesheet ? "blue " : opts.colorscheme==SCHEME_BLACK ? "color:#3333FF;" : "color:blue;", //Blue
		opts.stylesheet ? "purple " : opts.colorscheme==SCHEME_BLACK ? "color:fuchsia;" : "color:purple;", //Purple
		opts.stylesheet ? "cyan " : opts.colorscheme==SCHEME_BLACK ? "color:aqua;" : "color:teal;", //Cyan
		opts.stylesheet ? "white " : opts.colorscheme==SCHEME_BLACK ? "color:white;" : "color:gray;", //White
		opts.stylesheet ? "inverted " : opts.colorscheme==SCHEME_BLACK ? "color:black;" : opts.colorscheme==SCHEME_PINK ? "color:pink;" : "color:white;", //Background
		opts.stylesheet ? "reset " : opts.colorscheme==SCHEME_BLACK ? "color:white;" : "color:black;" //Foreground
	};

	char* bcstyle[10] = {
		opts.stylesheet ? "bg-black " : "background-color:black;", //Black
		opts.stylesheet ? "bg-red " : "background-color:red;", //Red
		opts.stylesheet ? "bg-green " : opts.colorscheme==SCHEME_BLACK ? "background-color:lime;" : "background-color:green;", //Green
		opts.stylesheet ? "bg-yellow " : opts.colorscheme==SCHEME_BLACK ? "background-color:yellow;" : "background-color:olive;", //Yellow
		opts.stylesheet ? "bg-blue " : opts.colorscheme==SCHEME_BLACK ? "background-color:#3333FF;" : "background-color:blue;", //Blue
		opts.stylesheet ? "bg-purple " : opts.colorscheme==SCHEME_BLACK ? "background-color:fuchsia;" : "background-color:purple;", //Purple
		opts.stylesheet ? "bg-cyan " : opts.colorscheme==SCHEME_BLACK ? "background-color:aqua;" : "background-color:teal;", //Cyan
		opts.stylesheet ? "bg-white " : opts.colorscheme==SCHEME_BLACK ? "background-color:white;" : "background-color:gray;", //White
		opts.stylesheet ? "bg-reset " : opts.colorscheme==SCHEME_BLACK ? "background-color:black;" : opts.colorscheme==SCHEME_PINK ? "background-color:pink;" : "background-color:white;", //Background
		opts.stylesheet ? "bg-inverted " : opts.colorscheme==SCHEME_BLACK ? "background-color:white;" : "background-color:black;", //Foreground
	};

	if (!opts.no_header)
		printHeader(&opts);

	//Begin of Conversion
	int c;
	int fc = -1; //Standard Foreground Color //IRC-Color+8
	int bc = -1; //Standard Background Color //IRC-Color+8
	int ul = 0; //Not underlined
	int bo = 0; //Not bold
	int it = 0; //Not italic
	int bl = 0; //No Blinking
	int co = 0; //Not crossed out
	int negative = 0; //No negative image
	int special_char = 0; //No special characters
	int ofc,obc,oul,obo,oit,obl,oco; //old values
	int line=0;
	int momline=0;
	int newline=-1;
	int temp;
	while ((c=fgetc(fp)) != EOF)
	{
		if (c=='\033')
		{
			//Saving old values
			ofc=fc;
			obc=bc;
			oul=ul;
			obo=bo;
			oit=it;
			obl=bl;
			oco=co;
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
									bo=0; it=0; ul=0; bl=0; co=0;
									fc=-1; bc=-1;
									negative=0; special_char=0;
									break;

								case 1: // 1 - Enable Bold
									bo=1;
									break;

								case 3: // 3 - Enable Italic
									it=1;
									break;

								case 4: // 4 - Enable underline
									ul=1;
									break;

								case 5: // 5 - Slow Blink
									bl=1;
									break;

								case 7: // 7 - Inverse video
									if (bc == -1)
										bc = 8;
									if (fc == -1)
										fc = 9;
									temp = bc;
									bc = fc;
									fc = temp;
									negative = !negative;
									break;

								case 9: // 9 - Enable Crossed-out
									co=1;
									break;

								case 21: // 21 - Reset bold
								case 22: // 22 - Not bold, not "high intensity" color
									bo=0;
									break;

								case 23: // 23 - Reset italic
									it=0;
									break;

								case 24: // 23 - Reset underline
									ul=0;
									break;

								case 25: // 25 - Reset blink
									bl=0;
									break;

								case 27: // 27 - Reset Inverted
									if (bc == -1)
										bc = 8;
									if (fc == -1)
										fc = 9;
									temp = bc;
									bc = fc;
									fc = temp;
									negative = 0;
									break;

								case 29: // 29 - Reset crossed-out
									co=0;
									break;

								case 30:
								case 31:
								case 32:
								case 33:
								case 34:
								case 35:
								case 36:
								case 37:
								case 38:
								case 39: // 3X - Set foreground color
									if (negative == 0)
										fc=momelem->value-30;
									else
										bc=momelem->value-30;
									break;

								case 40:
								case 41:
								case 42:
								case 43:
								case 44:
								case 45:
								case 46:
								case 47:
								case 48:
								case 49: // 4X - Set background color
									if (negative == 0)
										bc=momelem->value-40;
									else
										fc=momelem->value-40;
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
				if ((fc!=ofc) || (bc!=obc) || (ul!=oul) || (bo!=obo) || (oit!=it) || (bl!=obl) || (co!=oco)) //ANY Change
				{
					if ((ofc!=-1) || (obc!=-1) || (oul!=0) || (obo!=0) || (oit!=0) || (obl!=0) || (oco!=0))
						printf("</span>");
					if ((fc!=-1) || (bc!=-1) || (ul!=0) || (bo!=0) || (it!=0) || (bl!=0) || (co!=0))
					{
						if (opts.stylesheet)
							printf("<span class=\"");
						else
							printf("<span style=\"");

						if(fc>=0 && fc<=9) printf("%s", fcstyle[fc]);
						if(bc>=0 && bc<=9) printf("%s", bcstyle[bc]);

						if (ul)
						{
							if (opts.stylesheet)
								printf("underline ");
							else
								printf("text-decoration:underline;");
						}
						if (bo)
						{
							if (opts.stylesheet)
								printf("bold ");
							else
								printf("font-weight:bold;");
						}
						if (it)
						{
							if (opts.stylesheet)
								printf("italic ");
							else
								printf("font-weight:italic;");
						}
						if (bl)
						{
							if (opts.stylesheet)
								printf("blink ");
							else
								printf("text-decoration:blink;");
						}
						if (co)
						{
							if (opts.stylesheet)
								printf("crossed-out ");
							else
								printf("text-decoration:line-through;");
						}
						printf("\">");
					}
				}
			}
			else
			if ( c == ']' ) //Operating System Command (OSC), ignoring for now
			{
				while (c != 2 && c != 7) //STX and BEL end an OSC.
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
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wimplicit-fallthrough="
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
			#pragma GCC diagnostic pop
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

	//Footer
	if ((fc!=-1) || (bc!=-1) || (ul!=0) || (bo!=0) || (bl!=0))
		printf("</span>\n");

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
