#include"AI.h"
#include"menu.h"
#define BOARD_SIZE 15

int scoreMap[15][15] = { 0 };
extern int Map[15][15];
extern Menu menu;

//计算分数
void AI::CalculateScore() {
    //统计玩家或电脑连成的子
    int personNum = 0; //玩家连成子的个数
    int botNum = 0;   //AI连成子的个数
    int emptyNum = 0;   //各方向空白位的个数

    //清空评分数组
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            scoreMap[i][j] = 0;
        }
    }
    //计分
    /*计分个人理解：
     * 遍历每一个格子，判断哪些是空白的点(即为0的点)，以该点为中心，判断周围的八个点向外延伸的四格里，有多少個是黑子、白子、空白，以此作为依据来评分。下方算法是以守为主，所以守的分数>攻的分数
     */
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            //空白点才算
            if (row > 0 && col > 0 && Map[row][col] == 0) {
                //遍历周围8个方向
                for (int y = -1; y <= 1; y++) {
                    for (int x = -1; x <= 1; x++) {
                        //重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;
                        //原坐标不算
                        if (!(y == 0 && x == 0)) {
                            //每个方向延伸4个子

                            //对玩家黑子评分(正反两个方向)
                            for (int i = 1; i <= 4; i++) {
                                if (row + i * y > 0 && row + i * y < BOARD_SIZE &&
                                    col + i * x>0 && col + i * x < BOARD_SIZE &&
                                    Map[row + i * y][col + i * x] == 1) { //真人玩家的子
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < BOARD_SIZE &&
                                    col + i * x>0 && col + i * x < BOARD_SIZE &&
                                    Map[row + i * y][col + i * x] == 0) { //空白位
                                    emptyNum++;
                                    break;
                                }
                                else { //出边界，或有白子
                                    break;
                                }
                            }
                            for (int i = 1; i <= 4; i++) {
                                if (row - i * y > 0 && row - i * y < BOARD_SIZE &&
                                    col - i * x>0 && col - i * x < BOARD_SIZE &&
                                    Map[row - i * y][col - i * x] == 1) { //真人玩家的子
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < BOARD_SIZE &&
                                    col - i * x>0 && col - i * x < BOARD_SIZE &&
                                    Map[row - i * y][col - i * x] == 0) { //空白位
                                    emptyNum++;
                                    break;
                                }
                                else { //出边界，或有白子
                                    break;
                                }
                            }
                            if (personNum == 1) {                 //杀2
                                scoreMap[row][col] += 10;
                            }
                            else if (personNum == 2) {           //杀3
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 40;
                            }
                            else if (personNum == 3) {           //杀4
                               //量变空位不一样，优先级不一样
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 110;
                            }
                            else if (personNum == 4) {           //杀5
                                scoreMap[row][col] += 10100;
                            }

                            //进行一次清空
                            emptyNum = 0;

                            //对AI白子评分
                            for (int i = 1; i <= 4; i++) {
                                if (row + i * y > 0 && row + i * y < BOARD_SIZE &&
                                    col + i * x>0 && col + i * x < BOARD_SIZE &&
                                    Map[row + i * y][col + i * x] == -1) { //AI的子
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < BOARD_SIZE &&
                                    col + i * x>0 && col + i * x < BOARD_SIZE &&
                                    Map[row + i * y][col + i * x] == 0) { //空白位
                                    emptyNum++;
                                    break;
                                }
                                else { //出边界
                                    break;
                                }
                            }
                            for (int i = 1; i <= 4; i++) {
                                if (row - i * y > 0 && row - i * y < BOARD_SIZE &&
                                    col - i * x>0 && col - i * x < BOARD_SIZE &&
                                    Map[row - i * y][col - i * x] == -1) { //AI的子
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < BOARD_SIZE &&
                                    col - i * x>0 && col - i * x < BOARD_SIZE &&
                                    Map[row - i * y][col - i * x] == 0) { //空白位
                                    emptyNum++;
                                    break;
                                }
                                else { //出边界
                                    break;
                                }
                            }
                            if (botNum == 0) {
                                scoreMap[row][col] += 5;  //活1
                            }
                            else if (botNum == 1) {
                                scoreMap[row][col] += 10; //活2
                            }
                            else if (botNum == 2) {         //活3
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 50;
                            }
                            else if (botNum == 3) {         //活4
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 100;
                            }
                            else if (botNum >= 4) {         //活5
                                scoreMap[row][col] += 20000;
                            }


                        }
                    }
                }
            }
        }
    }

}

//AI执行下棋
void AI::ActionByAI(int& clickRow, int& clickCol) {
    //计算评分
    CalculateScore();

    //从评分中找出最大分数的位置
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;
    for (int row = 1; row < BOARD_SIZE; row++) {
        for (int col = 1; col < BOARD_SIZE; col++) {
            //前提是这个坐标是空的
            if (Map[row][col] == 0) {
                if (scoreMap[row][col] > maxScore) {     //找最大数和坐标
                    maxPoints.clear();
                    maxScore = scoreMap[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreMap[row][col] == maxScore) {   //如果有多个最大值就將他们存储起来，在后面的代码随机抽1个
                    maxPoints.push_back(std::make_pair(row, col));
                }
            }
        }
    }
    //随机落子，如果有多个点的话
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();
    std::pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first;
    clickCol = pointPair.second;
    Sleep(300);
    menu.DropChessWhite(clickRow, clickCol);
}
