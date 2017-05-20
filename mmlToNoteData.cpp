#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "/usr/include/mysql/mysql.h"

using namespace std;

int defatultPinaoNumber(int);// 디폴트 옥타브  건반번호 지정하는 함수
int octavePianoNumber(int , int ); // 옥타브가 바뀔 때 건반 번호 지정하는 함수
void highStoreRecord(char[]); //하나하나 저장한 배열을 입력받아 디비형식으로 저장하는 함수. - 높은음자리표용
void lowStoreRecord(char[]); // 낮은음자리표용


float musicRecord[1000][5];//DB형식으로 저장하기 위한 배열



int main(int argc, char *argv[])
{
	ifstream inStream;

	inStream.open("CHOPSTICKS_mml.txt");
	if (inStream.fail())
	{
		cerr << "mml 파일을 불러 오지 못했습니다.\n";
		exit(1);
	}


	string track1 = ""; 
	inStream >> track1; //첫번째 트랙(오른손) mml 입력

	string track2 = "";
	inStream >> track2; //두번째 트랙(왼손) mml 입력

	cout << track1 <<endl; //mml string이 트랙에 맞게 들어갔나 확인
	cout << track2 <<endl;


	char track1Arr[200]; //첫번째 트랙의 문자를 하나하나 저장할 배열
	char track2Arr[200]; //두번쨰 트랙의 문자를 하나하나 저장할 배열

	for(int m =0; m<200; m++)
	{
		track1Arr[m] = 'U'; // 각배열은 'U'로 초기화
		track2Arr[m] = 'U';
		
	}
	
	strcpy(track1Arr,track1.c_str()); // 입력받은 트랙을 char형 배열에 문자 하나하나를 각각 저장
	strcpy(track2Arr,track2.c_str());

	for(int h = 0; h<strlen(track1Arr); h++) //맞게 들어갔는지 확인
	{
		cout << track1Arr[h]  <<" " <<endl;
	}
	for(int h = 0; h<strlen(track2Arr); h++)
	{
		cout << track2Arr[h]  <<" " <<endl;
	}


	/*
	for(int n = 0; n<1000; n++)
	{
		// 각배열은 -1로 초기화.
		musicRecord[n][0] = -1;  //note_order
		musicRecord[n][1] = -1;  //right_note_time
		musicRecord[n][2] = -1;  //right_note_tune1
		musicRecord[n][3] = -1;  //right_note_tune2
		musicRecord[n][4] = -1;  //right_note_tune3
		musicRecord[n][5] = -1;  //right_note_tune4
		musicRecord[n][6] = -1;  //left_note_time
		musicRecord[n][7] = -1;  //left_note_tune1
		musicRecord[n][8] = -1;  //left_note_tune2
		musicRecord[n][9] = -1;  //left_note_tune3
		musicRecord[n][10] = -1;  //left_note_tune4
	}
	*/
	for(int n = 0; n<1000; n++)
        {
                // 각배열은 -1로 초기화.
                musicRecord[n][0] = -1;  //order
                musicRecord[n][1] = -1;  //right_note_time
                musicRecord[n][2] = -1;  //right_note_tune
                musicRecord[n][3] = -1;  //left_note_time
                musicRecord[n][4] = -1;  //left_note_tune
        }

	highStoreRecord(track1Arr); // 트랙1 높은 음자리표 포맷으로 저장
	lowStoreRecord(track2Arr); // 트랙2 낮은 음자리표 포맷으로 저장

	//DB에 들어가게 될 배열 확인
	
	/*
	for(int i = 0; i<strlen(track1Arr); i++)
	{
		cout << "  note_order : "      <<musicRecord[i][0]<< 
				"  right_note_time :  "<<musicRecord[i][1]<<
				"  right_note_tune1 : "<<musicRecord[i][2]<<
				"  right_note_tune2 : "<<musicRecord[i][3]<<
				"  right_note_tune3 : "<<musicRecord[i][4]<<
				"  right_note_tune4 : "<<musicRecord[i][5]<<
				"  left_note_time   : "<<musicRecord[i][6]<<
				"  left_note_tune1  : "<<musicRecord[i][7]<<
				"  left_note_tune2  : "<<musicRecord[i][8]<<
				"  left_note_tune3  : "<<musicRecord[i][9]<<
				"  left_note_tune4  : "<<musicRecord[i][10]<<endl;
	}
	*/

	for(int i = 0; i<strlen(track1Arr); i++)
        {
                cout << " note_order : "<<  musicRecord[i][0]  << "  note_right_time : "<< musicRecord[i][1] << "  note_right_tune : "<<musicRecord[i][2]<<
                        " note_left_time : "<<musicRecord[i][3]<<" note_left_tune : "<<musicRecord[i][4]<<endl;
        }

	inStream.close();

	//MySQL 연동

        MYSQL *conn_ptr;
        MYSQL_RES *sql_result;
        MYSQL_ROW sql_row;


        conn_ptr = mysql_init(NULL);

        if(!conn_ptr)
        {
                fprintf(stderr, "mysql_init failed\n");
                return -1;
        }

        conn_ptr = mysql_real_connect(conn_ptr, "35.161.154.86", "root", "dong8036", argv[1], 0, NULL, 0
);

       //create table

	char createbuffer[256];
	sprintf(createbuffer, "CREATE TABLE IF NOT EXISTS %s(note_order float NOT NULL, note_time_right float NOT NULL, note_tune_right float NOT NULL, note_time_left float NOT NULL, note_tune_left float NOT NULL)",argv[2]);
	mysql_query(conn_ptr,createbuffer);

        //insert query

        int len = sizeof(musicRecord) / sizeof(musicRecord[0]);
        char insertbuffer[256];
        for(int i =0; i< len; i++)
        {
        if(musicRecord[i][0] != -1)
        {
                sprintf(insertbuffer,"INSERT INTO %s(note_order, note_time_right, note_tune_right, note_time_left, note_tune_left) VALUES(%f,%f,%f,%f,%f)",argv[2],musicRecord[i][0],musicRecord[i][1],musicRecord[i][2],musicRecord[i][3],musicRecord[i][4]);
        
		mysql_query(conn_ptr,insertbuffer);
        }
        }


        mysql_close(conn_ptr);

        return 0;
}





