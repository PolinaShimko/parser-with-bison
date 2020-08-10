%{
    #include <stdio.h>
	#include "string.h"
	#define alloca malloc
	int i;
	FILE *f, *f2;
%}

%union
{
struct MyHTML {
	char name[100];
	char attribute[50][100];
	int number;
} tagtype;
int flag_tag_start[113];
int flag_tag_end[113];
}

%token Single_Tag_No_Attribute  //одиночный тег без атрибутов
%token Pair_Tag_No_Attribute_Start  //начало парного тега без атрибутов
%token Single_Tag_With_Attribute //одиночный тег с атрибутами
%token Pair_Tag_Start_With_Attribute  //начало парного тега с атрибутами
%token Tag_End //конец парного тега


%type <tagtype> Single_Tag_No_Attribute
%type <tagtype> Pair_Tag_No_Attribute_Start
%type <tagtype> Single_Tag_With_Attribute
%type <tagtype> Pair_Tag_Start_With_Attribute
%type <tagtype> Tag_End

%type <tagtype> singletag startpairtag endpairtag

%%
list:
	| list stat 
	| list '\n'
	;
stat:
	singletag 
	| startpairtag list endpairtag ;
singletag:
	Single_Tag_No_Attribute
	{
		i = 0;
		while($1.name[i] > 0)
		{
			if (i < 100)
			{
				$$.name[i] = $1.name[i];
				i++;
			}
		}
		
	}
	| Single_Tag_With_Attribute
	{
		int k;
		i = 0;

		//тег

		while($1.name[i] > 0)
		{
			if (i < 100)
			{
				$$.name[i] = $1.name[i];
			
				i++;
			}
		};
	

		//атрибуты
		$$.number = $1.number;
		i = 0;
		for( k = 0; k < $1.number; k++)
		{
			i = 0;
			while($1.attribute[k][i] > 0)
			{
				if (i < 100)
				{
					$$.attribute[k][i] = $1.attribute[k][i];
					i++;
				}
			}

		}

		
    //обработка ошибок
		i = 0;
		while(i < $1.number)
			if($$.attribute[i] && !TestAttribute($$.name, $$.attribute[i]))
			{
				printf_s("ERROR: Invalid attribute '%s' for tag '%s'. ", $$.attribute[i], $$.name);
				print_error_line();
				i++;//break;
			}
			else
				i++;
	} 
	; 
startpairtag:
	Pair_Tag_No_Attribute_Start
	{
		i = 0;
		while($1.name[i] > 0)
		{
			if (i < 100)
			{
				$$.name[i] = $1.name[i];
				i++;
			}
		}

	}
	| Pair_Tag_Start_With_Attribute
	{
		int k;
		i = 0;

		//тег
		while($1.name[i] > 0 && i < 100)
		{
			$$.name[i] = $1.name[i];
			i++;
		};


		//атрибуты
		$$.number = $1.number;
		i = 0;


		for( k = 0; k < $1.number; k++)
		{
			i = 0;
			while($1.attribute[k][i] > 0)
			{
				if (i < 100)
				{
					$$.attribute[k][i] = $1.attribute[k][i];
				
					i++;
				}
			}
		}
		
		i = 0;

     //обработка всех атрибутов
		while(i < $$.number)
			if($$.attribute[i] && !TestAttribute($$.name, $$.attribute[i]))
			{
				printf_s("ERROR: Invalid attribute '%s' for tag '%s'. ", $$.attribute[i], $$.name);
				print_error_line();
				i++;//break;
			}
			else
				i++;
	} ;
endpairtag: Tag_End;

%% 
