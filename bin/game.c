#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// [!] comment out one of these depending on your OS
#include "key_linux/key.h"
//#include "key_windows/key.h"

#include "matriks/matriks.h"
#include "point/point.h"
#include "jam/jam.h"
#include "queue/queue.h"
#include "graph/graph.h"
#include "stack/stack.h"
#include "tree/tree.h"

#define STATE_MENU 0
#define STATE_NAME 1
#define STATE_LOAD 2
#define STATE_GAME 3
#define STATE_RESEP 4

#define OPTION_NEW 0
#define OPTION_START 1
#define OPTION_LOAD 2
#define OPTION_EXIT 3

const char *not_found = "Player name not found, press any key to go back";
const char *opt[] = {" New Game"," Start   "," Load    "," Exit    "};

int state, option;
char name[100], name_load[100];

Infotype arrmenu[8] = {"Banana Split", "Sundae", "Nasi telur dadar", "Nasi Ayam Goreng", "Burger", "Hot Dog", 
                            "Spaghetti Carbonara", "Spaghetti Bolognese"};

infotypeQue tunggumenu[12];
int meja[15];
int current_room;
MATRIKS room[5], gameRoom;
POINT p_pos;
Waktu Jam;
Queue Customer;
Queue Duduk;
Graph g;
int life = 30;
int money = 0;
Stack Tangan, Tray;
boolean useTray = false;

struct savedata {
    infotypeQue tunggumenu[12];
	int meja[15];
	int current_room;
	MATRIKS room[5], gameRoom;
	POINT p_pos;
	Waktu Jam;
	Queue Customer;
	Queue Duduk;
	int life;
	int money;
	Stack Tangan, Tray;
	boolean useTray;
};

void saveGame(){
	
	char *path = "./savedata/";
    char fname[105];
    strcpy(fname, path);
    strcat(fname, name);
    strcat(fname, ".engi");
	
	FILE *outfile;
	outfile = fopen(fname, "w");
	if (outfile == NULL){
		fprintf(stderr, "\nError opening file\n");
		exit(1);
	}
	
	struct savedata s;
	for (int i = 0; i < 12; i++) s.tunggumenu[i] = tunggumenu[i];
	for (int i = 0; i < 15; i++) s.meja[i] = meja[i];
	s.current_room = current_room;
	for (int i = 0; i < 5; i++) s.room[i] = room[i];
	s.gameRoom = gameRoom;
	s.p_pos = p_pos;
	s.Jam = Jam;
	s.Customer = Customer;
	s.Duduk = Duduk;
	s.life = life;
	s.money = money;
	s.Tangan = Tangan;
	s.Tray = Tray;
	s.useTray = useTray;
	
	fwrite(&s, sizeof(struct savedata), 1, outfile);
	if(fwrite != 0)  
		printf("Contents to file written successfully!\n"); 
	else 
		printf("Error writing file!\n"); 
	fclose(outfile);
	printf("Game saved to: %s\n", fname);
	printf("Load the game by inputting the player name as %s\n", name);
	printf("Press any key to continue the game..."); getKey();
	
}

void emptyString(char* s){
    for (int i = 0; i < 100; i++)
        s[i] = 0;
}

void printLine(){
    printf("--------------------------------------------------------------------------------------------------------------------\n");
}

void printEmpty(){
    printf("---                                                                                                              ---\n");
}

void printCenter(char *s) {
    printf("---%*s%*s---\n",(int)(55+strlen(s)/2),s,(int)(55-strlen(s)/2),"");
}

void displayCustomer(){
    printf("Waiting Customers:\n");
    printLine();    
    printf("Customers queuing for table: %d\n", NBElmtQue(Customer));
    PrintQue(Customer);
}

