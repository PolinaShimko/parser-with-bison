typedef union
{
struct MyHTML {
	char name[100];
	char attribute[50][100];
	int number;
} tagtype;
int flag_tag_start[113];
int flag_tag_end[113];
} YYSTYPE;
#define	Single_Tag_No_Attribute	258
#define	Pair_Tag_No_Attribute_Start	259
#define	Single_Tag_With_Attribute	260
#define	Pair_Tag_Start_With_Attribute	261
#define	Tag_End	262


extern YYSTYPE yylval;
