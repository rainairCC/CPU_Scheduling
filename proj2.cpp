#include <iostream>
#include <stdio.h>   // cout, endl
#include <string.h>  // strcmp, strcpy
#include <fstream>   // open, is_open
#include <stdlib.h>
#include <string.h>

using namespace std;

typedef struct Id
{       // 存定義過的ID
    int id;
    int cpuBurst;
    int arrivalTime;
    int priority;
    int waitingTime;      // 進入時間 - arrivalTime
    int turnaroundTime;   // cpuBurst + waitingTime
    int used;             // 確認是否用過CPU
    int uused;
} ID;

string   fileName ;
ID gProblem[100] ;              // 總長是aP-2 因為包含題目 // 做事的陣列
ID gProblemTemp[100];           //存放正確題目的陣列

int gTest;                      // method
int gTime;                      // time slice
char gReg[100][100];
int aP = 0;                     // 文件總行數(有內容才算)
int gClock = 0;                 // 目前時間
string gMap;                    // 第1~5題的圖存放處
string gMapAnswer[100];         // 第6題的 所有排程 圖存放處
string gWaitingAnswer[100];     // 第6題的 所有排程 Waiting Time
string gTurnaroundAnswer[100];  // 第6題的 所有排程 Turnaround Time

bool readProblem();             // 讀檔
void token();                   // 切每段資料放入struct
void arrivalTimeBubbleSort();   // arrivalTimeq排序
void idBubbleSort();            // id排序
void FCFG();
void RR();
void PSJF();
void NSJF();
void PP();
void writeAnswer();             // 寫檔

int main()
{           // 每計算是重新開始時記得g的參數要初始化!!!!!!
    if( readProblem() == true )
    {
        token();
        arrivalTimeBubbleSort();

        int a = 0;
        while( a <= aP-2 )     // 要有放正確的題目的位置
        {
            gProblemTemp[a] = gProblem[a];
            a = a + 1;
        }

        if( gTest == 1 )
            FCFG();
        if( gTest == 2 )
            RR();
        if( gTest == 3 )
            PSJF();
        if( gTest == 4 )
            NSJF();
        if( gTest == 5 )
            PP();

        if( gTest == 6 )
        {
            int u = 1;

            while( gTest > u )
            {
                if( u == 1 )
                    FCFG();
                if( u == 2 )
                    RR();
                if( u == 3 )
                    PSJF();
                if( u == 4 )
                    NSJF();
                if( u == 5 )
                    PP();

                idBubbleSort();

                a = 0;

                string lala ;    /// 註解掉的話　輸出會是亂碼喔！！！！！！！！！！！！！！！！
                char reg[2];

                while( a <= aP-2 && u < 6 )     // 正確的題目回歸
                {
                    sprintf( reg, "%i", gProblem[a].waitingTime );
                    gWaitingAnswer[a] = gWaitingAnswer[a] + "        " + reg;

                    sprintf( reg, "%i", gProblem[a].turnaroundTime );
                    gTurnaroundAnswer[a] = gTurnaroundAnswer[a] + "        " + reg;

                    if( u < 5 )
                        gProblem[a] = gProblemTemp[a];

                    a = a + 1;
                }

                gMapAnswer[u] = gMap;
                gMap = "";
                gClock = 0;
                u = u + 1 ;
            }
        }
        idBubbleSort();
        writeAnswer();
    }
}

