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
 of the GNU Lesser General Public license (the	"LGPL License"), in which case the
 provisions of LGPL License are applicable instead of those
 above.

 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com
*/
#define AHA_VERSION "0.4.8.0"
#define AHA_YEAR "2015"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int future=0;
int future_char=0;


int divide (int dividend, int divisor){
	div_t result;
	result = div (dividend, divisor);
	return result.quot;
}
//
// makes the the comma-separated values for an RGB given
// a 256 color value.  Example 196 => 255,0,0
//
void make_rgb (int color_id, char str_rgb[11]){
	//printf("color_id = %d", color_id);
	// QA color_id as less than <256

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
	sprintf(str_rgb, "%d,%d,%d", rgb_R,rgb_G,rgb_B);
}



int getNextChar(register FILE* fp)
{
	int c;
	if (future)
	{
		future=0;
		return future_char;
	}
	if ((c = fgetc(fp)) != EOF)
		return c;
	fprintf(stderr,"Unknown Error in File Parsing!\n");
	exit(1);
}

typedef struct selem *pelem;
typedef struct selem {
	unsigned char digit[8];
	unsigned char digitcount;
	pelem next;
} telem;

pelem parseInsert(char* s)
{
	pelem firstelem=NULL;
	pelem momelem=NULL;
	unsigned char digit[8];
	unsigned char digitcount=0;
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
			for (unsigned char a=0;a<8;a++)
				newelem->digit[a]=digit[a];
			newelem->digitcount=digitcount;
			newelem->next=NULL;
			if (momelem==NULL)
				firstelem=newelem;
			else
				momelem->next=newelem;
			momelem=newelem;
			digitcount=0;
			memset(digit,0,8);
			if (s[pos]==0)
				break;
		}
		else
		if (digitcount<8)
		{
			digit[digitcount]=s[pos]-'0';
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

int createIntegerFromStructure (pelem elem){
	int c, sign, offset, n;
	offset = 0;
	n = 0;
	for (c = offset; c < elem->digitcount; c++){
	   n = n*10 + elem->digit[c] - 0;
	}
	//printf("\nJLPOOLE %d: n = %d\n",__LINE__,n);
    return n;
}
#define VERSION_PRINTF_MAKRO \
	printf("\033[1;31mAnsi Html Adapter\033[0m Version "AHA_VERSION"\n");

int main(int argc,char* args[])
{
	char* filename=NULL;
	register FILE *fp = stdin;
	int colorshema=0; //0:normal, 1:black, 2:pink
	int iso=-1; //utf8
	char stylesheet=0;
	char htop_fix=0;
	char line_break=0;
	char* title=NULL;
	char word_wrap=0;
	char no_header=0;
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
			printf("         \033[5;36mzizsdl@googlemail.com\033[0m\n");
			printf("         \033[5;36mhttp://ziz.delphigl.com/tool_aha.php\033[0m\n");
			printf("This application is subject to the \033[1;34mMPL\033[0m or \033[1;34mLGPL\033[0m.\n");
			return 0;
		}
		else
		if ((strcmp(args[p],(char*)"--version")==0) || (strcmp(args[p],(char*)"-v")==0))
		{
			VERSION_PRINTF_MAKRO
			return 0;
		}
		else
		if ((strcmp(args[p],"--title")==0) || (strcmp(args[p],"-t")==0))
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"No title given!\n");
				return 0;
			}
			title=args[p+1];
			p++;
		}
		else
		if ((strcmp(args[p],"--line-fix")==0) || (strcmp(args[p],"-l")==0))
		{
			htop_fix=1;
		}
		else
		if ((strcmp(args[p],"--no-header")==0) || (strcmp(args[p],"-n")==0))
		{
			no_header=1;
		}
		else
		if ((strcmp(args[p],"--word-wrap")==0) || (strcmp(args[p],"-w")==0))
			word_wrap=1;
		else
		if ((strcmp(args[p],"--black")==0) || (strcmp(args[p],"-b")==0))
			colorshema=1;
		else
		if ((strcmp(args[p],"--pink")==0) || (strcmp(args[p],"-p")==0))
			colorshema=2;
		else
		if ((strcmp(args[p],"--stylesheet")==0) || (strcmp(args[p],"-s")==0))
			stylesheet=1;
		else
		if ((strcmp(args[p],"--iso")==0) || (strcmp(args[p],"-i")==0))
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"No ISO code given!\n");
				return 0;
			}
			iso = atoi(args[p+1]);
			if (iso<1 || iso>16)
			{
				fprintf(stderr,"not a valid ISO code: ISO 8859-%s\n",args[p+1]);
				return 0;
			}
			p++;
		}
		else
		if (strcmp(args[p],"-f")==0)
		{
			if (p+1>=argc)
			{
				fprintf(stderr,"no file to read given after \"-f\"!\n");
				return 0;
			}
			fp = fopen(args[p+1],"r");
			if (fp==NULL)
			{
				fprintf(stderr,"file \"%s\" not found!\n",args[p+1]);
				return 0;
			}
			p++;
			filename=args[p];
		}
		else
		{
			fprintf(stderr,"Unknown parameter \"%s\"\n",args[p]);
			return 0;
		}
	}

	if (no_header == 0)
	{
		//Header:
		if (iso<0)
			printf("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n");
		else
			printf("<?xml version=\"1.0\" encoding=\"ISO-8859-%i\" ?><!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n",iso);
		printf("<!-- This file was created with the aha Ansi HTML Adapter. http://ziz.delphigl.com/tool_aha.php -->\n");
		printf("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
		printf("<head>\n<meta http-equiv=\"Content-Type\" content=\"application/xml+xhtml; charset=UTF-8\" />\n");
		if (title)
			printf("<title>%s</title>\n",title);
		else
		{
			if (filename==NULL)
				printf("<title>stdin</title>\n");
			else
				printf("<title>%s</title>\n",filename);
		}
		if (stylesheet)
		{
			printf("<style type=\"text/css\">\n");
			switch (colorshema)
			{
				case 1:  printf("body       {color: white; background-color: black;}\n");
								 printf(".reset     {color: white;}\n");
								 printf(".bg-reset  {background-color: black;}\n");
								 break;
				case 2:  printf("body       {background-color: pink;}\n");
								 printf(".reset     {color: black;}\n");
								 printf(".bg-reset  {background-color: pink;}\n");
								 break;
				default: printf(".reset     {color: black;}\n");
				         printf(".bg-reset  {background-color: white;}\n");
			}
			if (colorshema!=1)
			{
				printf(".dimgray     {color: dimgray;}\n");
				printf(".red       {color: red;}\n");
				printf(".green     {color: green;}\n");
				printf(".yellow    {color: olive;}\n");
				printf(".blue      {color: blue;}\n");
				printf(".purple    {color: purple;}\n");
				printf(".cyan      {color: teal;}\n");
				printf(".white     {color: gray;}\n");
				printf(".bg-black  {background-color: black;}\n");
				printf(".bg-red    {background-color: red;}\n");
				printf(".bg-green  {background-color: green;}\n");
				printf(".bg-yellow {background-color: olive;}\n");
				printf(".bg-blue   {background-color: blue;}\n");
				printf(".bg-purple {background-color: purple;}\n");
				printf(".bg-cyan   {background-color: teal;}\n");
				printf(".bg-white  {background-color: gray;}\n");
			}
			else
			{
				printf(".dimgray     {color: dimgray;}\n");
				printf(".red       {color: red;}\n");
				printf(".green     {color: lime;}\n");
				printf(".yellow    {color: yellow;}\n");
				printf(".blue      {color: #3333FF;}\n");
				printf(".purple    {color: fuchsia;}\n");
				printf(".cyan      {color: aqua;}\n");
				printf(".white     {color: white;}\n");
				printf(".bg-black  {background-color: black;}\n");
				printf(".bg-red    {background-color: red;}\n");
				printf(".bg-green  {background-color: lime;}\n");
				printf(".bg-yellow {background-color: yellow;}\n");
				printf(".bg-blue   {background-color: #3333FF;}\n");
				printf(".bg-purple {background-color: fuchsia;}\n");
				printf(".bg-cyan   {background-color: aqua;}\n");
				printf(".bg-white  {background-color: white;}\n");
			}
			printf(".underline {text-decoration: underline;}\n");
			printf(".bold      {font-weight: bold;}\n");
			printf(".blink     {text-decoration: blink;}\n");
			printf("</style>\n");
		}
		if (word_wrap)
		{
			printf("<style type=\"text/css\">pre {white-space: pre-wrap; white-space: -moz-pre-wrap !important;\n");
			printf("white-space: -pre-wrap; white-space: -o-pre-wrap; word-wrap: break-word;}</style>\n");
		}
		printf("</head>\n");
		if (stylesheet || ! colorshema)
			printf("<body>\n");
		else
		{
			switch (colorshema)
			{
				case 1: printf("<body style=\"color:white; background-color:black\">\n"); break;
				case 2: printf("<body style=\"background-color:pink\">\n");	break;
			}
		}

		//default values:
		//printf("<div style=\"font-family:monospace; white-space:pre\">");
		printf("<pre>\n");
	}

	//Begin of Conversion
	unsigned int c;
	int fc = -1; //Standard Foreground Color //IRC-Color+8
	int bc = -1; //Standard Background Color //IRC-Color+8
	int ul = 0; //Not underlined
	int bo = 0; //Not bold
	int bl = 0; //No Blinking
	int ofc,obc,oul,obo,obl; //old values
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
			obl=bl;
			//Searching the end (a letter) and safe the insert:
			c='0';
			char buffer[1024];
			int counter=0;
			while ((c<'A') || ((c>'Z') && (c<'a')) || (c>'z'))
			{
				c=getNextChar(fp);
				//printf("JLPOOLEDEBUG %d: %c\n",__LINE__,c);
				buffer[counter]=c;
				if (c=='>') //end of htop
					break;
				counter++;
				if (counter>1022)
					break;
			}
			//printf("JLPOOLE %d: after while\n",__LINE__);
			buffer[counter-1]=0;
			pelem elem;
			int colorCode = 0;
			switch (c)
			{
				case 'm':

					//printf("\n[%d] Control Code: %s\n",__LINE__,buffer); //DEBUG
					elem=parseInsert(buffer);
					pelem momelem=elem;

					while (momelem!=NULL)
					{
						//jump over zeros
						int mompos=0;
						while (mompos<momelem->digitcount && momelem->digit[mompos]==0)
							mompos++;
						if (mompos==momelem->digitcount) //only zeros => delete all
						{
							bo=0;ul=0;bl=0;fc=-1;bc=-1;
						}
						else
						{
							// determkne the  Select Graphic Rendition) parameter
							//printf("JLPOOLE %d: momelem->digit = %d \n",__LINE__,
							//		momelem->digit[mompos]);
							switch (momelem->digit[mompos])
							{
								case 1: bo=1; break;
								case 2: if (mompos+1<momelem->digitcount)
												switch (momelem->digit[mompos+1])
												{
													case 1: //Reset blink and bold
														bo=0;
														bl=0;
														break;
													case 4: //Reset underline
														ul=0;
														break;
														case 7: //Reset Inverted
														temp = bc;
														if (fc == -1 || fc == 9)
														{
															if (colorshema!=1)
																bc = 0;
															else
																bc = 7;
														}
														else
															bc = fc;
														if (temp == -1 || temp == 9)
														{
															if (colorshema!=1)
																fc = 7;
															else
																fc = 0;
														}
														else
															fc = temp;
														break;
												}
												break;
				        // Case 3: set foreground color
						case 3: if (mompos+1 < momelem->digitcount){
										//printf("\nJLPOOLE %d fc = %d\n",__LINE__,fc);
										fc=momelem->digit[mompos+1];
										//printf("\nJLPOOLE %d digitcount = %d fc = %d mompos = %d\n",
										//		__LINE__,momelem->digitcount,fc,mompos);
								}
							            if (fc == 8){
							            	//printf("\nJLPOOLE %d: Flag of 8 \n",__LINE__);
							            	// we have an RGB flag, look to the next flag to determine
							            	// if 5 for 256 color table, or 2 for real RGB
							            	pelem momelem2=momelem->next;
							            	int mompos2 = 0;
							            	if (mompos2 < momelem2->digitcount){
							            		//printf("JLPOOLE %d: Have 3+ \n",__LINE__);
							            		int rgbFlag = momelem2->digit[mompos2];
							            		//printf("JLPOOLE %d: rgbFlag = %d \n",__LINE__,rgbFlag);
							            	    // peak ahead to get the color code starting at position +5
							            	    if (rgbFlag ==  5){
							            	    	// we have a color table [3/4_bit]
							            	    	//printf("\nJLPOOLE %d: We have a flag of 5 \n",__LINE__);
							            	    	pelem momelem3=momelem2->next;
							            	    	int mompos3 = 0;
							            	    	//printf("About to call createInteger...\n");
							            	    	colorCode = createIntegerFromStructure(momelem3);
							            	    	//printf("After calling createInteger...\n");
							            	    }
							            	} else {
							            		//printf("JLPOOLE %d: NO Flag of 5 \n",__LINE__);
							            	}
							            	// dispose of the RGB field lest it be read
							            	// at the beginning of this switch statement and
							            	// then misinterpreted as a color setting code
							            	momelem->next=NULL;

							            }
										break;
						// Case 4: set background color
						case 4: if (mompos+1==momelem->digitcount)
											ul=1;
										else
											bc=momelem->digit[mompos+1];
										break;
						case 5: bl=1; break;
						case 7: //TODO: Inverse
										temp = bc;
										if (fc == -1 || fc == 9)
										{
											if (colorshema!=1)
												bc = 0;
											else
												bc = 7;
										}
										else
											bc = fc;
										if (temp == -1 || temp == 9)
										{
											if (colorshema!=1)
												fc = 7;
											else
												fc = 0;
										}
										else
											fc = temp;
										break;
							}
						}
						momelem=momelem->next;
					}
					deleteParse(elem);
				break;
				case 'H':
					if (htop_fix) //a little dirty ...
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
							line_break=1;
					}
				break;
			}
			if (htop_fix)
				if (line_break)
				{
					for (;line<80;line++)
						printf(" ");
				}
			//Checking the differences
			if ((fc!=ofc) || (bc!=obc) || (ul!=oul) || (bo!=obo) || (bl!=obl)) //ANY Change
			{
				if ((ofc!=-1) || (obc!=-1) || (oul!=0) || (obo!=0) || (obl!=0))
					printf("</span>");
				if ((fc!=-1) || (bc!=-1) || (ul!=0) || (bo!=0) || (bl!=0))
				{
					if (stylesheet)
						printf("<span class=\"");
					else
						printf("<span style=\"");
					//printf("\nJLPOOLE %d: fc = %d \n",__LINE__,fc);
					char rgb[11];
					switch (fc)
					{
						case	0: if (stylesheet)
											 printf("dimgray ");
										 else
											 printf("color:dimgray;");
										 break; //Black
						case	1: if (stylesheet)
											 printf("red ");
										 else
											 printf("color:red;");
										 break; //Red
						case	2: if (stylesheet)
											 printf("green ");
										 else if (colorshema!=1)
											 printf("color:green;");
										 else
											 printf("color:lime;");
										 break; //Green
						case	3: if (stylesheet)
											 printf("yellow ");
										 else if (colorshema!=1)
											 printf("color:olive;");
										 else
											 printf("color:yellow;");
										 break; //Yellow
						case	4: if (stylesheet)
											 printf("blue ");
										 else if (colorshema!=1)
											 printf("color:blue;");
										 else
											 printf("color:#3333FF;");
										 break; //Blue
						case	5: if (stylesheet)
											 printf("purple ");
										 else if (colorshema!=1)
											 printf("color:purple;");
										 else
											 printf("color:fuchsia;");
										 break; //Purple
						case	6: if (stylesheet)
											 printf("cyan ");
										 else if (colorshema!=1)
											 printf("color:teal;");
										 else
											 printf("color:aqua;");
										 break; //Cyan
						case	7: if (stylesheet)
											 printf("white ");
										 else if (colorshema!=1)
											 printf("color:gray;");
										 else
											 printf("color:white;");
										 break; //White
						case    8:
                                // the "196" is a test, need to have a variable
							    // here that represents the N of the code 8 trigger.
						        make_rgb(colorCode,rgb);
						        printf("color: rgb(%s);",rgb);
						        break;

						case	9: if (stylesheet)
											 printf("reset ");
										 else if (colorshema!=1)
											 printf("color:black;");
										 else
											 printf("color:white;");
										 break; //Reset
					}
					switch (bc)
					{
						case	0: if (stylesheet)
											 printf("bg-black ");
										 else
											 printf("background-color:black;");
										 break; //Black
						case	1: if (stylesheet)
											 printf("bg-red ");
										 else
											 printf("background-color:red;");
										 break; //Red
						case	2: if (stylesheet)
											 printf("bg-green ");
										 else if (colorshema!=1)
											 printf("background-color:green;");
										 else
											 printf("background-color:lime;");
										 break; //Green
						case	3: if (stylesheet)
											 printf("bg-yellow ");
										 else if (colorshema!=1)
											 printf("background-color:olive;");
										 else
											 printf("background-color:yellow;");
										 break; //Yellow
						case	4: if (stylesheet)
											 printf("bg-blue ");
										 else if (colorshema!=1)
											 printf("background-color:blue;");
										 else
											 printf("background-color:#3333FF;");
										 break; //Blue
						case	5: if (stylesheet)
											 printf("bg-purple ");
										 else if (colorshema!=1)
											 printf("background-color:purple;");
										 else
											 printf("background-color:fuchsia;");
										 break; //Purple
						case	6: if (stylesheet)
											 printf("bg-cyan ");
										 else if (colorshema!=1)
											 printf("background-color:teal;");
										 else
											 printf("background-color:aqua;");
										 break; //Cyan
						case	7: if (stylesheet)
											 printf("bg-white ");
										 else if (colorshema!=1)
											 printf("background-color:gray;");
										 else
											 printf("background-color:white;");
										 break; //White
						case	9: if (stylesheet)
											 printf("bg-reset ");
										 else if (colorshema==1)
											 printf("background-color:black;");
										 else if (colorshema==2)
											 printf("background-color:pink;");
										 else
											 printf("background-color:white;");
										 break; //Reset
					}
					if (ul)
          {
						if (stylesheet)
							printf("underline ");
						else
							printf("text-decoration:underline;");
          }
					if (bo)
          {
						if (stylesheet)
							printf("bold ");
						else
							printf("font-weight:bold;");
          }
					if (bl)
          {
						if (stylesheet)
							printf("blink ");
						else
							printf("text-decoration:blink;");
          }

					printf("\">");
				}
			}
		}
		else
		if (c==13 && htop_fix)
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
			if (line_break)
			{
				printf("\n");
				line=0;
				line_break=0;
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
			switch (c)
			{
				case '&':	printf("&amp;"); break;
				case '\"': printf("&quot;"); break;
				case '<':	printf("&lt;"); break;
				case '>':	printf("&gt;"); break;
				case '\n':case 13: momline++;
									 line=0;
				default:	 printf("%c",c);
			}
			if (iso>0) //only at ISOS
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

	if (no_header == 0)
	{
		printf("</pre>\n");
		printf("</body>\n");
		printf("</html>\n");
	}

	if (fp!=stdin)
		fclose(fp);
	return 0;
}
