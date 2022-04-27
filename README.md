这是一个用于个人学习与实践的练习项目。
用C++实现简单的五子棋程序。



V1.0版本，实现功能如下：
1. 棋局存储与读取
采用文件记录未完成棋局，使得玩家可以保存残局、载入残局继续下棋。
2. 图形界面
使用EasyX图形界面开发框架，实现具有图形界面的五子棋程序。
3. AI对弈功能
实现玩家与电脑AI下棋的功能，AI具有一定的逻辑。
4. 玩家对战功能
实现本地双人对战功能，并且实现了悔棋操作。



V1.0待优化问题
1.存档功能可以考虑进一步实现多个存档，并对人机存档和双人存档进行区分。

2.一些按钮的鼠标点击判断使用了粗略坐标，可以考虑进一步提高精准度；以及可以把按钮的判定区域做成圆角矩形等，更加精确。
3.可以考虑自定义棋盘大小的实现。要想实现此功能，最好将棋盘类的成员变量由二维数组改为其指针。
4.玩家和AI对决时的选边实现。
5.Socket网络编程的实现。
6.边缘落子待优化。
7.悔棋后留下的黑色边框阴影有待优化。
