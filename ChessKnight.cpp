using namespace std;
#include <iostream>
#include "Graph.h"
#include <fstream>
#include <random>
#include <string>

class coord;
static int BOARDSIZE = 8;
/*
   utility definitions used to make life easier for myself
*/
typedef vector< vector<bool> > bMatrix;
typedef vector<bool> bRow;
typedef vector< vector<int> > Matrix;
typedef vector<int> Row;
typedef vector< vector<coord> > cMatrix;
typedef vector<coord> cRow;

/*
   class to represent coordinates cuz it made more sense to me.
   various overloaded operators and friend functions
*/
class coord {
public:
   coord(int X = 0, int Y = 0) {
      x = X;
      y = Y;
   }
   friend ostream& operator<<(ostream& os, const coord& c1) {
      os << "<" << c1.x << "," << c1.y << ">";
      return os;
   }

   friend bool operator ==(const coord& c1, const coord& c2) {
      return (c1.x == c2.x) && (c1.y == c2.y);
   }

   friend coord operator +(const coord& c, int i) {
      return coord(c.x + i, c.y + i);
   }

   friend coord operator -(const coord& c, int i) {
      return coord(c.x - i, c.y - i);
   }
   
   string tostring() {
      string s = "(";
      s += to_string(x);
      s += ", ";
      s += to_string(y);
      s += ")";
      return s;
   }
   friend void printpath(cMatrix pred, Matrix dist, coord end);

   friend bool valid(coord c);

   friend void Dijpath(coord start, coord end);

   friend vector<coord> moveset(coord c, bMatrix visited);

   friend class graph;
private:
   int x;
   int y;
};


/*
   checks to see of coordinate is within the board
*/
bool valid(coord c) {
   return ((c.x >= 0) && (c.x < BOARDSIZE)) && ((c.y >= 0) && (c.y < BOARDSIZE));
}

/*
   returns a vector of coordiantes that can be visited from given coordinate,
   provided they havent already been visited and are on the board
*/
vector<coord> moveset(coord c, bMatrix visited) {
   vector<coord> list;
   coord c1 = coord(c.x + 1, c.y - 2);
   if (valid(c1) && visited[c.x ][c.y ] == false) { list.push_back(c1); }
   c1 = coord(c.x + 2, c.y - 1);
   if (valid(c1) && visited[c.x ][c.y ] == false) { list.push_back(c1);}
   c1 = coord(c.x + 2, c.y + 1 );
   if (valid(c1) && visited[c.x ][c.y ] == false) { list.push_back(c1);}
   c1 = coord(c.x + 1, c.y + 2);
   if (valid(c1) && visited[c.x ][c.y ] == false) { list.push_back(c1);}
   c1 = coord(c.x - 1, c.y + 2);
   if (valid(c1) && visited[c.x ][c.y ] == false) { list.push_back(c1);}
   c1 = coord(c.x - 2, c.y + 1);
   if (valid(c1) && visited[c.x ][c.y ] == false) { list.push_back(c1);}
   c1 = coord(c.x - 2, c.y - 1);
   if (valid(c1) && visited[c.x ][c.y ] == false) { list.push_back(c1);}
   c1 = coord(c.x - 1 , c.y - 2);
   if (valid(c1) && visited[c.x ][c.y ] == false) { list.push_back(c1);}
   return list;
}

/*
   generates 2d vector of coordinates to represent the chessboard
*/
Graph<coord> chessboard() {
   Graph<coord> g = Graph<coord>();
   for (int i = 0; i < BOARDSIZE; i++) {
      for (int j = 0; j < BOARDSIZE; j++) {
         g.InsertVertex(coord(i, j));
      }
   }
   return g;
}

/*
   checks if a coordinate is within a list
*/
bool inList(vector<coord> list, coord c) {
   for (int i = 0; i < list.size(); i++) {
      if (list[i] == c) {
         return true;
      }
   }
   return false;
}

/*
   returns the position of a coordinate in a list of coordinates if it is in there. returns -1 otherwise
*/
int findPos(vector<coord> list, coord c) {
   for (int i = 0; i < list.size(); i++) {
      if (list[i] == c) {
         return i;
      }
   }
   return -1;
}

/*
   returns the position of the coordinate with the least distant within the list of unvisited coordiantes
*/
int minKey(vector<coord> list, Matrix dist, bMatrix visited) {
   int min = INT_MAX; int min_index = 0;
   for (int i = 0; i < BOARDSIZE;i++) {
      for (int j = 0; j < BOARDSIZE; j++) {
         if (visited[i][j] == false  && dist[i][j] < min) {
            //cout << "looking for: " << i << "," << j << endl;
            min = dist[i][j];
            min_index = findPos(list, coord(i, j));
            //cout << "min dist: " <<dist[i][j]<<" at index: " << min_index << endl;
         }
      }
   }
   return min_index;
}

/*
   utility function to print 2d vectors neatly
*/
string fitCell(string s, int n) {
   while (s.size() < n) {
      s += " ";
   }
   return s;
}