int defaultPianoNumber(int alphabet) // 디폴트옥타브(4) 일때 C,D,E,F,G,H,A,B,C에 해당하는 피아노건반
{
	//알파벳을 아스키코드로
		int pianoNumber = 0;

		if(alphabet == 67) // C 도
		{
			pianoNumber = 13;
		}
		else if(alphabet == 68)// D 레
		{
			pianoNumber = 15;
		}
		else if(alphabet == 69)// E 미
		{
			pianoNumber = 17;
		}
		else if(alphabet == 70)// F 파
		{
			pianoNumber = 18;
		}
		else if(alphabet == 71)// G 솔
		{
			pianoNumber = 20;
		}
		else if(alphabet == 65)// A 라
		{
			pianoNumber = 22;
		}
		else if(alphabet == 66 )// B 시
		{
			pianoNumber = 24;
		}

		return pianoNumber;
}
	

int octavePianoNumber(int currentOctave, int alphabet) // 알파벳을 읽고 옥타브 값에 맞는 건반 번호를 리턴하는 함수
{
	int pianoNumber2 = 0;
	
	if(currentOctave == 3) // 옥타브가 3 일떄
	{
		pianoNumber2 = defaultPianoNumber(alphabet) - (12*1);
	}
	else if(currentOctave == 4 ) // 옥타브 4 일 때
	{
		pianoNumber2 = defaultPianoNumber(alphabet); // 디폴트는 그대로 디폴트 피아노건반을 따라가고 한옥타브의 건반숫자는 12차이.
	}
	else if(currentOctave == 5) // 옥타브 5 일 때
	{
		pianoNumber2 = defaultPianoNumber(alphabet) + (12*1);
	}
	else if(currentOctave == 6) // 옥타브 6 일 때
	{
		pianoNumber2 = defaultPianoNumber(alphabet) + (12*2);
	}
	else if(currentOctave == 7) // 옥타브 7 일 때
	{
		pianoNumber2 = defaultPianoNumber(alphabet) + (12*3);
	}

	return pianoNumber2;
}

