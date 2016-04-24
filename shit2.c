#include <stdio.h>
#include <string.h>

#include <stdlib.h>

typedef enum  {
  INT, DOUBLE
} data_type;

struct area_proportion {
  double dry;
  double wet;
  double utility;
  double per_dry;
  double per_wet;
  double per_utility;
};
// struct apartment{
//   int rooms;
//   double total_area;
//   double rooms_widths[];
//   double rooms_hights[];
//   int codes[];
//   int room_numbers[];
// }

double*
combing_zones(double areas[], int rooms, int code_nums[], int room_nums[]);
struct area_proportion
area_proportion(double area_for_apart[],int rooms,int code_numbers[]);

char* code_word(int code);

void
print_apart_desc(double areas[], int rooms, int code_numbers[],
                      int room_numbers[], double widths[], double heights[]);

void summary_table(double areas[],int apartment_nums[],int room_numbers[], int codes[],
   int apartment_room[], int apartments, double widths[], double heights[] ) ;
///Note : Update total as index

int main(int argc, char *argv[])
{

  int apartment_number;

  // scanf("%d", &apartment_number);

  int code_number, room_number;
  int code_numbers[100];
  int room_numbers[100];
  double widths[100];
  double heights[100];
  double areas[100];
  int total = 0;
  int i;
  double width, height, total_area;
  int apartments = 0;
  int apartment_numbers[100];
  int rooms_per_aprtment = 0;
  int apartment_room[100];
  double area_per_apartment = 0;
  double areas_array[10];
  // printf("Apartment %d\n----------\n", apartment_number);
  // char c;
  char line[256];


  while(fgets(line, sizeof(line), stdin)){
    if(sscanf(line, "%d %d %lf %lf", &code_number,&room_number, &width, &height) == 4){
      // printf("Hello\n")
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

      if(sscanf(line, "%d", &apartment_number) == 1){
      if(apartment_number != -1){
         // printf("%d %d\n",total, rooms_per_aprtment);
         apartment_numbers[apartments] = apartment_number;
         apartments++;
      }
      if(apartment_number == -1){
         apartment_room[apartments-1] = rooms_per_aprtment;
         areas_array[apartments-1] = area_per_apartment;
         area_per_apartment = 0;
         rooms_per_aprtment = 0;
      }
    }
    }
  }

  // struct area_proportion a = area_proportion(areas, 11, code_numbers);

  // printf("%0.2lf %0.2lf \n", a.dry, a.per_dry );

  // printf("%d \n",code_numbers[0] );

  summary_table(areas, apartment_numbers,room_numbers,  code_numbers, apartment_room, apartments,
  widths, heights);

  return 0;

}

// double areas[], int rooms, int code_numbers[],
                      // int room_numbers[], double widths[], double heights[]

void summary_table(double areas[],int apartment_nums[],int room_numbers[], int codes[],
   int apartment_room[], int apartments, double widths[], double heights[] ) {
   int i;
   int index = 0;
   int rooms;
   int j,k = 0;
   for(i = 0; i< apartments; i++){

     rooms = apartment_room[i];
     int new_codes[rooms];
     double new_areas[rooms];
     double new_widths[rooms];
     double new_heights[rooms];
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
     print_apart_desc(new_areas, rooms, new_codes, new_rooms, new_widths, new_heights);

     index += rooms;
   }
   printf("\n");
   index = 0;
   k = 0;
   struct area_proportion a;
   printf("+-------+----------------+----------------+----------------+\n");
   printf("| Apart |    Dry areas   |    Wet areas   |  Utility areas |\n");
   printf("+-------+----------------+----------------+----------------+\n");
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
    a = area_proportion(new_areas, rooms, new_codes);
    printf("|  %4.1lf  %03.1lf%%   ",a.dry, a.per_dry );
    printf("|  %4.1lf  %03.1lf%%   ",a.wet, a.per_wet );
    printf("|  %4.1lf  %03.1lf%%   |\n",a.utility, a.per_utility );
    printf("+-------+----------------+----------------+----------------+\n");

    index += rooms;
  }

  return;
}
void *copy_apartment(void* array,int start,  int end, int size, data_type type ){
  int i = 0;
  int j = 0;
  if(type==0){
    int* new_array =  malloc(end-start+1 * sizeof(int));
    for(i = start; i < end; i++){
      new_array[j] = ((int*)array)[i];
      j++;
    }
    return new_array;
  }
  else if(type == 1){
    double* new_array = malloc(end-start+1 * sizeof(double));
    for(i = start; i < end; i++){
      new_array[j] = ((double*)array)[i];
      j++;
    }
    return new_array;
  }


}


struct area_proportion
area_proportion(double area_for_apart[],int rooms,int code_numbers[]){
  int i;
  double temp_dry = 0.0, temp_wet = 0.0 , temp_utility = 0.0;
  double total = 0;
  for(i = 0; i < rooms; i++){
      if(code_numbers[i] == 1 || code_numbers[i] == 2 || code_numbers[i] == 3){
        //
        temp_dry += area_for_apart[i];

      }
      else if(code_numbers[i] == 4 || code_numbers[i] == 5 || code_numbers[i] == 6){
        //
        temp_wet += area_for_apart[i];
      }
      else{
        temp_utility  += area_for_apart[i];
      }

      total += area_for_apart[i];
  }

  struct area_proportion proportions;
  proportions.dry = temp_dry;
  proportions.wet = temp_wet;
  proportions.utility = temp_utility;
  proportions.per_dry = (temp_dry/total)*100;
  proportions.per_wet = (temp_wet/total)*100;
  proportions.per_utility = (temp_utility/total)*100;
  return proportions;
}
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



void
print_apart_desc(double areas[], int rooms, int code_numbers[],
                      int room_numbers[], double widths[], double heights[])
  {
    areas = combing_zones(areas, rooms, code_numbers, room_numbers);
    int i;
    double total_area = 0;
    char code_string[20];
    for(i = 0; i < rooms; i++){
      strcpy(code_string, code_word(code_numbers[i]));
      if(areas[i] == 0){
      printf("%-10s  %d  %0.2lf  %0.2lf  %s\n", code_string, room_numbers[i], widths[i], heights[i], "---");

      }
      else{
      printf("%-10s  %d  %0.2lf  %0.2lf  %0.2lf\n", code_string, room_numbers[i], widths[i], heights[i], areas[i]);
      }
      total_area += areas[i];
    }

    printf("%-26s %0.2lf %s\n","Total Area" , total_area, "meteres^2");
    printf("\n");
  }





char *code_word(int code){
  char *code_string = malloc(20);
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
