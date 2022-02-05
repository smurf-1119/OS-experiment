#include <iostream>
#include <fstream>

#define PROGRESS 5     //��������
#define REC_NUM  3     //��Դ��������

using namespace std;

//�������ݽṹ

int Available[PROGRESS];      //���������Դ����Available
int sign[PROGRESS], work[PROGRESS][REC_NUM], workAll[PROGRESS][REC_NUM];
//��¼�ɹ��İ�ȫ����,�����幤������Ϳ�����Դ����



int Max[PROGRESS][REC_NUM], Allocation[PROGRESS][REC_NUM], Need[PROGRESS][REC_NUM];
//��������������Max���������Allocation���������Need


void Read_file();     //��ȡ�ļ�
int Banker_Algorithm(int, int[][REC_NUM]);       //���м��㷨
int Safe_Algorithm(int[], int[][REC_NUM], int[][REC_NUM]);    //��ȫ���㷨
void Print_Run_Order(int);         //��ӡ�ж�ִ������

//���м��㷨
int Banker_Algorithm(int i, int Request[][REC_NUM]) {
    for (int m = 0; m < REC_NUM; m++) {
        if (Request[i][m] > Need[i][m]) {
            cout << "������Դ�����������������ֵ!" << endl;
            return 0;
        }
        else if (Request[i][m] > Available[m]) {
            cout << "���㹻��Դ��p[" << i << "]��ȴ�!" << endl;
            return 0;
        }
    }

    //����Ϊ���̷�����Դ
    for (int j = 0; j < REC_NUM; j++) {
        Available[j] = Available[j] - Request[i][j];
        Allocation[i][j] = Allocation[i][j] + Request[i][j];
        Need[i][j] = Need[i][j] - Request[i][j];
    }

    //ִ�а�ȫ���㷨
    int n = Safe_Algorithm(Available, Need, Allocation);
    cout << endl;

    if (n == PROGRESS) {//��5��'true'����1����ʾ��ʱ�̰�ȫ
        cout << "��ʱ���ǰ�ȫ״̬�����Է�����Դ��" << "P[" << i << "]!" << endl;
    }
    else {

        //�Ѹ�����P[i]���������Դ����ϵͳ
        for (int j = 0; j < REC_NUM; j++) {
            Available[j] = Available[j] + Request[i][j];
            Allocation[i][j] = Allocation[i][j] - Request[i][j];
            Need[i][j] = Need[i][j] + Request[i][j];
        }
        cout << "��ʱ�̲��ǰ�ȫ״̬�����ܽ���Դ�����" << "P[" << i << "]!" << endl;
    }
    return n;
}


//��ȡ�ļ����ݣ���ӡ������̨���������ݴ��뵽��Ӧ������
void Read_file() {
    //��ȡ�����ļ�����ӡ
    ifstream inFile1("Alldata.txt", ios::in);  //�����ļ�������
    if (!inFile1)      //�ж϶���inFile���ļ��ɹ����
        cerr << "File open error." << endl;  //ʹ�ô������������������Ϣ
    else {
        char c;
        while ((c = inFile1.get()) != EOF)  //���ַ���ȡ�ļ����ݣ�����ĩβֹͣ
            cout << c;
        cout << endl;
        inFile1.close();
    }

    //��ȡֻ�����ֵ��ļ������������
    ifstream inFile2("data.txt", ios::in);
    if (!inFile2)
        cerr << "File open error." << endl;
    else {
        int data;
        //��ȡ���ֲ��������
        for (int j = 0; j < PROGRESS; j++) {
            for (int i = 0; i < REC_NUM; i++) {
                inFile2 >> data;
                Max[j][i] = data;
            }
            for (int i = 0; i < REC_NUM; i++) {
                inFile2 >> data;
                Allocation[j][i] = data;
            }
            for (int i = 0; i < REC_NUM; i++) {
                inFile2 >> data;
                Need[j][i] = data;
            }
            if (j == 0) {
                for (int i = 0; i < REC_NUM; i++) {
                    inFile2 >> data;
                    Available[i] = data;
                }
            }
        }
        inFile2.close();
    }
}

