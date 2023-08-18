package maze_solution;

import java.io.File;
import java.util.ArrayList;
import java.util.Scanner;

public class MazeSolution {

	public static void main(String[] args) throws Exception{
		Scanner in = new Scanner(System.in);
		System.out.println("Please enter the size(不能大于20) and name of your maze(数据的文件名)");
		int n = in.nextInt();
		String filename = in.next();
		File file=new File(filename);
//读取文件里面的数据，将其存入map二维数组中
		Scanner input = new Scanner(file);
		int[][] map = new int[n][n];
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				map[i][j] = input.nextInt();
			}
		}

//		这是开始时间，用于记录搜索耗费的时间
		long startTime = System.currentTimeMillis();

//		定义地图的基本数据，长宽高，大小
		MapStructure QuestionMap = new MapStructure(map, map.length, new Point(0, 0),
				new Point(map.length-1, map.length-1));

		new Algorithm().Go(QuestionMap);
//		打印最后的map
		printAnswer(map);
//		记录结束时间
		long endTime=System.currentTimeMillis();
		System.out.println("This program takes " + (endTime - startTime) + "ms.");
	}


	public static void printAnswer(int[][] maps)
	{
		for (int i = 0; i < maps.length; i++)
		{
			for (int j = 0; j < maps[i].length; j++)
			{
				if (maps[i][j] == 1) {
//					这个代表墙体
					System.out.print("🟦");
				}
				if (maps[i][j] == 0) {
//					这个代表路
					System.out.print("0️⃣");
				}
				if (maps[i][j] == 2) {
//					这个代表最终的路径
					System.out.print("*️⃣");
				}
//				System.out.print(maps[i][j] + " ");
			}
			System.out.println();
		}
	}

}

class Coord{
	int x;
	int y;

	Coord(int x, int y){
		this.x = x;
		this.y = y;
	}
//判断是否已经到达了终点
	boolean isEquals(Coord goal){
		if(goal.x == x && goal.y == y)
			return true;
		return false;
	}
}

class Point{
	Coord coord;
	Point last;
//	A*算法的两个参数
	int Gx;	//移动距离
	int Hx;	//离终点的距离
	int air;

	//节点坐标
	Point(int x, int y){
		this.coord = new Coord(x,y);
	}

	Point(Coord coord, Point parent, int Gx, int Hx){
		this.coord = coord;
		this.last = parent;
		this.Gx = Gx;
		this.Hx = Hx;
	}
	Point(int air){
		this.air = air;
	}
}

//MapStructure定义结构
class MapStructure{
	int[][] map;
	int n;	//地图的宽和高
	Point start;
	Point end;

	MapStructure(int [][]map, int n, Point start,Point end){
		this.map = map;
		this.n = n;
		this.start = start;
		this.end = end;
	}

	void setMapStructure(Point start,Point end){
		this.start = start;
		this.end = end;
	}
}

//完成A*算法，并求出路径
class Algorithm{
//	1代表墙
	final static int bar = 1;
//	2代表选中，走的路径
	final static int sign = 2;
	final static int value = 1; //只能上下左右移动

	ArrayList<Point> openList = new ArrayList<Point>();		//放入可移动的路径
	ArrayList<Point> closeList = new ArrayList<Point>();	//放入走过的路径

	boolean isEndCoord(Coord coord, Coord end){
		if(coord != null && end.isEquals(coord))
//			已经倒带终点
			return true;
		return false;
	}
//是否能加入移动路径
	boolean canAddopenList(MapStructure map, int x, int y){
		if(x<0 || x>=map.n || y<0 || y>=map.n)
//			不能越界
			return false;
		if(map.map[y][x] == bar)
//			不能等于墙
			return false;
		if(isIncloseList(x, y))
			return false;
		if(isInopenList(x, y))
			return false;
//		以上判断判断都不符合，可以加入
		return true;
	}
//	是否在放入可移动的路径
	boolean isInopenList(int x, int y){
		for(Point point : openList){
			if(x == point.coord.x && y == point.coord.y)
				return true;
		}
		return false;
	}
//  是否在放入走过的路径
	boolean isIncloseList(int x, int y){
		if(closeList.isEmpty())
			return false;
		for(Point point : closeList){
			if(x == point.coord.x && y == point.coord.y)
				return true;
		}
		return false;
	}