/*
   prints 2d vector of integers
*/
void printMatrix(Matrix matrix) {
   int CELLSIZE = 10;
   string s = "";
   for (int i = 0; i < matrix.size() + 1; i++) {
      if (i != 0) {
         s = to_string(i);
         cout << fitCell(s, CELLSIZE);
      }
      else {
         cout << fitCell(s, CELLSIZE);
      }
      for (int j = 0; j < matrix.size(); j++) {
         if (i == 0) {
            s = to_string(j + 1);
            cout << fitCell(s, CELLSIZE);
         }
         else {
            s = to_string(matrix[i - 1][j]);
            cout << fitCell(s, CELLSIZE);
         }
      }
      cout << "\n";
   }
}

/*
   prints matrix of parent coordinates for each individual position
*/
void printpathMatrix(cMatrix matrix) {
   int CELLSIZE = 10;
   string s = "";
   for (int i = 0; i < matrix.size() + 1; i++) {
      if (i != 0) {
         s = to_string(i-1);
         cout << fitCell(s, CELLSIZE);
      }
      else {
         cout << fitCell(s, CELLSIZE);
      }
      for (int j = 0; j < matrix.size(); j++) {
         if (i == 0) {
            s = to_string(j );
            cout << fitCell(s, CELLSIZE);
         }
         else {
            s = (matrix[i - 1][j]. tostring());
            cout << fitCell(s, CELLSIZE);
         }
      }
      cout << "\n";
   }
}

/*
   recursive function that goes through matrix of parent coordiantes and prints out the shortest path
*/
void printpath(cMatrix pred, Matrix dist, coord end) {
   if (dist[end.x][end.y] > 0) {
      printpath(pred, dist, pred[end.x][end.y]);
   }
   cout << end+1 << endl;
}

/*
   generates a bunch of 2d vectors, one represents distance from source node, one represents if coord has been visited,
   one represents the parent coord of the coord at that position, and a vector of unvisited coords

   implementation of the dijkstra algorithm to figure out MST of a chessboard for a knight from a starting coordinate
   prints out the end path using utility functions
*/
void Dijpath(coord start, coord end) {
   Graph<coord> board = chessboard();
   Matrix dist;
   for (int i = 0; i < 8; i++) {
      Row row(8, INT_MAX);
      dist.push_back(row);
   }
   bMatrix visited;
   for (int i = 0; i < 8; i++) {
      bRow row(8, false);
      visited.push_back(row);
   }
   cMatrix pred;
   for (int i = 0; i < 8; i++) {
      cRow row(8, coord(-1,-1));
      pred.push_back(row);
   }
   coord v;
   vector<coord> unvisited = board.getVertexes();
   dist[start.x][start.y] = 0;
   pred[start.x][start.y] = coord(-1,-1);
   while (!unvisited.empty()) {
      //cout << "hola" << endl;
      v = unvisited[minKey(unvisited, dist, visited)];
      vector<coord> moves = moveset(v, visited);
      //v = coord(v.x + 1, v.y + 1);
      visited[v.x ][v.y ] = true;
      //cout << "v is: "<<v << endl;
      for (int i = 0; i < unvisited.size(); i++) {
         if (unvisited[i] == v) {
            unvisited.erase(unvisited.begin() + i);
            break;
         }
      }
      for (int i = 0; i < moves.size(); i++) {
         coord u = moves[i];
         if (dist[u.x ][u.y ] > (dist[v.x ][v.y ]) + 1) {
            //cout << "adjusting dist of" << u.x << "," << u.y << " in relation to " << v.x << "," << v.y << endl;
            dist[u.x ][u.y ] = (dist[v.x ][v.y ]) + 1;
            pred[u.x][u.y] = coord(v.x , v.y );
            //cout <<"pred of " << u.x << "," << u.y << "is " << v.x << "," << v.y << endl;
         }
      }
   }
   cout << "Path from " << start+1 << " to " << end+1 << " takes " << dist[end.x][end.y] << " steps, which are:" << endl;
   /*printpathMatrix(pred);
   printMatrix(dist);*/
   printpath(pred, dist, end);
}

int main() {
   int startx, starty, endx, endy;
   bool running = true;
   coord start, end;
   string input;
   cout << "Please enter coordinates in terms of 1 to 8 for x and y, separated by spaces, then the enter key" << endl;
   while (running) {
      cout << "Enter the Knight's current location: " << endl;
      cin >> startx >> starty;
      cout << "Enter the Knight's destination location: " << endl;
      cin >> endx >> endy;
      start = coord(startx - 1, starty - 1);
      end = coord(endx - 1, endy - 1);
      Dijpath(start, end);
      cout << "Would you like to test another path? (Y/N)" << endl;
      cin >> input;
      if (input == "Y") {running = true;}
      if (input == "N") { running = false; cout << "Thanks for playing, see ya later" << endl; }
   }
}