//��ȫ���㷨
int Safe_Algorithm(int Available[], int Need[][REC_NUM], int Allocation[][REC_NUM]) {
    int i = 0, j = 0, m = 0, n = 0;
    int Work[REC_NUM], Finish[PROGRESS] = { 0,0,0,0,0 };
    for (int r = 0; r < REC_NUM; r++) //��������Դ��Ŀ������������Work
        Work[r] = Available[r];
    while (i < PROGRESS) {
        if (Finish[i] == 0) {
            //���������ͷ���Դ������ͷ��ʼɨ����̼���
            while (j < REC_NUM && Need[i][j] <= Work[j])
                j++;
            if (j == REC_NUM) {
                for (int k = 0; k < REC_NUM; k++) {
                    work[i][k] = Work[k];
                    Work[k] = Work[k] + Allocation[i][k];
                    workAll[i][k] = Work[k];
                }
                Finish[i] = 1;
                sign[m] = i;  //���氲ȫ����
                i = -1; m++;
            }
        }
        j = 0; i++;
    }
    for (int p = 0; p < PROGRESS; p++) {
        if (Finish[p] == 1)
            n++;   //��¼'true'����
    }
    return n;     //����'true'����
}

//��ӡ��ȫ�Լ���ִ������
void Print_Run_Order(int result) {
    if (result == PROGRESS) {
        cout << " ����\\��Դ���" << " Work(A B C)" << " Need(A B C)"
            << " Allocation(A B C)" << " Work+Available(A B C)" << " Finish";
        cout << endl;
        for (int i = 0; i < PROGRESS; i++) {
            cout << "    " << "P[" << sign[i] << "]  " << '\t';
            for (int j = 0; j < REC_NUM; j++)
                cout << work[sign[i]][j] << " ";
            cout << '\t' << '\t';
            for (int j = 0; j < REC_NUM; j++)
                cout << Need[sign[i]][j] << " ";
            cout << '\t' << '\t';
            for (int j = 0; j < REC_NUM; j++)
                cout << Allocation[sign[i]][j] << " ";
            cout << '\t' << '\t';
            for (int j = 0; j < REC_NUM; j++)
                cout << workAll[sign[i]][j] << " ";
            cout << '\t' << '\t';
            cout << "true" << endl;
        }
        cout << endl << "�ҵ���ȫ���У�P[" << sign[0] << "]";
        for (int m = 1; m < PROGRESS; m++) {
            cout << ", P[" << sign[m] << "]";
        }
        cout << "}" << endl;
    }
    else {
        cout << "   ����\\��Դ��� " << "  Allocation(A B C)" << "   Need(A B C)" << "   Available(A B C)";
        cout << endl;
        for (int k = 0; k < 5; k++) {
            cout << '\t' << "P[" << k << "]" << '\t' << '\t';
            for (int j = 0; j < 3; j++)
                cout << Allocation[k][j] << " ";
            cout << '\t' << '\t';
            for (int j = 0; j < 3; j++)
                cout << Need[k][j] << " ";
            cout << '\t' << '\t';
            if (k == 0) {
                for (int j = 0; j < 3; j++)
                    cout << Available[j] << " ";
            }
            cout << endl;
        }
    }
}

int main()
{
    //��ȡ�ļ����ݲ���ӡ,Ȼ�����ݴ��뵽��Ӧ������
    Read_file();

    //����ʼ
    int i, N = 0;    // 'N'��ʾ������Դ����
    int Request[PROGRESS][REC_NUM];  //�����������Requset
    while (N != 999) {
        cout << endl << "������������ԴRequest[���̱��i][��Դ����j]:" << endl;
        cout << "����i=��";
        cin >> i;
        cout << "������Դ����(A B C)=:  ";
        for (int m = 0; m < REC_NUM; m++)
            cin >> Request[i][m];
        cout << endl;
        //ִ�����м��㷨
        int result = Banker_Algorithm(i, Request);
        //���ÿ���жϲ�����ִ������
        cout << endl << "��Դ�����" << endl;
        Print_Run_Order(result);
        cout << endl << "������N(��N=999�˳�)��" << endl;
        cin >> N;
    }
}


