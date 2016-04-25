/*
  Purpose    -       solution to Assignment 1 Comp20005 | SEM1 | 2016
  Author       -     Yash Narwal
  Stu_ID     -       612840
  Email      -       ynarwal@student.unimelb.edu.au
  
  Programming is fun :)

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_APARTS 100 /* Maximum number of apartments in the input file */
#define MAX_ROOMS  100 /* Maximum number of rooms in an apartment */

#define DIV "+-------+----------------+----------------+----------------+\n"
#define HDR "| Apart |    Dry areas   |    Wet areas   |  Utility areas |\n"

#define FILLER "---"

#define UNIT   "meteres^2"

#define MAX_CHAR  256

/*
  Different type of area, and
  their proportion as one type
*/
struct area_proportion {
  double dry;
  double wet;
  double utility;
  double per_dry;
  double per_wet;
  double per_utility;
};

double* combing_zones(double areas[], int rooms, int code_nums[], 
                      int room_nums[]);


struct area_proportion area_proportion(double area_for_apart[],
                                      int rooms,int code_numbers[]);

char* code_word(int code , char* code_string);

void print_apart_desc(double areas[], int rooms, int code_numbers[],
                    int room_numbers[], double widths[], double heights[]);

void print_stage_four(double areas[],int apartment_nums[],
                   int room_numbers[], int codes[],
                   int apartment_room[], int apartments,
                   double widths[], double heights[] ) ;

/*

  main program
    * read input steam
    * call print_stage_four to print output

*/

int
main(int argc, char *argv[])
{

  int apartment_number;
  int code_number, room_number, total = 0, apartments = 0;
  int code_numbers[MAX_ROOMS*MAX_ROOMS], room_numbers[MAX_ROOMS*MAX_ROOMS];
  double widths[MAX_ROOMS*MAX_ROOMS], heights[MAX_ROOMS*MAX_ROOMS], areas[MAX_ROOMS*MAX_ROOMS];
  double width, height, total_area;
  int apartment_numbers[MAX_APARTS];
  int rooms_per_aprtment = 0;
  int apartment_room[MAX_ROOMS*MAX_ROOMS];
  double area_per_apartment = 0;
  char line[MAX_CHAR];

  //read the line until it's end of input stream
  while(fgets(line, sizeof(line), stdin)){
    if(sscanf(line, "%d %d %lf %lf",
              &code_number,&room_number, &width, &height) == 4){
      // there are 2 integer and 2 double in the line
      code_numbers[total] = code_number;
      room_numbers[total] = room_number;
      widths[total]  = width;
      heights[total] = height;
      areas[total] = width * height;
      total_area += width * height;
      area_per_apartment += width*height;
      total++;
      rooms_per_aprtment++;

    }
    else{
      // only one integer in the line
      if(sscanf(line, "%d", &apartment_number) == 1){
        if(apartment_number != -1){
           apartment_numbers[apartments] = apartment_number;
           apartments++;
        }
        if(apartment_number == -1){
           apartment_room[apartments-1] = rooms_per_aprtment;
           area_per_apartment = 0;
           rooms_per_aprtment = 0;
        }
      }
    }
  }
  //calling the stage 4 to print out the summary 
  // and area proportion for each apartment
  print_stage_four(areas, apartment_numbers,room_numbers,  code_numbers,
                apartment_room, apartments, widths, heights);

  return 0;

}
// ##########################################################

/*
  1. print description of each apartment
  2. print summary of area section of each apartment
*/


// ##########################################################


void
print_stage_four(double areas[],int apartment_nums[],
                   int room_numbers[], int codes[],
                   int apartment_room[], int apartments,
                   double widths[], double heights[] ) {
   int i, index = 0, rooms, j, k = 0;
   char area_percentage[10];
  for(i = 0; i< apartments; i++){
    rooms = apartment_room[i];
    int new_codes[rooms];
    double new_areas[rooms], new_widths[rooms], new_heights[rooms];
    int new_rooms[rooms];

    k = 0;
    for(j = index; j < index + rooms; j++){
      new_areas[k] = areas[j];
      new_codes[k] = codes[j];
      new_widths[k] = widths[j];
      new_heights[k] = heights[j];
      new_rooms[k] = room_numbers[j];
      k++;
    }
    print_apart_desc(new_areas, rooms, new_codes,
                     new_rooms, new_widths, new_heights);

    index += rooms;
  }
  printf("\n");
  index = 0;
  k = 0;
  struct area_proportion area_proportions;
  printf(DIV);
  printf(HDR);
  printf(DIV);
  for(i = 0; i< apartments; i++){
    printf("|  %d  ",apartment_nums[i]);
    rooms = apartment_room[i];
    int new_codes[rooms];
    double new_areas[rooms];
    k = 0;
    for(j = index; j < index + rooms; j++){
      new_areas[k] = areas[j];
      new_codes[k] = codes[j];
      k++;
    }
    area_proportions = area_proportion(new_areas, rooms, new_codes);
    sprintf(area_percentage, "%0.1lf%%",area_proportions.per_dry);
    printf("|%7.1lf  %5s  ",area_proportions.dry, area_percentage);
    sprintf(area_percentage, "%0.1lf%%",area_proportions.per_wet);

    printf("|%7.1lf  %5s  ",area_proportions.wet,area_percentage);
    sprintf(area_percentage, "%0.1lf%%",area_proportions.per_utility);

    printf("|%7.1lf  %5s  |\n",area_proportions.utility,area_percentage);
    printf(DIV);

    index += rooms;
  }

  return;
}

