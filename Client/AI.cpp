#include"AI.h"
#include"menu.h"
#define BOARD_SIZE 15

int scoreMap[15][15] = { 0 };
extern int Map[15][15];
extern Menu menu;

//�������
void AI::CalculateScore() {
    //ͳ����һ�������ɵ���
    int personNum = 0; //��������ӵĸ���
    int botNum = 0;   //AI�����ӵĸ���
    int emptyNum = 0;   //������հ�λ�ĸ���

    //�����������
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            scoreMap[i][j] = 0;
        }
    }
    //�Ʒ�
    /*�Ʒָ�����⣺
     * ����ÿһ�����ӣ��ж���Щ�ǿհ׵ĵ�(��Ϊ0�ĵ�)���Ըõ�Ϊ���ģ��ж���Χ�İ˸�������������ĸ���ж��ق��Ǻ��ӡ����ӡ��հף��Դ���Ϊ���������֡��·��㷨������Ϊ���������صķ���>���ķ���
     */
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            //�հ׵����
            if (row > 0 && col > 0 && Map[row][col] == 0) {
                //������Χ8������
                for (int y = -1; y <= 1; y++) {
                    for (int x = -1; x <= 1; x++) {
                        //����
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;
                        //ԭ���겻��
                        if (!(y == 0 && x == 0)) {
                            //ÿ����������4����

                            //����Һ�������(������������)
                            for (int i = 1; i <= 4; i++) {
                                if (row + i * y > 0 && row + i * y < BOARD_SIZE &&
                                    col + i * x>0 && col + i * x < BOARD_SIZE &&
                                    Map[row + i * y][col + i * x] == 1) { //������ҵ���
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < BOARD_SIZE &&
                                    col + i * x>0 && col + i * x < BOARD_SIZE &&
                                    Map[row + i * y][col + i * x] == 0) { //�հ�λ
                                    emptyNum++;
                                    break;
                                }
                                else { //���߽磬���а���
                                    break;
                                }
                            }
                            for (int i = 1; i <= 4; i++) {
                                if (row - i * y > 0 && row - i * y < BOARD_SIZE &&
                                    col - i * x>0 && col - i * x < BOARD_SIZE &&
                                    Map[row - i * y][col - i * x] == 1) { //������ҵ���
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < BOARD_SIZE &&
                                    col - i * x>0 && col - i * x < BOARD_SIZE &&
                                    Map[row - i * y][col - i * x] == 0) { //�հ�λ
                                    emptyNum++;
                                    break;
                                }
                                else { //���߽磬���а���
                                    break;
                                }
                            }
                            if (personNum == 1) {                 //ɱ2
                                scoreMap[row][col] += 10;
                            }
                            else if (personNum == 2) {           //ɱ3
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 40;
                            }
                            else if (personNum == 3) {           //ɱ4
                               //�����λ��һ�������ȼ���һ��
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 110;
                            }
                            else if (personNum == 4) {           //ɱ5
                                scoreMap[row][col] += 10100;
                            }

                            //����һ�����
                            emptyNum = 0;

                            //��AI��������
                            for (int i = 1; i <= 4; i++) {
                                if (row + i * y > 0 && row + i * y < BOARD_SIZE &&
                                    col + i * x>0 && col + i * x < BOARD_SIZE &&
                                    Map[row + i * y][col + i * x] == -1) { //AI����
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < BOARD_SIZE &&
                                    col + i * x>0 && col + i * x < BOARD_SIZE &&
                                    Map[row + i * y][col + i * x] == 0) { //�հ�λ
                                    emptyNum++;
                                    break;
                                }
                                else { //���߽�
                                    break;
                                }
                            }
                            for (int i = 1; i <= 4; i++) {
                                if (row - i * y > 0 && row - i * y < BOARD_SIZE &&
                                    col - i * x>0 && col - i * x < BOARD_SIZE &&
                                    Map[row - i * y][col - i * x] == -1) { //AI����
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < BOARD_SIZE &&
                                    col - i * x>0 && col - i * x < BOARD_SIZE &&
                                    Map[row - i * y][col - i * x] == 0) { //�հ�λ
                                    emptyNum++;
                                    break;
                                }
                                else { //���߽�
                                    break;
                                }
                            }
                            if (botNum == 0) {
                                scoreMap[row][col] += 5;  //��1
                            }
                            else if (botNum == 1) {
                                scoreMap[row][col] += 10; //��2
                            }
                            else if (botNum == 2) {         //��3
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 50;
                            }
                            else if (botNum == 3) {         //��4
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 100;
                            }
                            else if (botNum >= 4) {         //��5
                                scoreMap[row][col] += 20000;
                            }


                        }
                    }
                }
            }
        }
    }

}

//AIִ������
void AI::ActionByAI(int& clickRow, int& clickCol) {
    //��������
    CalculateScore();

    //���������ҳ���������λ��
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;
    for (int row = 1; row < BOARD_SIZE; row++) {
        for (int col = 1; col < BOARD_SIZE; col++) {
            //ǰ������������ǿյ�
            if (Map[row][col] == 0) {
                if (scoreMap[row][col] > maxScore) {     //�������������
                    maxPoints.clear();
                    maxScore = scoreMap[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreMap[row][col] == maxScore) {   //����ж�����ֵ�͌����Ǵ洢�������ں���Ĵ��������1��
                    maxPoints.push_back(std::make_pair(row, col));
                }
            }
        }
    }
    //������ӣ�����ж����Ļ�
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();
    std::pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first;
    clickCol = pointPair.second;
    Sleep(300);
    menu.DropChessWhite(clickRow, clickCol);
}