	int calcHx(Coord goal, Coord end){
//		欧式距离
		return Math.abs(goal.x - end.x) + Math.abs(goal.y - end.y);
	}

//	增加可移动的路径列表
	void addPointInopenList(MapStructure map, Point current, int x, int y){
		if(canAddopenList(map, x, y)){
			Point end = map.end;
			Point goal = new Point(x, y);
//			当前的距离加1
			int Gx = current.Gx + value;
//			这是与目标的距离
			int Hx = calcHx(current.coord, end.coord);
//			判断是否在终点
			if(isEndCoord(goal.coord, end.coord)){
				goal = end;
				goal.last = current;
				goal.Gx = Gx;
				goal.Hx = calcHx(goal.coord, end.coord);
			}
			else
//				如果不在，继续寻找
				goal = new Point(goal.coord, current, Gx, Hx);
//			加入放入可移动的路径
			openList.add(goal);
		}
	}

	void addPointInopenList(MapStructure map, Point current){
		int x = current.coord.x;
		int y = current.coord.y;

		addPointInopenList(map, current, x-1, y);	//左
		addPointInopenList(map, current, x+1, y);	//右
		addPointInopenList(map, current, x, y-1);	//上
		addPointInopenList(map, current, x, y+1);	//下
	}

	void drawPath(int[][] map, Point end){
		while(end != null){
			map[end.coord.y][end.coord.x] = sign;
			end = end.last;
		}
	}
//获得路径信息
	void getPathInformation(int[][] map, Point end){
		int path = 1;
		System.out.println("The shortest path needs " + end.Gx + " steps.");
		while(end != null){
//			可移动路径进行遍历
			for(Point point : openList){
				if(point.Hx == end.Hx && point.Gx == end.Gx){
					path++;
				}
			}
			map[end.coord.y][end.coord.x] = sign;
			end = end.last;
		}
		System.out.println("The shortest path number is " + path);
	}

	//从地图左上角为所有的0赋予
	void addBreathbyLeft(MapStructure map){
		int i = 0;
		int j = 0;
		Point zero = new Point(j, i);
		for (i = 1; i < map.n-1; i++) {
			for (j = 1; j < map.n-1; j++) {
				zero.air = 0;
				if(map.map[i][j] == 0){
					if(map.map[i][j+1] == 0)
						zero.air++;
					if(j >= 1 && map.map[i][j-1] == 0)
						zero.air++;
					if(map.map[i+1][j] == 0)
						zero.air++;
					if(i >= 1 && map.map[i-1][j] == 0)
						zero.air++;
				}

				if(zero.air <= 1){		//判断迷宫中只有一格气的点，说明它是死胡同，将它改为1
					map.map[i][j] = bar;
//					System.out.println("0➡1");
				}
			}
		}
	}

	//从地图右下角为所有的0赋予气
		void addBreathbyRight(MapStructure map){
			int i = 0;
			int j = 0;
			Point zero = new Point(j, i);
			for (i = map.n-2; i > 0; i--) {
				for (j = map.n-2; j > 0; j--) {
					zero.air = 0;
					if(map.map[i][j] == 0){
						if(map.map[i][j+1] == 0)
							zero.air++;
						if(j >= 1 && map.map[i][j-1] == 0)
							zero.air++;
						if(map.map[i+1][j] == 0)
							zero.air++;
						if(i >= 1 && map.map[i-1][j] == 0)
							zero.air++;
					}
					if(zero.air <= 1){		//判断迷宫中只有一格气的点，说明它是死胡同，将它改为1
						//System.out.println("0➡1");
						map.map[i][j] = bar;

					}
				}
			}
		}

	//从左上和右下两次遍历，将所有死胡同堵死
	void changeBreath(MapStructure map){
		for (int i = 0; i < map.map.length-1; i++) {
			addBreathbyLeft(map);
			addBreathbyRight(map);
		}
		System.out.println("Get rid of all dead paths (they don't have extra breath)");
	}

	void Go(MapStructure map){
		if(map == null){
//			初始化失败
			System.out.println("The map structure is wrong!");
			return;
		}
		openList.clear();
		closeList.clear();

		changeBreath(map);

		openList.add(map.start);
		movePoint(map);
		getPathInformation(map.map, map.end);
		System.out.println("This is all maze paths (The sign of the pathway is '2'):");
	}

	void Back(MapStructure map){
		openList.clear();
		closeList.clear();
		openList.add(map.start);
		movePoint(map);
	}

	Point findMinInopenList(){
		Point goal = openList.get(0);
		for(Point point : openList){
			if(point.Gx + point.Hx < goal.Gx + goal.Hx)
				goal = point;
		}
		return goal;
	}

	void movePoint(MapStructure map){
		while(!openList.isEmpty()){
			Point goal = findMinInopenList();
			openList.remove(goal);
			closeList.add(goal);
			addPointInopenList(map, goal);
			if(isIncloseList(map.end.coord.x, map.end.coord.y)){
				drawPath(map.map, map.end);
				break;
			}
		}
	}
}

