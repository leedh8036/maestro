#define SOCKET int
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<time.h>
#include<sys/types.h>
//#include<unistd.h>
//#include<errno.h>
#include "/usr/include/mysql/mysql.h"


#define SDI     0
#define RCLK    1
#define SRCLK   2

unsigned long long int LED[61] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,
0x8000,0x10000,0x20000,0x40000,0x80000,0x100000,0x200000,0x400000,0x800000,0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000,
0x100000000,0x200000000,0x400000000,0x800000000,0x1000000000,0x2000000000,0x4000000000,0x8000000000,0x10000000000,0x20000000000,0x40000000000,0x80000000000,
0x100000000000,0x200000000000,0x400000000000,0x800000000000,0x1000000000000,0x2000000000000,0x4000000000000,0x8000000000000,
0x10000000000000,0x20000000000000,0x40000000000000,0x80000000000000,0x100000000000000,0x200000000000000,0x400000000000000,0x800000000000000,0x1000000000000000};

void pulse(int pin)
{
        digitalWrite(pin, 0);
        delay(0.001);
        digitalWrite(pin, 1);
}


void SIPO(unsigned long long int byte)
{
        int i;

        for(i=0;i<61;i++){
                digitalWrite(SDI, ((byte & (0x1000000000000000 >> i)) >0));
                //digitalWrite(SDI, 0x1000000000000000 & (byte << i));
                pulse(SRCLK);
        }
}

void init(void)
{
        pinMode(SDI, OUTPUT);
        pinMode(RCLK, OUTPUT);
        pinMode(SRCLK, OUTPUT);

        digitalWrite(SDI, 0);
        digitalWrite(RCLK, 0);
        digitalWrite(SRCLK,0);
}

void wait(double seconds)
{
        clock_t endwait;
        endwait = clock() + seconds * CLOCKS_PER_SEC;
        while(clock() < endwait){}
}

int main(int argc, char *argv[]){

//pid_t pid;
//pid = fork();

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

char *server="35.161.154.86";
char *user="root";
char *password="dong8036";
char *database=argv[1];

int i;
int tmp=9999;

if(wiringPiSetup() == -1){
        printf("setup wiringPi failed !");
        return 1;
}

init();

if(!(conn=mysql_init((MYSQL*)NULL))){
printf("init fail\n");
exit(1);
}

printf("mysql_init success.\n");

if(!mysql_real_connect(conn, server, user,password, NULL, 3306,NULL, 0)){
printf("connect error.\n");
exit(1);
}

printf("mysql_real_connect suc.\n");

if(mysql_select_db(conn, database) != 0){
mysql_close(conn);
printf("select_db rail.\n");
exit(1);
}
printf("select mydb suc.\n");

//printf("%d", mysql_query(conn, "select*from testtab"));

char selectbuffer[256];
sprintf(selectbuffer,"select * from %s",argv[2]);
if(mysql_query(conn,selectbuffer)){
printf("query fail\n");
exit(1);
}

printf("query success\n");

res=mysql_store_result(conn);
printf("res success\n");

/*if(pid == -1){
        printf("can`t fork,erro\n");
        exit(0);
}
*/

//printf("%#x\n",(int)LED[6]);

while((row=mysql_fetch_row(res))!=NULL){
//printf("%d %f %d\n", atoi(row[0]),atof(row[1]),atoi(row[2]));
//printf("%d %f %f %d %d\n",atoi(row[0]),atof(row[1]),atof(row[2]),atoi(row[3]),atoi(row[4]));
//printf("%d %f %f %d %d %d\n",atoi(row[0]),atof(row[1]),atof(row[2]),atoi(row[3]),atoi(row[4]),atoi(row[5]));
printf("%f %d %d %d %f %d %d %d\n",atof(row[0]),atoi(row[1]),atoi(row[2]), atoi(row[3]), atof(row[4]), atoi(row[5]), atoi(row[6]), atoi(row[7]));
/*if(tmp != atoi(row[2])){
        SIPO(LED[atoi(row[2])]);
        pulse(RCLK);
        delay(atof(row[1])*1000);
        SIPO(0x00);
        pulse(RCLK);
        delay(100);
        //wait(atof(row[1])*2);
}
else if(tmp == atoi(row[2])){
        SIPO(LED[atoi(row[2])]);
        pulse(RCLK);
        delay(atof(row[1])*1000);
        //wait(atof(row[1])*2);
        //SIPO(0x0000000000000000);
        SIPO(0x00);
        pulse(RCLK);
        delay(100);
}
*/
//printf("%#x\n",(unsigned long)(LED[atoi(row[2])]+LED[atoi(row[3])]+LED[atoi(row[1])]));
//if(pid == 0){
SIPO(LED[atoi(row[1])]+LED[atoi(row[2])]+LED[atoi(row[3])]);
pulse(RCLK);
delay(atof(row[0])*1000);
SIPO(0x00);
pulse(RCLK);
delay(100);
//}
/*else{
SIPO(LED[atoi(row[5])]+LED[atoi(row[6])]+LED[atoi(row[7])]);
pulse(RCLK);
delay(atof(row[4])*1000);
SIPO(0x00);
pulse(RCLK);
delay(100);
}
*/

tmp = atoi(row[2]);

}

SIPO(0x0000000000000000);
pulse(RCLK);

mysql_close(conn);



return 0;
}