void PP()
{
    int i = 0;
    char reg[2];

    while( i <= aP-2 )
    {
        if( gClock < gProblem[i].arrivalTime )
        {
            gMap = gMap + "-";
            gClock = gClock + 1;
        }

        int a = i+1;
        ID tt = gProblem[i];
        ID rr;

        while( gProblem[a].arrivalTime == tt.arrivalTime )
        {
            if( tt.priority > gProblem[a].priority )
            {
                rr = gProblem[a];
                gProblem[a] = tt;
                tt = rr;
            }
            a = a + 1;
        }
        gProblem[i] =tt;

        int c = i+1;
        ID regg = gProblem[i];
        gProblem[c-1] = regg;
        gProblem[i].waitingTime = gProblem[i].waitingTime + gClock - gProblem[i].arrivalTime;    // 等待時間 為 進入時間 - arrivalTime
        gClock = gClock + 1;
        gProblem[i].cpuBurst = gProblem[i].cpuBurst - 1;
        gProblem[i].arrivalTime = gClock;
        gProblem[i].turnaroundTime = gProblem[i].turnaroundTime + 1;
        gProblem[i].used = 1;
        gProblem[i].uused = 1;
        int y = i+1;
        while( y <= aP-2 )
        {
            gProblem[y].uused = 0;
            y = y + 1;
        }

        int j = 0;

        if( gProblem[i].id >= 10 )
            reg[0] = gProblem[i].id - 10 + 'A';
        else
            sprintf( reg, "%i", gProblem[i].id );

        gMap = gMap + reg;
        j = j + 1;


        if( gProblem[i].cpuBurst == 0 ){
            gProblem[i].turnaroundTime = gProblem[i].turnaroundTime + gProblem[i].waitingTime;    // cpuBurst + waitingTime
            i = i + 1;
        }

        int k = i+1;
        ID temp = gProblem[i];
        ID ttemp ;

        while( k <= aP-2 && gProblem[k].arrivalTime <= gClock  )
        {
            if( gProblem[k].priority < temp.priority )
            {
                ttemp = temp;
                temp = gProblem[k];
                gProblem[k] = ttemp;
                gProblem[k].uused = 0;
            }

            if( gProblem[k].priority == temp.priority && temp.uused == 0 )
            {
                while( k <= aP-2 && gProblem[k].used == 1 && gProblem[k].priority == temp.priority && gProblem[k].arrivalTime <= gClock  )
                {
                    k = k + 1;
                }

                if( k <= aP-2 && gProblem[k].used == 0 && gProblem[k].arrivalTime <= gClock && gProblem[k].priority == temp.priority  )
                {
                    ttemp = temp;
                    temp = gProblem[k];
                    gProblem[k] = ttemp;
                    //temp.uused = 1;
                    gProblem[k].uused = 0;
                }
                if( gProblem[k].used == 0 && gProblem[k].priority == temp.priority )
                {
                    while( k <= aP-2 && gProblem[k].arrivalTime < temp.arrivalTime && gProblem[k].arrivalTime <= gClock  )
                    {
                        ttemp = temp;
                        temp = gProblem[k];
                        gProblem[k] = ttemp;
                        k = k + 1;
                    }
                    k = k - 1;
                    if( gProblem[k].arrivalTime == temp.arrivalTime )
                    {
                        while( k <= aP-2 && gProblem[k].id < temp.id )
                        {
                            ttemp = temp;
                            temp = gProblem[k];
                            gProblem[k] = ttemp;
                            k = k + 1;
                        }
                    }
                }
            }
            k = k + 1;
        }
        k = k - 1;
        gProblem[i] = temp;
    }
}

void NSJF()
{
    int i = 0;
    char reg[2];

    while( i <= aP-2 )
    {
        if( gClock < gProblem[i].arrivalTime )
        {
            gMap = gMap + "-";
            gClock = gClock + 1;
        }

        int a = i+1;
        ID tt = gProblem[i];
        ID rr;

        while( gProblem[a].arrivalTime == tt.arrivalTime )
        {
            if( tt.cpuBurst > gProblem[a].cpuBurst )
            {
                rr = gProblem[a];
                gProblem[a] = tt;
                tt = rr;
            }
            a = a + 1;
        }
        gProblem[i] =tt;

        int c = i+1;
        ID regg = gProblem[i];
        gProblem[c-1] = regg;
        gProblem[i].waitingTime =  gClock - gProblem[i].arrivalTime;    // 等待時間 為 進入時間 - arrivalTime
        gClock = gClock + gProblem[i].cpuBurst;
        gProblem[i].arrivalTime = gClock;
        gProblem[i].turnaroundTime = gProblem[i].waitingTime + gProblem[i].cpuBurst;

        int j = 0;
        while( j < gProblem[i].cpuBurst )
        {
            if( gProblem[i].id >= 10 )
                reg[0] = gProblem[i].id - 10 + 'A';
            else
                sprintf( reg, "%i", gProblem[i].id );

            gMap = gMap + reg;
            j = j + 1;
        }

        i = i + 1;

        int k = i+1;
        ID temp = gProblem[i];
        ID ttemp ;

        while( k <= aP-2 && gProblem[k].arrivalTime <= gClock  )
        {
            if( gProblem[k].cpuBurst < temp.cpuBurst )
            {
                ttemp = temp;
                temp = gProblem[k];
                gProblem[k] = ttemp;
            }
            int ch = 0 ;

            if( gProblem[k].cpuBurst == temp.cpuBurst )
            {
                while( k <= aP-2 && gProblem[k].cpuBurst < temp.cpuBurst )
                {
                    k = k + 1;
                    ch = 1;
                }

                if( gProblem[k].cpuBurst == temp.cpuBurst )
                {
                    while( k <= aP-2 && gProblem[k].arrivalTime < temp.arrivalTime && gProblem[k].arrivalTime <= gClock  )
                    {
                        ttemp = temp;
                        temp = gProblem[k];
                        gProblem[k] = ttemp;
                        k = k + 1;
                    }

                    if( gProblem[k].arrivalTime == temp.arrivalTime )
                    {
                        while( k <= aP-2 && gProblem[k].id < temp.id )
                        {
                            ttemp = temp;
                            temp = gProblem[k];
                            gProblem[k] = ttemp;
                            k = k + 1;
                        }
                    }
                }
            }
            k = k + 1;
        }
        k = k - 1;
        gProblem[i] = temp;
    }
}