void cetaktunggumenu(){
    printLine();
    printf("Customers waiting for the order: \n");
    for(int i = 1;i<=10;i++){
        if(tunggumenu[i].id != -1){
			int len = 22 - strlen(arrmenu[tunggumenu[i].id]);
			if (i > 9) len--;
            printf("Table #%d   |   %s",i,arrmenu[tunggumenu[i].id]);
			for (int j = 0; j < len; j++) printf(" ");
			printf("|   Deadline: ");
            TulisWaktuKesabaran(tunggumenu[i].Jam);
        }
    }
}
void updateCustomer(int number)
{
	UpDate(&Jam,1);
    infotypeQue orang;
    int jumlah;
    Waktu WaktuKesabaran;
    CreateStart(&WaktuKesabaran);
    if (Sec(Jam) % 37 == 0 || (Hour(Jam) == 0 && Min(Jam) == 0 && Sec(Jam) == 1) || Sec(Jam) % 23 == 0)
    {
        orang.id = (rand()%(number+1) +1)*2;
        printf("%d\n", orang.id);
        WaktuKesabaran = Jam;
        int temp = (rand()%(1) +1);
        if (temp == 1)
        {
            UpDate(&WaktuKesabaran, (rand()%20)+30);
        }
        else
        {
            UpDate(&WaktuKesabaran, (rand()%15)+30);
        }
        orang.Jam = CreateWaktu(Hour(WaktuKesabaran), Min(WaktuKesabaran), Sec(WaktuKesabaran), Day(WaktuKesabaran));
        AddQue(&Customer, orang);
    }
}

void newSave(){
	
    CreateStart(&Jam);
    CreateEmptyQue(&Customer, 20);
    p_pos = MakePOINT(7, 7);
    current_room = 1;
    for (char c = '1'; c <= '4'; c++){
        char src[] = "./src/1.txt";
        src[6] = c;
        BacaMATRIKS(&room[c - '0'], src);
    }
    CreateEmpty(&Tangan);
    for (int i = 0; i < 15; ++i) meja[i] = 0;
    for (int i = 0; i <= 10; ++i)
    {
        tunggumenu[i].id = -1;
        CreateStart(&tunggumenu[i].Jam);
    }
	
}

void firstSetup(){
	
    InitGraph(&g, "./src/g.txt");
    BuildTree(&treemakanan);
    emptyString(name);
    state = STATE_MENU;
    option = OPTION_NEW;

}

void printGameOver(){
  printf("\t\t\t  -------                                  --------\n");
  printf("\t\t\t |       |                                |        |\n");
  printf("\t\t\t|         |                              |          |\n" );
  printf("\t\t\t|                                        |          |\n");  
  printf("\t\t\t|     ------ ----- ---- ----  ----|      |          ||     ||----||----\n");
  printf("\t\t\t|          |----  |    |    |  ___.      |          | |   | |____.|    |\n");
  printf("\t\t\t |        |       |    |    |     `       |        |   | |  |    `|---|\n");
  printf("\t\t\t  -------- -------|    |    |-----|        --------     |   |----||    -|\n\n\n");
}

void printThank(){
    printf("\n\t\t\t\t----- .    .  ----  |   | |  |     |   |  ----  |   |\n");
    printf("\t\t\t\t  |   |----| |----| | | | |||        |   |    | |   |\n");
    printf("\t\t\t\t  |   |    | |    | |  || |  |       |    ----   ---\n");
}

void printCreditCreator(int money){
    printf("\t\t\t\t\t\tThanks for playing with us!\n");
    printf("\t\t\t\t\t\tYour Score is %d. Not bad!\n",money);
    printf("\t\t\t\t\t\tHope you enjoy the game!\n\n");
    printThank();
    printf("\n\t\t\t\t\t\tThis game is presented by:\n");
    printf("\t\t\t\t\t\t     Asif Hummam Rais\n");
    printf("\t\t\t\t\t\t  Faiz Muhammad Muflich\n");
    printf("\t\t\t\t\t\t   Irfan Sofyana Putra\n");
    printf("\t\t\t\t\t\t     Jofiandy Leonata\n");
    printf("\t\t\t\t\t\t  Muhammad Hanif Adzkiya\n");
    printf("\t\t\t\t\t\t      Winston Wijaya\n\n");
}


void printCredit(int money){
    clrscr();
    printGameOver();   
    printCreditCreator(money);
	printf("\n");
	printf("\n");
	printf("\n");
	printf("Press any key to go to main menu...");
	getKey();
}

boolean isMeja() {
    if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)+1) == 'M'
        || Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)-1) == 'M'
        || Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)-1) == 'M'
        || Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)+1) == 'M'
        || Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)) == 'M'
        || Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) == 'M') 
        return true;
    else return false;
}