// #########################################################################
/*
  calculate area proportion for an apartment for each category
*/
// ########################################################################
struct area_proportion
area_proportion(double area_for_apart[],int rooms,int code_numbers[]){
  int i;
  double temp_dry = 0.0, temp_wet = 0.0 , temp_utility = 0.0;
  double total = 0;
  for(i = 0; i < rooms; i++){
    if(code_numbers[i] == 1 || code_numbers[i] == 2
                            || code_numbers[i] == 3){
        //dry area
        temp_dry += area_for_apart[i];

    }
    else if(code_numbers[i] == 4 || code_numbers[i] == 5
                                 || code_numbers[i] == 6){
      // wet area
      temp_wet += area_for_apart[i];
    }
    else{
      //utility area
      temp_utility  += area_for_apart[i];
    }
    total += area_for_apart[i];
  }
  //initialize the area_proprtion variable
  struct area_proportion proportions;

  //assign the value to all part of struct
  proportions.dry = temp_dry;
  proportions.wet = temp_wet;
  proportions.utility = temp_utility;
  proportions.per_dry = (temp_dry/total)*100;
  proportions.per_wet = (temp_wet/total)*100;
  proportions.per_utility = (temp_utility/total)*100;
  return proportions;
}
// ########################################################################

/*
  assign zeroes to the areas where room are of same type and same number
  and return modifiled array of areas

*/


// ########################################################################

double*
combing_zones(double areas[], int rooms, int code_nums[], int room_nums[]){
  int i;
  for(i = 0; i < rooms-1; i++){
    if(code_nums[i] == code_nums[i+1] && room_nums[i] == room_nums[i+1]){
      areas[i+1] += areas[i];
      areas[i] = 0;
    }
  }
  return areas;
}
// ########################################################################


/*
  @args
    int areas[] - array of areas of rooms
    int rooms - numbers of room in the apartment
    int code_numbers[] - code numbers for an apartment
    int room_numbers[] - room numbers for a code numbers
    double widths[] - widths of all rooms
    double heights[] -heights of all rooms
  @output
    print description for an apartment
*/


// ########################################################################
void
print_apart_desc(double areas[], int rooms, int code_numbers[],
                 int room_numbers[], double widths[], double heights[]){
  areas = combing_zones(areas, rooms, code_numbers, room_numbers);
  int i;
  double total_area = 0;

  for(i = 0; i < rooms; i++){
    char* code_string = malloc(20 * sizeof(char));
    // assign 20 bytes of memory to the code string
    code_word(code_numbers[i], code_string);
    if(areas[i] == 0){
      printf("%-10s  %d  %0.2lf  %0.2lf  %s\n", code_string,
      room_numbers[i], widths[i], heights[i], FILLER);
    }
    else{
      printf("%-10s  %d  %0.2lf  %0.2lf  %0.2lf\n", code_string,
          room_numbers[i], widths[i], heights[i], areas[i]);
    }
    // free the memory assigned to code_string
    free(code_string);
    total_area += areas[i];
  }
  printf("%-26s %0.2lf %s\n\n","Total Area" , total_area, UNIT);
}
// ########################################################################

/*
  takes code as an integer, and char pointer
  set code_word accordingly

*/


// ########################################################################
char 
*code_word(int code, char* code_string){
  switch (code) {
    case 1:
      strcpy(code_string, "Hallway");
      break;
    case 2:
      strcpy(code_string, "Bedroom");
      break;
    case 3:
      strcpy(code_string, "Living");
      break;
    case 4:
      strcpy(code_string, "Bathroom");
      break;
    case 5:
      strcpy(code_string, "Kitchen");
      break;
    case 6:
      strcpy(code_string, "Laundry");
      break;
    case 7:
      strcpy(code_string, "Storage");
      break;
    case 8:
      strcpy(code_string, "Garage");
      break;
    case 9:
      strcpy(code_string, "Balcony");
      break;

  }
  return code_string;
}
// ########################################################################
//          END OF PROGRAM     AND     Programming is fun :)             // 
// ########################################################################