void PSJF()
{
    int i = 0;
    char reg[2];

    while( i <= aP-2 )
    {
        if( gClock < gProblem[i].arrivalTime )
        {
            gMap = gMap + "-";
            gClock = gClock + 1;
        }
        int a = i+1;
        ID tt = gProblem[i];
        ID rr;

        while( gProblem[a].arrivalTime == tt.arrivalTime )
        {
            if( tt.cpuBurst > gProblem[a].cpuBurst )
            {
                rr = gProblem[a];
                gProblem[a] = tt;
                tt = rr;
            }
            a = a + 1;
        }
        gProblem[i] =tt;

        int c = i+1;
        ID regg = gProblem[i];
        gProblem[c-1] = regg;
        gProblem[i].waitingTime = gProblem[i].waitingTime + gClock - gProblem[i].arrivalTime;    // 等待時間 為 進入時間 - arrivalTime
        gClock = gClock + 1;
        gProblem[i].cpuBurst = gProblem[i].cpuBurst - 1;
        gProblem[i].arrivalTime = gClock;
        gProblem[i].turnaroundTime = gProblem[i].turnaroundTime + 1;
        gProblem[i].used = 1;
        gProblem[i].uused = 1;
        int y = i+1;

        while( y <= aP-2 )
        {
            gProblem[y].uused = 0;
            y = y + 1;
        }

        int j = 0;

        if( gProblem[i].id >= 10 )
            reg[0] = gProblem[i].id - 10 + 'A';
        else
            sprintf( reg, "%i", gProblem[i].id );

        gMap = gMap + reg;
        j = j + 1;

        if( gProblem[i].cpuBurst == 0 ){
            gProblem[i].turnaroundTime = gProblem[i].turnaroundTime + gProblem[i].waitingTime;    // cpuBurst + waitingTime
            i = i + 1;
        }

        int k = i+1;
        ID temp = gProblem[i];
        ID ttemp ;

        while( k <= aP-2 && gProblem[k].arrivalTime <= gClock  )
        {
            if( gProblem[k].cpuBurst < temp.cpuBurst )
            {
                ttemp = temp;
                temp = gProblem[k];
                gProblem[k] = ttemp;
                gProblem[k].uused = 0;
            }

            if( gProblem[k].cpuBurst == temp.cpuBurst && temp.uused == 0 )
            {
                while( k <= aP-2 && gProblem[k].used == 1 && gProblem[k].cpuBurst == temp.cpuBurst )
                {
                    k = k + 1;
                }

                if( k <= aP-2 && gProblem[k].used == 0 && gProblem[k].cpuBurst == temp.cpuBurst )
                {
                    ttemp = temp;
                    temp = gProblem[k];
                    gProblem[k] = ttemp;
                    gProblem[k].uused = 0;
                }

                if( gProblem[k].used == 0 && gProblem[k].cpuBurst == temp.cpuBurst )
                {
                    while( k <= aP-2 && gProblem[k].arrivalTime < temp.arrivalTime && gProblem[k].arrivalTime <= gClock  )
                    {
                        ttemp = temp;
                        temp = gProblem[k];
                        gProblem[k] = ttemp;
                        k = k + 1;
                    }
                    k = k - 1;
                    if( gProblem[k].arrivalTime == temp.arrivalTime && gProblem[k].arrivalTime <= gClock )
                    {
                        while( k <= aP-2 && gProblem[k].id < temp.id )
                        {
                            ttemp = temp;
                            temp = gProblem[k];
                            gProblem[k] = ttemp;
                            k = k + 1;
                        }
                    }
                }
            }
            k = k + 1;
        }
        k = k - 1;
        gProblem[i] = temp;
    }
}