int kursiKosong() {
    if ((Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)) == 'o' && Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)+1) == 'o')
        || (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)) == 'o' && Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)-1) == 'o')
        || (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) == 'o' && Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)-1) == 'o')
        || (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) == 'o' && Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)+1) == 'o')) 
        return 4;
    else if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)) == 'M' && Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)-1) == 'o'
             || Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) == 'M' && Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)-1) == 'o')
        return 2;
    else return 0;
}

int noMeja (int X, int Y) {
    if (X==3 && Y==4 && current_room==2) return 1;
    else if (X==6 && Y==4 && current_room==2) return 2;
    else if (X==9 && Y==4 && current_room==2) return 3;
    else if (X==4 && Y==4 && current_room==1) return 4;
    else if (X==4 && Y==10 && current_room==1) return 5;
    else if (X==9 && Y==4 && current_room==1) return 6;
    else if (X==9 && Y==10 && current_room==1) return 7;
    else if (X==3 && Y==4 && current_room==3) return 8;
    else if (X==6 && Y==4 && current_room==3) return 9;
    else if (X==9 && Y==4 && current_room==3) return 10;
}

void caricustmarah(){
    for(int i = 1;i<=10;i++){
        if (tunggumenu[i].id != -1 && tunggumenu[i].Jam.H == Jam.H && tunggumenu[i].Jam.M == Jam.M 
            && tunggumenu[i].Jam.S == Jam.S)
            {
                tunggumenu[i].id = -1;
                meja[i] = 0;
                life--;
            }

        /* Life Habis */
		if (life <= 0)
		{
			printCredit(money);
			state = STATE_MENU;
        }
    }
}

void taruhCustomer(int n) {
    // n jumlah kursi di meja
    infotypeQue buang;
    int nomermeja;
    Waktu jamtmp;
    jamtmp = Jam;
    UpDate(&jamtmp,50); 
    if (InfoHeadQue(Customer).id <= n && n==2) {
        if (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) == 'M') {
            nomermeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos));
            meja[nomermeja] = 2;
        } else {
            nomermeja = noMeja(Absis(p_pos)+1, Ordinat(p_pos));
            meja[nomermeja] = 2;
        }
        DelQue(&Customer, &buang);
        tunggumenu[nomermeja].id = rand()%8;
        tunggumenu[nomermeja].Jam = jamtmp;
    } else if (InfoHeadQue(Customer).id <= n) {
        if (InfoHeadQue(Customer).id == 2) {
            if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)+1) == 'M') {
                nomermeja = noMeja(Absis(p_pos)+1,Ordinat(p_pos)+1);
                meja[nomermeja] = 2;
            } else if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)-1) == 'M') {
                nomermeja = noMeja(Absis(p_pos)+1, Ordinat(p_pos)-1);
                meja[nomermeja] = 2;
            } else if (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)-1) == 'M') {
                nomermeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos)-1);
                meja[nomermeja] = 2;
            } else {
                nomermeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos)+1);
                meja[nomermeja] = 2;
            }

        } else {
            if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)+1) == 'M') {
                nomermeja = noMeja(Absis(p_pos)+1,Ordinat(p_pos)+1);
                meja[nomermeja] = 4;
            } else if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)-1) == 'M') {
                nomermeja = noMeja(Absis(p_pos)+1, Ordinat(p_pos)-1);
                meja[nomermeja] = 4;
            } else if (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)-1) == 'M') {
                nomermeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos)-1);
                meja[nomermeja] = 4;
            } else {
                nomermeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos)+1);
                meja[nomermeja] = 4;
            }
        }
        tunggumenu[nomermeja].id = rand()%8;
        tunggumenu[nomermeja].Jam = jamtmp;
        DelQue(&Customer, &buang);
    } else {
        // untuk yang antrian terdepan tidak cukup tapi di belakang ada yang cukup
        // infohead == 4 kursi 2
        if (IsAdaDuaQue(Customer)) {
            if (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) == 'M') {
                nomermeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos));
                meja[nomermeja] = 2;
            } else {
                nomermeja = noMeja(Absis(p_pos)+1, Ordinat(p_pos));
                meja[nomermeja] = 2;
            }
            tunggumenu[nomermeja].id = rand()%8;
            tunggumenu[nomermeja].Jam = jamtmp;
            Del2Que(&Customer, &buang);
        }
    }
}

