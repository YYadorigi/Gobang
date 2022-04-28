这是一个用于个人学习与实践的练习项目。用C++实现简单的五子棋程序。<br/>
<br/>
<br/>
<br/>
V1.1版本，实现功能如下：<br/>
1. 棋局存储与读取<br/>
采用文件记录未完成棋局，使得玩家可以保存残局、载入残局继续下棋。<br/>
2. 图形界面<br/>
使用EasyX图形界面开发框架，实现具有图形界面的五子棋程序。<br/>
3. AI对弈功能<br/>
实现玩家与电脑AI下棋的功能，AI具有一定的逻辑。<br/>
4. 玩家对战功能<br/>
实现本地双人对战功能，并且实现了悔棋操作。<br/>
<br/>
<br/>
<br/>
V1.x待优化问题：<br/>
1. 存档功能可以考虑进一步实现多个存档，及存档文本文件的路径检索待优化。<br/>
2. 一些按钮的鼠标点击判断使用了粗略坐标，可以考虑进一步提高精准度；以及可以把按钮的判定区域做成圆角矩形等，更加精确。<br/>
3. 玩家和AI对决时的选边实现。<br/>
4. Socket网络编程的实现。<br/>
5. 边缘落子待优化。<br/>
<br/>
<br/>
<br/>
2022.04.28更新：V1.1<br/>
1. 修复了AI对弈情况下玩家获胜时无法执行获胜弹窗的bug。<br/>
2. 实现了对人机对战和本地双人对战的分别存档。<br/>
<br/>
<br/>
2022.04.29更新：V1.2<br/>
1. 优化了类的封装。<br/>
2. 优化了悔棋后残留的黑色边框阴影。<br/>
3. 增加了追踪落子功能：现在，棋盘上会存在一个亮红色十字标记，它会随最近一次落子的位置而移动。<br/>
