#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
// Defines
#define MAX_ROOMS 100
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 20
#define MAX_PASSWORD_LENGTH 50
// Structs
struct Room
{
    char view[10];
    int room_number;
    int room_price;
    int is_available;
    char customer_name[MAX_NAME_LENGTH];
    char customer_phone[MAX_NAME_LENGTH];
    int reserve_number;
};
struct Customer
{

    char name[MAX_NAME_LENGTH];
    char phone[MAX_NAME_LENGTH];
};
// Global variables
struct Room room[MAX_ROOMS];
int nor = 0;//number_of_roomss
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SAVE DATA PART
void save_data(){
    
    FILE* book = fopen("booking.txt", "w");
    for(int i = 0; i < nor; i++){
        if(room[i].is_available == 0){
            fprintf(book, "room number %d is reserved by %s. phone: %s. reserve number: %d\n", 
                    room[i].room_number, room[i].customer_name, room[i].customer_phone, room[i].reserve_number);
        }
    }
    fclose(book);
}
//  LOAD DATA PART  first works with prices.txt second with booking.txt
void room_initialization(){

    FILE* rooms_ptr = fopen("prices.txt", "r");
    //three cycle for three floors
    for (int i = 0; i < 3; i++){
        // Skip lines until we find the first room 
        while (fscanf(rooms_ptr, " room number %d : %d$ (%[^)])",
                      &room[nor].room_number,
                      &room[nor].room_price,
                      room[nor].view) != 3)
        {
            // Skip the rest of the line if we didn't find a room
            fscanf(rooms_ptr, "%*[^\n]");
        }
        nor++;

        // Read all remaining rooms
        while (fscanf(rooms_ptr, " room number %d : %d$ (%[^)])",
                      &room[nor].room_number, 
                      &room[nor].room_price,
                      room[nor].view) == 3) 
                          {nor++;}
    }
    fclose(rooms_ptr);
}
void customers_reserves(){

    //int x = 0;
    int first, forth; char second[MAX_NAME_LENGTH]; char third[MAX_PHONE_LENGTH];
    for(int j = 0; j < nor; j++){room[j].is_available = 1;}    
    FILE* booking = fopen("booking.txt", "r");
    //
    if (booking == NULL) {
        printf("Error: Cannot open booking.txt!\n");
        return;
    }
    while(fscanf(booking, "room number %d is reserved by %[^.]. phone: %[^.]. reserve number: %d\n", 
                &first, second, third, &forth) == 4){
               int n;
               for(int j = 0; j < nor; j++){if(first == room[j].room_number){n = j; break;}}
               room[n].reserve_number = forth;
               strcpy(room[n].customer_name, second);
               strcpy(room[n].customer_phone, third);
               room[n].is_available = 0;
               //x++;
           }
    fclose(booking);
    //printf("x equals: %d", x);
}
//  CUSTOMER MENU
void make_reservation(struct Customer customer, int roomNumber){
    
    int Number;
    for(int i = 0; i < nor; i++){if(room[i].room_number == roomNumber){Number = i; break;}}
    if(room[Number].is_available == 0){printf("Room is already reserved!\n\n"); return;}
    room[Number].is_available = 0;
    strcpy(room[Number].customer_name, customer.name);
    strcpy(room[Number].customer_phone, customer.phone);
    printf("Room succesfuly reserved\n");
    srand(time(NULL));
    int code = rand();
    printf("You'r reserve code is: %d\n\n", code);
    room[Number].reserve_number = code;
}
void cancell_reservation(int reserveCode){

    for(int i = 0; i < nor; i++){
        if(room[i].reserve_number == reserveCode){
            room[i].is_available = 1; printf("Reserve succesfully cancelled\n"); break;}
        else {if(i == (nor-1)){printf("Wrong Reserve number\n\n");}} 
    }
}
//  MAIN FUNCTIONS
void change_password(char new_pass[]){
    FILE* newPass = fopen("admin_pass.txt", "w");
    fprintf(newPass, "Password : %s", new_pass);
    fclose(newPass);
}
int admin_login()
{

    // getting pass
    char password[MAX_PASSWORD_LENGTH];
    FILE *pass_ptr = fopen("admin_pass.txt", "r");
    fscanf(pass_ptr, "%*s %*c %s", password);
    fclose(pass_ptr);

    // checking pass
    char test_pass[MAX_PASSWORD_LENGTH];
    printf("Enter the admin password: ");
    scanf("%s", test_pass);
    if (strcmp(password, test_pass) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
struct Customer customer_login()
{
    struct Customer customer;
    printf("Enter yout name: ");
    scanf("%s", customer.name);
    printf("Enter your phone number: ");
    scanf("%s", customer.phone);

    return customer;
}
void admin_menu() { 

    printf("You entered to admin menu\n"); 
    int command;
    char new_pass[MAX_PASSWORD_LENGTH];
    long int income = 0;
fir:
    printf("\nAdmin menu:\nChange password: 1\nView reserves: 2\nIncome calculation: 3\nExit: 4\n");
    scanf("%d", &command);
    switch(command){
        case 1:
            printf("Enter the new pass: ");
            scanf("%s", new_pass);
            change_password(new_pass);
            break;
        case 2:
            for(int i = 0; i < nor; i++){
                if(room[i].is_available == 0){
                    printf("room number %d is reserved\n", room[i].room_number);
                }
            }
            break;
        case 3:
            for(int i = 0; i < nor; i++){
                if((room[i].is_available) == 0){income += room[i].room_price;}
            }
            printf("Today's income is: %ld$\n", income);
            break;
        case 4:
            return;
    }
    goto fir;
}
void customer_menu(struct Customer customer) {

    printf("\nYou entered customer menu\n"); 
    int dicision;
    int number; 
    int code; 
here:    
    printf("Customer menu:\nReservable rooms: 1\nReserve a room: 2\nCancell reservation: 3\nExit: 4\n");
    scanf("%d", &dicision);
    switch(dicision){
        case 1:
            for(int i = 0; i < nor; i++){
                if(room[i].is_available){
                    printf("Room number %d which is %s is available for %d$ per night\n",
                         room[i].room_number, room[i].view, room[i].room_price);}
            } break;
        case 2:
            printf("Enter number of room you want: ");
            scanf("%d", &number);
            make_reservation(customer, number);
            break;
        case 3:
            printf("Enter reserve number of reserve you want to cancell: ");
            scanf("%d", &code);
            cancell_reservation(code);
            break;
        case 4:
            return;
    }
    goto here;
}
//
int main(){
    //LOAD DATA PART
    room_initialization();//defines all room structs(number, view, price)
    customers_reserves();//defines rooms availablation
    // LOGIN PART
    int choice;
jump_here:    
    printf("\nMain menu: \nCustomer login : 1 \nAdmin login : 2 \nExit : 3 \n");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        customer_menu(customer_login());
        break;
    case 2:
        if (admin_login())
        {
            admin_menu();
        }
        else
        {
            printf("Wrong password.\n");
            goto jump_here;
        }
        break;
    case 3:
        return 0;
        break;
    }
    //SAVE DATA PART
    save_data();
    return 0;
}
