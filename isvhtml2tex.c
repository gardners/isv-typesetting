#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <strings.h>

FILE *f=NULL;

int parseText(char *file,off_t len,int bodyP);

int main(int argc,char **argv)
{
  if (!argv[1]) { fprintf(stderr,"usage: %s <file.html>\n",argv[0]); exit(1); }

  f=fopen(argv[1],"r");
  if (!f) { fprintf(stderr,"Could not open '%s' for reading\n",argv[1]); exit(1); }

  fseek(f,0,SEEK_END);
  off_t len =ftello(f);

  char *file = (char *)mmap(NULL,len,PROT_READ,MAP_SHARED,fileno(f),0);
  if (!file) { fprintf(stderr,"Failed to mmap() input file\n"); exit(1); }

  //   fprintf(stderr,"Loaded %lld bytes from file '%s'\n",(long long)len,argv[1]);
 
  return parseText(file,len,0 /* not yet in body */);
}

/* column of output is shared, regardless of nesting level */
int col=0;

int parseText(char *file,off_t len,int bodyP)
{
  int last_fn_ofs=0;
  int ofs=0;
  int inTagP=0;
  int inBodyP=bodyP;
  char *endParaText="";
  char *endSpanText="";

  // fprintf(stderr,"Parsing %lld bytes\n",(long long) len);

  while(ofs<len) {
    char c=file[ofs];

    int skipP=0;

    if (c=='<') inTagP=1;

#define IFMATCH(X) if (!strncmp(X,&file[ofs],strlen(X)))

    IFMATCH("</p>") { 
      printf("%s\n",endParaText); 
      col+=strlen(endParaText);
      endParaText=""; }
    IFMATCH("</span>") { 
      printf("%s",endSpanText); 
      col+=strlen(endSpanText); 
      endSpanText=""; }

    /* Italics */
    IFMATCH("<span class=\"T1\">") { 
      printf("{\\itshape{");
      col+=strlen("{\\itshape{");
      endSpanText="}}"; }
    IFMATCH("<span class=\"T2\">") { 
      printf("{\\itshape{");
      col+=strlen("{\\itshape{");
      endSpanText="}}"; }

    IFMATCH("<p class=\"P11\">") {
      printf("\\ShortTitle{");
      col+=strlen("\\ShortTitle{");
      endParaText="}"; 
    }
    IFMATCH("<p class=\"P21\">") {
      printf("\\ShortTitle{");
      col+=strlen("\\ShortTitle{");
      endParaText="}"; 
    }

    IFMATCH("&amp;") {
      printf("\\&");
      col+=2;
      ofs+=strlen("&amp;");
      skipP=1;
    }

    /* Chapter markers */
    IFMATCH("<p class=\"Heading_20_4\">Chapter ") { 
      printf("\\Chap{"); 
      /* Skip the rest of the tag and the word Chapter, so that we
	 can just get the chapter number. */
      ofs+=strlen("<p class=\"Heading_20_4\">Chapter ")-1; /* minus 1 because ofs gets incremented at the end of the loop */
      /* Clear the in-tag flag, because we want to grab the chapter number,
	 and we have matched the opening of this tag, and skipped over the closing
	 of the tag, so we need to manually update the state. */
      inTagP=0;
      /* but we don't want the < that opened the tag to be output, so mark this
	 character as a skip */
      skipP=1;
      endParaText="}";
    }
    /* Heading markers */
    IFMATCH("<p class=\"P27\">") {
      printf("\\Heading{");
      ofs+=strlen("<p class=\"P27\">")-1;
      endParaText="}";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<p class=\"P32\">") {
      printf("\\Heading{");
      ofs+=strlen("<p class=\"P32\">")-1;
      endParaText="}";
      skipP=1;
      inTagP=0;
    }
    /* Another variation of the headings */
    IFMATCH("<p class=\"Heading_20_5\">") {
      printf("\\Heading{");
      ofs+=strlen("<p class=\"Heading_20_5\">")-1;
      endParaText="}";
      skipP=1;
      inTagP=0;
    }
    /* Indication of parallel passage */
    IFMATCH("<p class=\"Heading_20_6\">") {
      printf("\\Parallel{");
      ofs+=strlen("<p class=\"Heading_20_6\">")-1;
      endParaText="}";
      skipP=1;
      inTagP=0;
    }

    IFMATCH("<span class=\"T4\">") {
      printf("{\\scshape{");
      ofs+=strlen("<span class=\"T4\">")-1;
      endSpanText="}}";
      skipP=1;
      inTagP=0;
    }


    /* Poetry/prose */
    /* First line: */
    IFMATCH("<p class=\"P1\">") {
      printf("\\PFL ");
      ofs+=strlen("<p class=\"P1\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"P1\">") {
      printf("\\PFL ");
      ofs+=strlen("<div class=\"P1\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"P2\">") {
      printf("\\PFL ");
      ofs+=strlen("<div class=\"P2\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<p class=\"P12\">") {
      printf("\\PFL ");
      ofs+=strlen("<p class=\"P12\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<p class=\"P13\">") {
      printf("\\PFL ");
      ofs+=strlen("<p class=\"P13\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<p class=\"P14\">") {
      printf("\\PFL ");
      ofs+=strlen("<p class=\"P14\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<p class=\"P15\">") {
      printf("\\PFL ");
      ofs+=strlen("<p class=\"P15\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<p class=\"P16\">") {
      printf("\\PFL ");
      ofs+=strlen("<p class=\"P16\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"Psalm_20_Line_20_3_20_Last_20_Line\">") {
      printf("\\PFL ");
      ofs+=strlen("<div class=\"Psalm_20_Line_20_3_20_Last_20_Line\">")-1;
      skipP=1;
      inTagP=0;
    }

    /* Second line: */
    IFMATCH("<p class=\"Psalm_20_Line_20_2\">") {
      printf("\\PNL ");
      ofs+=strlen("<p class=\"Psalm_20_Line_20_2\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"Psalm_20_Line_20_2\">") {
      printf("\\PNL ");
      ofs+=strlen("<div class=\"Psalm_20_Line_20_2\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"Psalm_20_Line_20_3\">") {
      printf("\\PNL ");
      ofs+=strlen("<div class=\"Psalm_20_Line_20_3\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"Psalm_20_Line_20_2_20_Last_20_Line\">") {
      printf("\\PNL ");
      ofs+=strlen("<div class=\"Psalm_20_Line_20_2_20_Last_20_Line\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }


    /* Back to prose mode */
    IFMATCH("<div class=\"P6\">") {
      printf("\\PPE ");
      ofs+=strlen("<div class=\"P6\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"P7\">") {
      printf("\\PPE ");
      ofs+=strlen("<div class=\"P7\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"P8\">") {
      printf("\\PPE ");
      ofs+=strlen("<div class=\"P8\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"Body_20_Block\">") {
      printf("\\PPE\\par ");
      ofs+=strlen("<div class=\"Body_20_Block\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<div class=\"Text_20_body\">") {
      printf("\\PPE\\par ");
      ofs+=strlen("<div class=\"Text_20_body\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }
    IFMATCH("<p class=\"Text_20_body\">") {
      printf("\\PPE\\par ");
      ofs+=strlen("<p class=\"Text_20_body\">")-1;
      endParaText="";
      skipP=1;
      inTagP=0;
    }

    /* Red letter */
    IFMATCH("<span class=\"RedLetter\">") {
      printf("{\\JFont{");
      col+=strlen("{\\JFont{");
      endSpanText="}}";
    }

    /* Verse numbers */
    IFMATCH("<span class=\"Verse\">1</span>") {
      /* Verse one */
      /* Leave the last '>' to be detected to exit inTag mode */
      ofs+=strlen("<span class=\"Verse\">1</span")-1;
      /* By default have a drop letter with the number of the chapter.
	 Use \VerseOne for no drop letter. */
      printf("\\VerseOneDrop\n");
    } else {
      IFMATCH("<span class=\"Verse\">") {
	/* Any other verse */
	if (col>60) { printf("\n"); col=0; }
	printf(" \\VerseNumber{");
	ofs+=strlen("<span class=\"Verse\">")-1;
	col+=strlen(" \\VerseNumber{")+2;
	skipP=1; inTagP=0;
	endSpanText="}";
      }
    }

    /* Footnotes.
       It seems that the footnotes are included in the reference as the title
       value.  This is convenient, because it means we don't need to go hunting
       the corresponding footnote node from the end of the document (at least
       unless we discover that some titles are incomplete or something nasty like
       that). */
    IFMATCH("<span class=\"Footnote_20_anchor\" title=\"Footnote: ") {
      {
	/* Find actual footnote body in end of file.  What we are dealing with here
	   is pairs of footnote references (which is where we are now), that look
	   something like:

	   <span class="Footnote_20_Symbol">
	   <span class="Footnote_20_anchor" title="Footnote: 1:1 Or birth">
	   <a href="#ftn2" id="body_ftn2">2</a></span></span>

	   And then the actual footnote itself later in the HTML file, that look
	   something like:

	   <p class="P25"><span class="footnodeNumber">
	   <a class="Footnote_20_Symbol" id="ftn2" href="#body_ftn2">2</a>
	   </span>1:1 Or <span class="T1">birth</span></p>

	   Note that the title in the footnote reference almost has what we need,
	   except that it lacks any formatting (in this case the italicisation of
	   the word "birth").  So we need to dig the id= tag out of the reference,
	   and then find the matching href="#<tag>" later in the document, and then
	   work out how much of that we need to interpret.  Annoyingly, we have to
	   do some substitution of that text as well, e.g., to get the italics back.

	   If we were really clever, we would also figure out how to make the Bible
	   references live links, but that is for another day.
	*/
	char id[256];
	int idlen,i;
	sprintf(id,"href=\"#"); idlen=strlen(id);
	/* Find id= tag and get footnote reference number */
	for(i=ofs;i<len;i++) if (!strncmp("id=\"",&file[i],4)) break;
	i+=4;
	for(;i<len&&idlen<256;i++) if (file[i]=='"') break; else id[idlen++]=file[i];
	if (idlen>200) idlen=200; 
	id[idlen++]='"';
	id[idlen]=0;
	fflush(stdout);
	// fprintf(stderr,"Looking for footnote '%s'\n",id);
	for(i=last_fn_ofs;i<len;i++)
	  if (!strncmp(id,&file[i],strlen(id)))
	    {
	      /* Bingo! Found the footnote.
		 Now find the extent of it that we need to parse */

	      /* skip over the string we were looking for */
	      i+=strlen(id);
	      /* and then skip to the end of the <a> tag */
	      for(;i<len;i++) 
		if (!strncmp("</a>",&file[i],4)) break;

	      /* Assume extends until next </p> */
	      // 	      fprintf(stderr,"Footnote: ");
	      int fn_len=0;
	      int fn_ofs=i;
	      last_fn_ofs=fn_ofs;
	      for(;i<len;i++) 
		if (!strncmp("</p>",&file[i],4)) break;
		else {
		  // fprintf(stderr,"%c",file[i]);
		  fn_len++;
		}
	      // fprintf(stderr,"\n");
	      fflush(stderr);

	      /* Now parse through it */
	      printf("\\FTNT{");
	      col+=strlen("\\FTNT{");	     
	      parseText(&file[fn_ofs],fn_len,1 /* already in body */);
	      printf("}");

	      break;
	    }
      }

      /* Find end of footnote anchor */
      for(;ofs<len;ofs++) if (!strncmp("</a>",&file[ofs],4)) break;
    }

    /* The footnotes are also duplicated at the end.  So once we hit the footnotes
       at the end, we can stop processing */
    IFMATCH("<span class=\"footnodeNumber\"") {
      ofs=len; skipP=1;
    }


    IFMATCH("</head>") { inBodyP=1; }

    /* Print only non-tag text */
    if (inBodyP&&!inTagP&&!skipP) {
      if (c<'\n'||c>0x7d) {
	unsigned int ec=
	  (((unsigned char)file[ofs])<<16)
	  +(((unsigned char)file[ofs+1])<<8)
	  +(((unsigned char)file[ofs+2])<<0);
	switch(ec) {
	  /* e2 80 9c = opening double quote */
	case 0xe2809c: printf("{``}"); ofs+=2; break;
	  /* e2 80 9d = closing double quote */
	case 0xe2809d: printf("{''}"); ofs+=2; break;
	  /* e2 80 98 = opening single quote */
	case 0xe28098: printf("{`}"); ofs+=2; break;
	  /* e2 80 99 = possessive apostrophe / closing single quote */
	case 0xe28099: printf("{'}"); ofs+=2; break;
	  /* em-dash */
	case 0xe28094: printf("---"); ofs+=2; break;
	  /* ellipsis */
	case 0xe280a6: printf("..."); ofs+=2; break;
	default:
	  /* skip invisible things */
	  if ((ec>>8)==0xc2a0) { ofs+=1; break; }
	  /* otherwise report the sequence */
	  printf("<Funny character sequence 0x%02x>",ec);
	}
      }
      else {
	/* insert line breaks between words so that the output is easier to edit */
	if (c==' '&&(col>65)) { printf("\n"); col=0; }
	else { printf("%c",c); col++; }
      }
    }

    if (c=='>') inTagP=0;
    ofs++;
  }

  return 0;
}