void RR()
{
    int i = 0;
    char reg[2];

    while( i <= aP-2 )
    {
        if( gClock < gProblem[i].arrivalTime )
        {
            gMap = gMap + "-";
            gClock = gClock + 1;
        }

        int Time = gTime;
        if( gProblem[i].cpuBurst < gTime )
            Time = gProblem[i].cpuBurst;

        gProblem[i].waitingTime = gProblem[i].waitingTime + gClock - gProblem[i].arrivalTime;    // 等待時間 為 進入時間 - arrivalTime
        gClock = gClock + Time;
        gProblem[i].cpuBurst = gProblem[i].cpuBurst - Time;
        gProblem[i].arrivalTime = gClock;
        gProblem[i].turnaroundTime = gProblem[i].turnaroundTime + Time;

        int j = 0;

        while( j < Time )
        {
            if( gProblem[i].id >= 10 )
                reg[0] = gProblem[i].id - 10 + 'A';
            else
                sprintf( reg, "%i", gProblem[i].id );

            gMap = gMap + reg;
            j = j + 1;
        }

        if( gProblem[i].cpuBurst == 0 ){
            gProblem[i].turnaroundTime = gProblem[i].turnaroundTime + gProblem[i].waitingTime;    // cpuBurst + waitingTime
            i = i + 1;
        }
        else
        {
            int k = i+1;
            ID temp = gProblem[i];

            while( k <= aP-2 && gProblem[k].arrivalTime <= gClock )
            {
                gProblem[k-1] = gProblem[k];
                k = k + 1;
            }

            if( k <= aP-2 && gProblem[k].arrivalTime == temp.arrivalTime )
            {
                while( k <= aP-2 && gProblem[k].id < temp.id )
                {
                    gProblem[k-1] = gProblem[k];
                    k = k + 1;
                }
            }
            gProblem[k-1] = temp;
        }
    }

}

void FCFG()
{
    int i = 0;
    char reg[2];

    while( i <= aP-2 )
    {
        if( gClock < gProblem[i].arrivalTime )
        {
            gMap = gMap + "-";
            gClock = gClock + 1;
        }
        gProblem[i].waitingTime = gClock - gProblem[i].arrivalTime;    // 等待時間 為 進入時間 - arrivalTime
        gProblem[i].turnaroundTime = gProblem[i].cpuBurst + gProblem[i].waitingTime;    // cpuBurst + waitingTime
        gClock = gClock + gProblem[i].cpuBurst;

        int j = 0;

        while( j < gProblem[i].cpuBurst )
        {
            if( gProblem[i].id >= 10 )
                reg[0] = gProblem[i].id - 10 + 'A';
            else
                sprintf( reg, "%i", gProblem[i].id );

            gMap = gMap + reg;
            j = j + 1;
        }
        i = i + 1;
    }
}

void idBubbleSort()
{
    int i = 0, j = 0;
    ID temp;

    for( i = 0; i <= aP-2 ; i++)
    {
        for( j = i; j <= aP-2; j++) {
            if( gProblem[j].id < gProblem[i].id ) {
                temp = gProblem[j];
                gProblem[j] = gProblem[i];
                gProblem[i] = temp;
            }
        }
    }
}

void arrivalTimeBubbleSort()
{
    int i = 0, j = 0;
    ID temp;

    for( i = 0; i <= aP-2 ; i++)
    {
        for( j = i; j <= aP-2; j++) {
            if( gProblem[j].arrivalTime < gProblem[i].arrivalTime ) {
                temp = gProblem[j];
                gProblem[j] = gProblem[i];
                gProblem[i] = temp;
            }
            else if( gProblem[j].arrivalTime == gProblem[i].arrivalTime && gProblem[j].id < gProblem[i].id )
            {
                temp = gProblem[j];
                gProblem[j] = gProblem[i];
                gProblem[i] = temp;
            }
        }
    }
}

void token()
{
    int i = 2;
    int j = 1;
    string reg;
    int check = 0;
    gTest = gReg[0][0] - '0';

    while( gReg[0][j] == ' ' || gReg[0][j] == '\t')
    {
        j = j + 1;
    }

    reg = gReg[0][j];

    if( gReg[0][j+1] != ' ' && gReg[0][j+1] != '\n' && gReg[0][j+1] != '\t' )
    {
        reg = reg + gReg[0][j+1];
        gTime = atoi(reg.c_str());
    }
    else
        gTime = gReg[0][j] - '0';

    j = 0;
    while( i <= aP )
    {
        reg = "";

        while( gReg[i][j] != '\n' )
        {
           if( gReg[i][j] != ' ' && gReg[i][j] != '\t' )
           {
                reg = reg + gReg[i][j];
                if( check == 0 )
                    check = 1;
                j = j + 1;
           }
           else
           {
                if( check == 1 )
                    gProblem[i-2].id = atoi( reg.c_str() );
                if( check == 2 )
                    gProblem[i-2].cpuBurst = atoi( reg.c_str() );
                if( check == 3 )
                    gProblem[i-2].arrivalTime = atoi( reg.c_str() );
                if( check == 4 )
                    gProblem[i-2].priority = atoi( reg.c_str() );

                while( gReg[i][j] == ' ' || gReg[i][j] == '\t'  )
                {
                    j = j + 1;
                }
                reg = "";
                check = check + 1;
           }
        }
        j = 0;
        check = 0;
        gProblem[i-2].used = 0;
        gProblem[i-2].uused = 0;
        i = i + 1;
    }
}

