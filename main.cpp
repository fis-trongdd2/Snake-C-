#include<iostream>
#include<time.h>
#include<windows.h>
#include<fstream>
#define MAX 100
#define xacsuat 5

using namespace std;
struct point{
	int x; int y;
	int ox; int oy;
};

typedef struct point Point;
int hang,cot,diemyeucau;
Point ran[MAX],huongdi;
int dodai,tocdo;
bool ketthuc;
char sanchoi[MAX][MAX];

//ham suu tam : dung de dua con tro den toa do (x, y) 
void gotoxy(short x, short y)
{
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x,y};
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput , Cursor_an_Pos);
}

//lay du lieu vao va khoi tao 1 so gia tri ban dau
void khoitao(int n){
	ketthuc = false;
	ifstream in_stream;
	if(n==1){
		in_stream.open("level1.txt");
		tocdo = 200;
		diemyeucau = 6;
	}
	if(n==2){
		in_stream.open("level2.txt");
		tocdo = 150;
		diemyeucau = 7;
	}
	if(n==3){
		in_stream.open("level3.txt");
		tocdo = 100;
		diemyeucau = 8;
	}
	if(n==4){
		in_stream.open("level4.txt");
		tocdo = 50;
		diemyeucau = 9;
	}	
	if(n==5){
		in_stream.open("level5.txt");
		diemyeucau = 10;
		tocdo=0;
	}
	in_stream >> hang >> cot;
	in_stream >> huongdi.x >> huongdi.y;
	char ch;
	int i,j;
	in_stream.get(ch);
	for ( i=0; i < hang; i++){
		for ( j=0; j<cot; j++){
			in_stream.get(ch);
			while (ch=='\n') 
				in_stream.get(ch);
			sanchoi[i][j]=ch;
			if(sanchoi[i][j]=='*'){
				ran[0].x = i; 
				ran[0].y = j;
			}
		}
	}
	in_stream.close();
	dodai = 1;
}

//di chuyen co the chu ran
void dichuyen(Point huong){
	int i,j,a,b;
	
	//di chyen dau ran
	ran[0].ox = ran[0].x; 
	ran[0].oy = ran[0].y;
	ran[0].x += huong.y;
	ran[0].y += huong.x;
	
	// khi ran an trung moi
	if(sanchoi[ran[0].x][ran[0].y]=='$'){
 		ran[dodai].x = ran[dodai - 1].ox;
		ran[dodai].y = ran[dodai - 1].oy;
		dodai++;
		if(dodai==diemyeucau+1) 
			ketthuc=true;
		do{
			a = rand()%hang;
			b = rand()%cot;
		}
		while(sanchoi[a][b]!=' ');
		sanchoi[a][b] = '$';
	}

	//di chuyen than ran
	for ( i = 1; i < dodai; i++){
		if (ran[0].x == ran[i].x && ran[0].y == ran[i].y){ 
			ketthuc = true;
			break;
		}
		else{
			ran[i].ox = ran[i].x; 
			ran[i].oy = ran[i].y;
			ran[i].x = ran[i - 1].ox; 
			ran[i].y = ran[i - 1].oy;
		}
	}
}

bool ngoaicanh(){
	return rand()%100 < xacsuat;
}

//thay doi huong di chuyen yeu thich cua ran 
void chinhhuongdi(){
	int a;

	//neu gap chuong ngai vat hoac yeu to ngoai canh thi thay doi huong di
	if( sanchoi[ran[0].x+huongdi.y][ran[0].y+huongdi.x]=='#' || ngoaicanh()){
		//neu trai la chuong ngai vat thi re phai
		if( sanchoi[ran[0].x-huongdi.x][ran[0].y+huongdi.y]=='#' ){
			a = huongdi.x;
			huongdi.x = -huongdi.y;
			huongdi.y = a;
		}
		else{
		//neu phai la chuong ngai vat thi re trai
			if( sanchoi[ran[0].x+huongdi.x][ran[0].y-huongdi.y]=='#'){
				a = huongdi.x;
				huongdi.x = huongdi.y;
				huongdi.y = -a;
			} 
			else{
			// neu trai va phai khong la chuong ngai vat thi re random
				if(rand()%2==0){
					//re phai
					a = huongdi.x;
					huongdi.x = -huongdi.y;
					huongdi.y = a;
				}
				else{
					//re trai					
					a = huongdi.x;
					huongdi.x = huongdi.y;
					huongdi.y = -a;
				}
			}		
		}
	}
}

//ham suu tam : dung de dat mau cho chu
void SetColor(WORD color)
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;
	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

//in ra mang sanchoi
void in(){
	int i,j;
	
	//kiem tra co dam vao chuong ngai vat hay khong val uu phan than ran vao mang
	if(sanchoi[ran[0].x][ran[0].y]=='#') 
		ketthuc = true;
	else{
		for (i = 0; i < dodai; i++) 
			sanchoi[ran[i].x][ran[i].y] = '*';
		sanchoi[ran[dodai-1].ox][ran[dodai-1].oy] = ' ';
	}
	
	//in ra sanchoi va diem 
	for ( i = 0; i < hang; i++) {
		for ( j = 0; j < cot; j++) {
			gotoxy(j,i);
			if(sanchoi[i][j] == '*')
				SetColor(9);
			if(sanchoi[i][j] == '$')
				SetColor(14);
			cout<<sanchoi[i][j];
			SetColor(2);
		}
	}
	cout<<endl<<"Score : "<<dodai-1<<endl;
}

//ham suu tam : dung de an con tro
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = {1, CursorVisibility};
	SetConsoleCursorInfo(handle, &cursor);
}

int main(){
	srand(time(NULL));
	SetColor(2);
	ShowCur(false);

	int n;
	cout<<"Choose level < 1..5 > : "<<endl;
	cin>>n;

	while(true){
		khoitao(n);
		system("cls");
		while(!ketthuc){
			Sleep(tocdo);
			chinhhuongdi();
			dichuyen(huongdi);
			in();
		}
		in();
		if(dodai==diemyeucau+1)
			cout<<"You win."<<endl;
		else{
			cout<<"You lose."<<endl;
			break;
		}
		Sleep(2000);
		if(n<5){
			n++;
			system("cls");
			cout<<"Loading level "<<n<<" ..."<<endl;
			Sleep(2000);
		}
		else break;
	}

	system("pause");
	return 0;
}