void taruhMakan() {
    int tempMeja;
    if (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) == 'M') tempMeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos));
    else if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)) == 'M') tempMeja = noMeja(Absis(p_pos)+1, Ordinat(p_pos));
    else if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)+1) == 'M') tempMeja = noMeja(Absis(p_pos)+1, Ordinat(p_pos)+1);
    else if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)-1) == 'M') tempMeja = noMeja(Absis(p_pos)+1, Ordinat(p_pos)-1);
    else if (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)-1) == 'M') tempMeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos)-1);
    else if (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)+1) == 'M') tempMeja = noMeja(Absis(p_pos)-1, Ordinat(p_pos)+1);

    Infotype tempMakanan;
    if (!IsEmptySta(Tray)){
        strcpy(tempMakanan, InfoTop(Tray));
        if (strcmp(tempMakanan, arrmenu[tunggumenu[tempMeja].id])==0 && useTray) {
            //ilangin
            Infotype X;
            Pop(&Tray, &X);
            tunggumenu[tempMeja].id = -1;
            meja[tempMeja] = 0;
            money += keuntungan(treemakanan, tempMakanan);
        }
    }
}

void klasifikasibahan(char c, Infotype res){
    if (c == 'T') strcpy(res, "Tray");
    else if (c == 'L') strcpy(res, "Telur");
    else if (c == 'S') strcpy(res, "Sendok");
    else if (c == 'P') strcpy(res, "Piring");
    else if (c == 'G') strcpy(res, "Garpu");
    else if (c == 'Y') strcpy(res, "Patty");
    else if (c == 'N') strcpy(res, "Nasi");
    else if (c == 'R') strcpy(res, "Roti");
    else if (c == 'A') strcpy(res, "Ayam Goreng");
    else if (c == 'O') strcpy(res, "Sosis");
    else if (c == 'E') strcpy(res, "Stroberi");
    else if (c == 'K') strcpy(res, "Es Krim");
    else if (c == 'I') strcpy(res, "Pisang");
    else if (c == 'C' ) strcpy(res, "Carbonara");
    else if (c == 'H') strcpy(res, "Spaghetti");
    else if (c == 'B') strcpy(res, "Bolognese");
    else if (c == 'J') strcpy(res, "Keju");
}

int lastbahan(Infotype bahan){
    int ibahan = cariIndeks(bahan);
    addrNode nbahan = AlokNode(ibahan);
    int res = -1;
    for (int i = 0; i < 8 && res == -1 ; i++){
        Infotype tmp;
        strcpy(tmp, arrmenu[i]);
        int itmp = cariIndeks(tmp);
        addrNode ntmp = AlokNode(itmp);
        if (isparent(treemakanan, nbahan, ntmp)){
            res =  i;
        }
    }
    return res;
}

char ambilbahanmakanan(){
    //atas
    char ret = '-';
    if (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) != ' ' && 
        Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) != '#')
            ret = Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos));
    //kanan
    else if (Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)+1) != ' ' && 
        Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)+1) != '#')
            ret = Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)+1);
    //bawah
    else if (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)) != ' ' && 
        Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)) != '#')
            ret = Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos));
    //kiri
    else if (Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)-1) != ' ' && 
        Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)-1) != '#')
            ret = Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)-1);

    return ret;
}