bool readProblem()
{                         // definition: read records from a file
    fstream     inFile;                                         // input file handle
    char        rBuf[255];
    int i = 0 ;

    cout << "請輸入檔名：" ;
    cin >> fileName ;                                           // input file name is fixed
    fileName = fileName + ".txt";

    inFile.open(fileName.c_str(), fstream::in);                 // open file to read

    if (!inFile.is_open())
    {                                  // unable to open file
        cout << endl << "### " << fileName << " does not exist! ###" << endl;
        return false;
    } // if
    else
        while (inFile.getline(rBuf, 255, '\n'))
        {                // get each input record line by line
            strcpy( gReg[i], ( rBuf ) );
            i = i + 1;
        }

    inFile.close();                                             // close file

    if (i == 0)
    {
        cout << endl << "### Get nothing from the file " << fileName << " ! ### " << endl;
        return false;
    } // if
    else
        aP = i - 1;

    return true;
}   // end readProblem

void writeAnswer()
{
    int i = 0;
    fstream outFile;
    string fname = fileName + "-output.txt";
    outFile.open( fname.c_str(),fstream::out );

    if( !outFile.is_open() ){
        cout << endl << "## Can't create " << fname << " ! ##"<< endl;
        return;
    }

    if( gTest == 1 || gTest == 6 )
        outFile << "==    FCFS==\n";
    if( gTest == 6 )
        outFile << gMapAnswer[1] << "\n";
    if( gTest == 2 || gTest == 6 )
        outFile << "==      RR==\n";
    if( gTest == 6 )
        outFile << gMapAnswer[2] << "\n";
    if( gTest == 3 || gTest == 6 )
        outFile << "==    PSJF==\n";
    if( gTest == 6 )
        outFile << gMapAnswer[3] << "\n";
    if( gTest == 4 || gTest == 6 )
        outFile << "==    NSJF==\n";
    if( gTest == 6 )
        outFile << gMapAnswer[4] << "\n";
    if( gTest == 5 || gTest == 6 )
        outFile << "==Priority==\n";
    if( gTest == 6 )
        outFile << gMapAnswer[5] << "\n";
    else
        outFile << gMap << "\n";

    outFile << "===========================================================\n\n";
    outFile << "Waiting Time\nID      ";

    if( gTest == 1 )
        outFile << "FCFS\n";
    if( gTest == 2 )
        outFile << "RR\n";
    if( gTest == 3 )
        outFile << "PSJF\n";
    if( gTest == 4 )
        outFile << "NSJF\n";
    if( gTest == 5 )
        outFile << "Priority\n";

    if( gTest == 6 )
    {
        outFile << "FCFS     RR      PSJF     NPSJF    Priority\n===========================================================\n";

        while( i <= aP-2 )
        {
            outFile << gProblem[i].id  << gWaitingAnswer[i] << "\n";
            i = i + 1;
        }
    }

    i = 0;
    while( i <= aP-2 && gTest != 6 )
    {
        outFile << gProblem[i].id << "        " << gProblem[i].waitingTime  << "\n";
            i = i + 1;
    }
    outFile << "===========================================================\n";
    i = 0;
    outFile << "\nTurnaround Time\nID      ";

    if( gTest == 1 )
        outFile << "FCFS\n";
    if( gTest == 2 )
        outFile << "RR\n";
    if( gTest == 3 )
        outFile << "PSJF\n";
    if( gTest == 4 )
        outFile << "NSJF\n";
    if( gTest == 5 )
        outFile << "Priority\n";

    if( gTest == 6 )
    {
        outFile << "FCFS     RR      PSJF     NPSJF    Priority\n===========================================================\n";

        while( i <= aP-2 )
        {
            outFile << gProblem[i].id  << gTurnaroundAnswer[i]  << "\n";
            i = i + 1;
        }
    }

    while( i <= aP-2 && gTest != 6 )
    {
        outFile << gProblem[i].id  << "       " <<  gProblem[i].turnaroundTime << "\n";
        i = i + 1;
    }
    outFile << "===========================================================";
    outFile.close();
}