void highStoreRecord(char trackArr[])
{
	int order = 0; // 음표 순서 초기값은 0
	int indexMusicRecord = 0; // musicRecord배열을 탐색하기 위한 인덱스

	char currentTimeChar = '4'; // 박자, 디폴트는 4 
	int currentTimeInt = 4;

	char currentOctaveChar ='4'; // 옥타브 , 디폴트는 4
	int currentOctaveInt = 4; 

	//track1Arr의 문자를 보고 musicRecord에 저장.
	for(int j = 0; j<strlen(trackArr); j++)
	{
		//cout<< "j 값 : "<< j <<endl;
		if(trackArr[j] == 'T') // 템포 철자 'T'를 만나면 그다음 배열의 인덱스값 3개는 템포 값 ,템포는 필요하지 않으므로 따로 저장하지 않음
		{
			cout << "find T : "<<  trackArr[j+1]<<trackArr[j+2]<<trackArr[j+3]  <<endl;
			
		}

		else if(trackArr[j] == 'L') // 박자 철자 'L'을 만나면 그다음 배열의 인덱스값은 박자 값에 저장.
		{
			cout << "find L : "<< trackArr[j+1] <<endl;
			currentTimeChar = trackArr[j+1];
			
		}
		else if(trackArr[j] == 'O') // 옥타브 철자 'O'를 만나면 그다음 배열의 인덱스값은 옥타브 값에 저장.
		{
			cout << "find O : "<<  trackArr[j+1]  <<endl;
			
			currentOctaveChar = trackArr[j+1];

			
			if(currentOctaveChar == '1') //char을 DB에 저장하기위해 int로 변환
			{
				currentOctaveInt = 1;
			}
				
			else if(currentOctaveChar == '2')
			{
				currentOctaveInt = 2;
			}
				
			else if(currentOctaveChar == '3')
			{
				currentOctaveInt = 3;
			}
				
			else if(currentOctaveChar == '4')
			{
				currentOctaveInt = 4;
			}

			else if(currentOctaveChar == '5')
			{
				currentOctaveInt = 5;
			}

			else if(currentOctaveChar == '6')
			{
				currentOctaveInt = 6;
			}

			else if(currentOctaveChar == '7')
			{
				currentOctaveInt = 7;
			}

			else if(currentOctaveChar == '8')
			{
				currentOctaveInt = 8;
			}
			

		}
		else if(trackArr[j] == '<') // '<'를 만나면 옥타브 -1
		{
			currentOctaveInt = currentOctaveInt -1;
		}

		else if(trackArr[j] == '>') // '>'를 만나면 옥타브 +1
		{
			currentOctaveInt = currentOctaveInt +1;
		}
		else if(trackArr[j] == 'C' || trackArr[j] == 'D' || trackArr[j] == 'E' || trackArr[j] == 'F' || trackArr[j] == 'G' || trackArr[j] == 'A' || trackArr[j] == 'B')  // 그외 C,D,E,F,G,A,B 값들
		{

			int pianoNum = octavePianoNumber(currentOctaveInt, int(trackArr[j])); //현재 옥타브에맞는 알파벳에 해당하는 건반을 구해 pianoNum에 저장한다.

			if(currentTimeChar == '2') //char을 DB에 저장하기위해 int로 변환
			{
				currentTimeInt = 2;
			}
				
			else if(currentTimeChar == '4')
			{
				currentTimeInt = 4;
			}
				
			else if(currentTimeChar == '8')
			{
				currentTimeInt = 8;
			}
				
//			else if(currentTimeChar == '16')
//			{
//				currentTimeInt = 16;
//			}
			
			cout << "find " << trackArr[j] <<endl;
			cout << "currentTime : " << currentTimeInt << endl;
			cout << "pianoNum : " << pianoNum << endl;


			musicRecord[indexMusicRecord][0] = order; // 음표 순서 저장
			order ++;
			musicRecord[indexMusicRecord][1] = currentTimeInt; // 현재 박자 저장
			musicRecord[indexMusicRecord][2] = pianoNum; // 건반 번호 저장
			
			indexMusicRecord ++;


			if(currentTimeInt == 2) //2분음표면 index 2개씩 증가.
			{
			indexMusicRecord ++;
			}
		}
	}
}