void keyGame(char key){
    
	if (key == 's'){
		saveGame("asd.dat");
	}
	
    if (key == KEY_UP && (Elmt(gameRoom, Absis(p_pos)-1, Ordinat(p_pos)) == ' ')){  
        updateCustomer(1);
        Absis(p_pos)--;
    } else if (key == KEY_DOWN && (Elmt(gameRoom, Absis(p_pos)+1, Ordinat(p_pos)) == ' ')){
        updateCustomer(1);
        Absis(p_pos)++;
    } else if (key == KEY_RIGHT && (Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)+1) == ' ')){
        updateCustomer(1);
        Ordinat(p_pos)++;
    } else if (key == KEY_LEFT && (Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)-1) == ' ')){
        updateCustomer(1);
        Ordinat(p_pos)--;
    } 
    else if (key == 'b' || key == 'B') {
		updateCustomer(1);
        while (!IsEmptySta(Tangan)){
            Infotype X;
            Pop(&Tangan, &X);
        }
    }
    else if(key == 't' || key == 'T'){
        if(useTray){
			updateCustomer(1);
            while(!IsEmptySta(Tray)){
                Infotype food_buang;
                Pop(&Tray,&food_buang);
            }
        }
    } else if (key == KEY_SPACE) {
        // meja
		updateCustomer(1);
        if (isMeja()) {
            if (kursiKosong()) {
                // taruh customer nb: meja pasti kosong.
                int n = kursiKosong();
                if (!IsEmptyQue(Customer)) {
                    taruhCustomer(n);
                }
            } else {
                // taruh makanan
                taruhMakan();
            }
        } 
        // tidak ada meja, berarti lagi di dapur mau ambil bahan makanan / menaruh makanan di tray
        else {
            char bahan = ambilbahanmakanan();
            if (bahan != '-'){
                //kasih ke tray, akan ada dua opsi:
                //1. simpen makanan jadinya di tray
                //2. ambil tray nya langsung anterin
                if (bahan == 'T') {
                    if (useTray) useTray = false;
                    else {
                        if (IsEmptySta(Tangan)) useTray = true;
                        if (!IsEmptySta(Tangan)) {
                            if (NbElmtSt(Tray) < 5) {
                                Infotype food_jadi;
                                Pop(&Tangan, &food_jadi);
                                Push(&Tray, food_jadi);
                            }
                        }
                    }
                }
                else {
                    Infotype bahanmakanan;
                    klasifikasibahan(bahan, bahanmakanan);
                    if (!useTray){ 
                        if (IsEmptySta(Tangan) && bahan == 'P'){
                            Push(&Tangan, bahanmakanan);
                        }
                        else {
                            Infotype toptangan;
                            strcpy(toptangan, InfoTop(Tangan));
                            int itoptangan = cariIndeks(toptangan);
                            addrNode tmp1 = AlokNode(itoptangan);

                            int inow = cariIndeks(bahanmakanan);
                            addrNode tmp2 = AlokNode(inow);
                            
                            //path for finding the food
                            if (isparent(treemakanan, tmp1, tmp2)){
                                Push(&Tangan, bahanmakanan);
                                //food is ready, food is on the chef hand's
                                if (lastbahan(bahanmakanan) != -1) {
                                    Infotype thefood;
                                    strcpy(thefood,arrmenu[lastbahan(bahanmakanan)]);
                                    while (!IsEmptySta(Tangan)){
                                        Infotype toptmp;
                                        Pop(&Tangan, &toptmp);
                                    }
                                    Push(&Tangan, thefood);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (key == 'r'){
        state = STATE_RESEP;
    } else if (key == 'q'){
        printCredit(money);
		state = STATE_MENU;
    }
    
}

void printHand(Stack S){
    printLine();
    printf("Hand:\n");
    PrintStack(S);
    if (useTray) printf("[Tray]\n");
}

void printTray(Stack S){
    printLine();
    printf("Tray:\n");
    PrintStack(S);
}

void printGame(){
    
    infotypeQue buang;
	
	infotypeGraph tmpg;
	tmpg.room = current_room;
	tmpg.p = p_pos;
	tmpg = GetFirstSuccInfo(g, tmpg);
    if (tmpg.room != -1){
        current_room = tmpg.room;
        p_pos = tmpg.p;
    }
    printf("                                                Life: %d | Money: %d",life,money);
    printf("\n                                            "); TulisWaktu(Jam);

    CopyMATRIKS(room[current_room], &gameRoom);
    Elmt(gameRoom, Absis(p_pos), Ordinat(p_pos)) = 'P';
    if (current_room == 2 ) {
       if (meja[1]) {
        Elmt(gameRoom,3,3) = 'C';
        Elmt(gameRoom,3,5) = 'C';
       }
       if (meja[2]) {
        Elmt(gameRoom,6,3) = 'C';
        Elmt(gameRoom,6,5) = 'C';
       }
       if (meja[3]) {
        Elmt(gameRoom,9,3) = 'C';
        Elmt(gameRoom,9,5) = 'C';
       }
    } else if (current_room == 1) {
        if (meja[4]) {
            Elmt(gameRoom,4,3) = 'C';
            Elmt(gameRoom,4,5) = 'C';
            if (meja[4] == 4) {
                Elmt(gameRoom,3,4) = 'C';
                Elmt(gameRoom,5,4) = 'C';
            }
        }
        if (meja[5]) {
            Elmt(gameRoom,4,9) = 'C';
            Elmt(gameRoom,4,11) = 'C';
            if (meja[5] == 4) {
                Elmt(gameRoom,3,10) = 'C';
                Elmt(gameRoom,5,10) = 'C';
            }
        }
        if (meja[6]) {
            Elmt(gameRoom,9,3) = 'C';
            Elmt(gameRoom,9,5) = 'C';
            if (meja[6] == 4) {
                Elmt(gameRoom,8,4) = 'C';
                Elmt(gameRoom,10,4) = 'C';
            }
        }
        if (meja[7]) {
            Elmt(gameRoom,9,9) = 'C';
            Elmt(gameRoom,9,11) = 'C';
            if (meja[7] == 4) {
                Elmt(gameRoom,8,10) = 'C';
                Elmt(gameRoom,10,10) = 'C';
            }
        }
    } else if (current_room == 3) {
        if (meja[8]) {
         Elmt(gameRoom,3,3) = 'C';
         Elmt(gameRoom,3,5) = 'C';
        }
        if (meja[9]) {
         Elmt(gameRoom,6,3) = 'C';
         Elmt(gameRoom,6,5) = 'C';
        }
        if (meja[10]) {
         Elmt(gameRoom,9,3) = 'C';
         Elmt(gameRoom,9,5) = 'C';
        }
    }
    
    TulisMATRIKS(gameRoom);
    printf("\n");
    displayCustomer();
    while (!IsEmptyQue(Customer) && InfoHeadQue(Customer).Jam.H == Jam.H && InfoHeadQue(Customer).Jam.M == Jam.M 
        && InfoHeadQue(Customer).Jam.S == Jam.S){
            DelQue(&Customer, &buang);
        life--;    
        /* Life Habis */
        if(life == 0)
        {
            printCredit(money);
			state = STATE_MENU;
        }
    }

    cetaktunggumenu();
    caricustmarah();
    printHand(Tangan);
    printTray(Tray);
}

void checkLoad(){
    FILE *infile;
    
    char *path = "./savedata/";
    char fname[105];
    strcpy(fname, path);
    strcat(fname, name_load);
    strcat(fname, ".engi");
    
    if (infile = fopen(fname, "r")){
		
		if (infile == NULL){
			fprintf(stderr, "\nError opening file\n");
			exit(1);
		}
		
		struct savedata s;
		fread(&s, sizeof(struct savedata), 1, infile);
		
		for (int i = 0; i < 12; i++) tunggumenu[i] = s.tunggumenu[i];
		for (int i = 0; i < 15; i++) meja[i] = s.meja[i];
		current_room = s.current_room;
		for (int i = 0; i < 5; i++) room[i] = s.room[i];
		gameRoom = s.gameRoom;
		p_pos = s.p_pos;
		Jam = s.Jam;
		Customer = s.Customer;
		Duduk = s.Duduk;
		life = s.life;
		money = s.money;
		Tangan = s.Tangan;
		Tray = s.Tray;
		useTray = s.useTray;
		
		fclose(infile);
        strcpy(name, name_load);
		
        state = STATE_MENU;
    } else 
        strcpy(name_load, not_found);
}

void readKey(){
    int key = getKey();
    int len;
    switch(state){
        case STATE_MENU:
            switch(key){
                case KEY_UP:
                    option = (option + 4 - 1) % 4;
                    break;
                case KEY_DOWN:
                    option = (option + 4 + 1) % 4;
                    break;
                case KEY_ENTER:
                    switch(option){
                        
                        case OPTION_NEW:
                            newSave();
                            emptyString(name);
                            state = STATE_NAME;
                            break;
                        case OPTION_START:
                            if (name[0] == 0){
                                newSave();
                                state = STATE_NAME;
                            }
                            else
                                state = STATE_GAME;
                            break;
                        case OPTION_LOAD:
                            emptyString(name_load);
                            state = STATE_LOAD;
                            break;
                        case OPTION_EXIT:
							exit(0);
                    }
            }
            break;
        case STATE_NAME:
            ; len = strlen(name);
            if ((key == KEY_BACKSPACE) && (len > 0))
                name[len-1] = 0;
            else if ((len < 100) && (((key >= 'a') && (key <= 'z')) || ((key >= 'A') && (key <= 'Z')) || (key == KEY_SPACE)))
                name[len] = (char) key;
            else if (key == KEY_ENTER)
                state = STATE_MENU;
            break;
        case STATE_LOAD:
            ; len = strlen(name_load);
            if (strcmp(name_load, not_found) == 0){
                state = STATE_MENU;
                emptyString(name_load);
            } else {
                if ((key == KEY_BACKSPACE) && (len > 0))
                    name_load[len-1] = 0;
                else if ((len < 100) && (((key >= 'a') && (key <= 'z')) || ((key >= 'A') && (key <= 'Z')) || (key == KEY_SPACE)))
                    name_load[len] = (char) key;
                else if (key == KEY_ENTER)
                    checkLoad();
            }
            break;
        case STATE_GAME:
            keyGame(key);
            break;
        case STATE_RESEP:
            if(key == KEY_SPACE){
                state = STATE_GAME;
            }
    }
}

void printResep(){
	printCenter("Here are the recipes:");
	printf("\n");
    PrintTree(treemakanan,1);
	printf("\n");
    printCenter("Press SPACE to go back to the game");
}

void printOption(){
    char input[100];
    switch(state){
        case STATE_MENU:
            for (int i = OPTION_NEW; i <= OPTION_EXIT; i++){
                emptyString(input);
                if (option == i)
                    strcat(input, ">");
                strcat(input, opt[i]);
                printCenter(input);
            }
            break;
        case STATE_NAME:
            printCenter("Tell me your name:");
            strcpy(input, name);
            strcat(input, "_");
            printCenter(input);
            break;
        case STATE_LOAD:
            printCenter("Input player name to load:");
            strcpy(input, name_load);
            strcat(input, "_");
            printCenter(input);
            break;
        case STATE_GAME:
            printf("\n");
            printGame();
            printf("\n");
            break;
        case STATE_RESEP:
            printEmpty();
            printResep();
            printEmpty();
            break;
    }
}

void showUI() {
    clrscr();
    printLine();
    printLine();
    printCenter(" _____            _ _       _   ___ _       _                ");
    printCenter("|  ___|          (_| )     | | / (_) |     | |               ");
    printCenter("| |__ _ __   __ _ _|/ ___  | |/ / _| |_ ___| |__   ___ _ __  ");
    printCenter("|  __| '_ \\ / _` | | / __| |    \\| | __/ __| '_ \\ / _ \\ '_ \\ ");
    printCenter("| |__| | | | (_| | | \\__ \\ | |\\  \\ | || (__| | | |  __/ | | |");
    printCenter("\\____/_| |_|\\__, |_| |___/ \\_| \\_/_|\\__\\___|_| |_|\\___|_| |_|");
    printCenter("             __/ |                                           ");
    printCenter("            |___/          Extended version!                 ");
    printLine();
    printLine();
    printEmpty();
    printEmpty();
    char greet[100] = "Hello, ";
    if (name[0] == 0)
        strcat(greet, "stranger");
    else
        strcat(greet, name);
    strcat(greet, "!");
    printCenter(greet);
    printOption();
    printEmpty();
    printEmpty();
    if (state != STATE_GAME)
		printCenter("Arrow keys to navigate, enter to select option");
	else {
		printCenter("Arrow keys: move | Q: quit to main menu | R: show recipe");
		printCenter("B: throw food on hand | T: throw food on tray");
		printCenter("Spacebar: put customer/serve food/take item");
	}
    printEmpty();
    printLine();
    printLine();
    readKey();
    showUI();
}

int main() {
    firstSetup();
    showUI();
    return 0;
}