void lowStoreRecord(char trackArr[])
{
	int order = 0; // 음표 순서 초기값은 0
	int indexMusicRecord = 0; // musicRecord배열을 탐색하기 위한 인덱스

	char currentTimeChar = '4'; // 박자, 디폴트는 4 
	int currentTimeInt = 4;

	char currentOctaveChar = '4'; // 옥타브 , 디폴트는 4
	int currentOctaveInt = 4;

	//track1Arr의 문자를 보고 musicRecord에 저장.
	for(int j = 0; j<strlen(trackArr); j++)
	{
		//cout<< "j 값 : "<< j <<endl;
		if(trackArr[j] == 'T') // 템포 철자 'T'를 만나면 그다음 배열의 인덱스값 3개는 템포 값 ,템포는 필요하지 않으므로 따로 저장하지 않음
		{
			cout << "find T : "<<  trackArr[j+1]<<trackArr[j+2]<<trackArr[j+3]  <<endl;
			
		}

		else if(trackArr[j] == 'L') // 박자 철자 'L'을 만나면 그다음 배열의 인덱스값은 박자 값에 저장.
		{
			cout << "find L : "<< trackArr[j+1] <<endl;
			currentTimeChar = trackArr[j+1];
			
		}
		else if(trackArr[j] == 'O') // 옥타브 철자 'O'를 만나면 그다음 배열의 인덱스값은 옥타브 값에 저장.
		{
			cout << "find O : "<<  trackArr[j+1]  <<endl;
			
			currentOctaveChar = trackArr[j+1];

			
			if(currentOctaveChar == '1') //char을 DB에 저장하기위해 int로 변환
			{
				currentOctaveInt = 1;
			}
				
			else if(currentOctaveChar == '2')
			{
				currentOctaveInt = 2;
			}
				
			else if(currentOctaveChar == '3')
			{
				currentOctaveInt = 3;
			}
				
			else if(currentOctaveChar == '4')
			{
				currentOctaveInt = 4;
			}

			else if(currentOctaveChar == '5')
			{
				currentOctaveInt = 5;
			}

			else if(currentOctaveChar == '6')
			{
				currentOctaveInt = 6;
			}

			else if(currentOctaveChar == '7')
			{
				currentOctaveInt = 7;
			}

			else if(currentOctaveChar == '8')
			{
				currentOctaveInt = 8;
			}
			

		}
		else if(trackArr[j] == '<') // '<'를 만나면 옥타브 -1
		{
			currentOctaveInt = currentOctaveInt -1;
		}

		else if(trackArr[j] == '>') // '>'를 만나면 옥타브 +1
		{
			currentOctaveInt = currentOctaveInt +1;
		}
		else if(trackArr[j] == 'C' || trackArr[j] == 'D' || trackArr[j] == 'E' || trackArr[j] == 'F' || trackArr[j] == 'G' || trackArr[j] == 'A' || trackArr[j] == 'B')  // 그외 C,D,E,F,G,A,B 값들
		{

			int pianoNum = octavePianoNumber(currentOctaveInt, int(trackArr[j])); //현재 옥타브에맞는 알파벳에 해당하는 건반을 구해 pianoNum에 저장한다.

			if(currentTimeChar == '2') //char을 DB에 저장하기위해 int로 변환
			{
				currentTimeInt = 2;
			}
				
			else if(currentTimeChar == '4')
			{
				currentTimeInt = 4;
			}
				
			else if(currentTimeChar == '8')
			{
				currentTimeInt = 8;
			}
				
//			else if(currentTimeChar == '16')
//			{
//				currentTimeInt = 16;
//			}
			
			cout << "find " << trackArr[j] <<endl;
			cout << "currentTime : " << currentTimeInt << endl;
			cout << "pianoNum : " << pianoNum << endl;


			//musicRecord[indexMusicRecord][0] = order; // 음표 순서 저장
			//if(currentTimeInt == 4)
			
			order ++;
			

			musicRecord[indexMusicRecord][3] = currentTimeInt; // 현재 박자 저장
			musicRecord[indexMusicRecord][4] = pianoNum; // 건반 번호 저장
			
			indexMusicRecord ++;

			if(currentTimeInt == 2) //2분음표면 index 2개씩 증가.
			{
			indexMusicRecord ++;
			}
		}
	}
}
