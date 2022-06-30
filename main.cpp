#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include "List.h"
#include<thread>
#include<mutex>
#include<math.h>
using namespace std;
class coords//a container class for the  node // this class is wrapper for all the data to be used for a individual node
{
public:
    int cost;//cost till now
    double heuristic;
    int r;
    int c;
    coords()
    {
        cost = 1;//default cost is 1

    }
    coords(int rr, int cc, int co = 1)
    {
        r = rr;
        c = cc;
        cost = co;
    }
    bool operator==(coords two)
    {
        if (r == two.r && c == two.c)
            return true;
        else
            return false;
    }
};
class vertex//object for the map
{
public:
    coords b;
    List<coords> vert;
    vertex()
    {


    }
    vertex(coords x)
    {
        b.r = x.r;
        b.c = x.c;
    }
};
class A
{
public:
    coords v;
    double cost;
    double heuristic;
    double sum;//sum=cost+heuristic
    coords path;
    A()
    {


    }
    A(coords& v1, double c, double h, coords p)
    {
        v = v1;
        cost = c;
        heuristic = h;
        sum = cost + heuristic;
        path = p;

    }

};
double calc_heuristic(coords origin, coords dest)
{
    //  return sqrt(pow(origin.r-dest.r,2)+pow(origin.c-dest.r,2));
    return abs(dest.r - origin.r) + abs(dest.c - origin.c);
}
///////////////////////////////////////////////////////////////////////// function for A*
int smallest_entry(List<A>& open)
{
    A temp = open[0];
    int index = 0;
    for (int i = 0; i < open.howMany(); i++)
    {
        if (temp.sum > open[i].sum)
        {
            temp = open[i];
            index = i;

        }
    }
    return index;
}
int isThere(coords b, List<A>& closed)//returns the index //for class A
{
    for (int i = 0; i < closed.howMany(); i++)
    {
        if (b == closed[i].v)
            return i;
    }
    return -1;

}
int find_index(coords b, List<vertex> graph)//for vertex class only
{
    for (int i = 0; i < graph.howMany(); i++)
    {
        if (b == graph[i].b)
            return i;
    }
    return -1;
}
void find_path(coords origin, coords goal, List<A>& closed, List<coords>& target)//takes in the closed list and returns the path
{
    target.add_front(goal);
    int index = isThere(goal, closed);
    if (index != -1 && !(closed[index].path == origin))
    {
        find_path(origin, closed[index].path, closed, target);

    }
    else
        return;

}
List<coords> path(List<vertex>& graph,List<coords>& cl)
{
    List<A> open;//open list for A*
    List<A> closed;//closed list for A*
    //starting node is 00
    graph[0].b.cost = 0;
    graph[0].b.heuristic = calc_heuristic(graph[0].b, graph[graph.howMany() - 1].b);

    open.add_rear(A(graph[0].b, graph[0].b.cost, graph[0].b.heuristic, graph[0].b));
    while (open.howMany() > 0)
    {
        //finding the smallest sum from the open list
        int temp = smallest_entry(open);
        //cout<<"\n chosen node is==>"<<open[temp].v.r<<open[temp].v.c<<endl;
        //if chosen node is the goal node  then break
        if (open[temp].v == graph[graph.howMany() - 1].b)
        {
            closed.add_rear(A(open[temp].v, open[temp].cost, open[temp].heuristic, open[temp].path));
            open.delAt(temp);

            break;
        }

        //taking the connections of the current node from the graph
        int index = find_index(open[temp].v, graph);//formula is [ number of columns*row+column]

        for (int i = 0; i < graph[index].vert.howMany(); i++)
        {
            int cost = open[temp].cost + graph[index].vert[i].cost;
            //checking if its present in closed list

            int ind = isThere(graph[index].vert[i], closed);

            //if the cost_so_far for the current node is better found then update it in the table
            int ind1 = isThere(graph[index].vert[i], open);
            //cout<<"\n neighbours are==>\n";
            //cout<<graph[index].vert[i].r<<graph[index].vert[i].c<<endl;
            if (ind != -1 && cost < closed[ind].cost)//found in closed list now removing it from closed list and adding it to open
            {
                //cout<<"\n worked for==>"<<closed[ind].v.r<<closed[ind].v.c<<endl;
                closed[ind].cost = cost;
                //remove from the closed list and add in the open list
                //update the path
                closed[ind].path = open[temp].v;//path is parent
                open.add_rear(A(closed[ind].v, closed[ind].cost, closed[ind].heuristic, closed[ind].path));
                closed.delAt(ind);
            }


            //checking if the connection is present in the open list


            else   if (ind1 != -1 && cost < open[ind1].cost)
            {
                open[ind1].cost = cost;
                open[ind1].path = open[temp].v;
                open[ind1].sum = cost + open[ind1].heuristic;
            }
            else//if the connection is not present in either of the list then add it in the open list with the correct parameters
            {
                //first finding the heuristic value
                if (ind1 == -1 && ind == -1)
                {

                    graph[index].vert[i].heuristic = calc_heuristic(graph[index].vert[i], graph[graph.howMany() - 1].b);

                    open.add_rear(A(graph[index].vert[i], cost, graph[index].vert[i].heuristic, open[temp].v));//the last parameter could be graph[index].b
                }

            }
        }
        //we have finished looking for the connections now remove the current node from open list and add it to the closed list
        closed.add_rear(A(open[temp].v, open[temp].cost, open[temp].heuristic, open[temp].path));
        open.delAt(temp);






    }
    //now the  closed list will have the required path....
    /*
                 cout<<"\n the open list is==>";
    for(int i=0;i<open.howMany();i++)
    {
        cout<<open[i].v.r<<open[i].v.c<<" "<<open[i].cost<<" "<<open[i].heuristic<<" "<<open[i].sum<<" "<<open[i].path.r<<open[i].path.c<<endl;

    }
     cout<<"\n the closed list is==>";
    for(int i=0;i<closed.howMany();i++)
    {
        cout<<closed[i].v.r<<closed[i].v.c<<" "<<closed[i].cost<<" "<<closed[i].heuristic<<" "<<closed[i].sum<<" "<<closed[i].path.r<<closed[i].path.c<<endl;

    }
*/
//determining the net path

//cout<<"\n the path to be taken is==>";
    List<coords> track;
    for (int i = 0; i < closed.howMany(); i++)
    {
        cl.add_rear(closed[i].v);
    }
    find_path(graph[0].b, graph[graph.howMany() - 1].b, closed, track);
    /*
    for(int i=0;i<track.howMany();i++)
    {
        cout<<track[i].r<<track[i].c<<" ";
    }
    */
    return track;

}
//works for two codes only for 0 and 1...it can be changed according to the conditions
List<vertex> getMap(int code[17][26], int r, int c)//takes in pointer to the code of the maze // here its 3*3 but has to be changed
{
    int n = 0;
    List<vertex> graph;
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (code[i][j] == 1)
            {
                graph.add_rear(vertex(coords(i, j)));

                if ((i - 1) >= 0 && code[i - 1][j] == 1)//checking upper box
                {
                    graph[n].vert.add_rear(coords((i - 1), j));
                }
                if ((j + 1) <= (c - 1) && code[i][j + 1] == 1)//going to the right
                {
                    graph[n].vert.add_rear(coords(i, j + 1));
                }
                if ((j - 1) >= 0 && code[i][j - 1] == 1)//going to the left
                {
                    graph[n].vert.add_rear(coords(i, j - 1));
                }
                if ((i + 1) <= (r - 1) && code[i + 1][j] == 1)
                {
                    graph[n].vert.add_front(coords(i + 1, j));
                }

                n++;
            }
        }
    }
    /*
    cout<<"\n the map is==>\n";
    for(int i=0;i<graph.howMany();i++)
    {
        cout<<graph[i].b.r<<graph[i].b.c<<"\t";
        for(int j=0;j<graph[i].vert.howMany();j++)
        {
            cout<<graph[i].vert[j].r<<graph[i].vert[j].c<<" ";

        }
        cout<<endl;
    }
    */
    return graph;

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Direction
{
    North = 1, South = 2, East = 3, West = 4
};
class maze_1
{

protected:



    class tileMap :public sf::Transformable
    {
    public:
        sf::VertexArray tile;
        /*
        w=width of the map
        h=height of the map
        xx=x coordinate of the starting point of the tile map
        yy=y coordinate of the starting point of the tile map
        */


        void setScene(int w, int h, int xx, int yy, sf::Texture& tex, int code[100][100])
        {

            sf::Vertex* temp;
            tile.resize(w * h * 4);
            tile.setPrimitiveType(sf::Quads);
            int x = xx;//origin
            int y = yy;//origin
            int z = 0;
            for (int i = 0; i < h; i++)
            {
                x = xx;
                for (int j = 0; j < w; j++)
                {
                    temp = &tile[z];
                    temp[0].position = sf::Vector2f(x, y);

                    temp[1].position = sf::Vector2f(x + 74, y);
                    temp[2].position = sf::Vector2f(x + 74, y + 60);
                    temp[3].position = sf::Vector2f(x, y + 60);


                    temp[0].texCoords = sf::Vector2f(code[i][j] * 74, 0);
                    temp[1].texCoords = sf::Vector2f(code[i][j] * 74 + 74, 0);
                    temp[2].texCoords = sf::Vector2f(code[i][j] * 74 + 74, 60);
                    temp[3].texCoords = sf::Vector2f(code[i][j] * 74, 60);

                   // temp[3].color = sf::Color::Blue;
                    x += 74;
                    z += 4;
                }
                y += 60;

            }


        }
    };


public:
    class file
    {
    public:
        int id;
        float x;
        float y;
        void setter(int i, float xx, float yy)
        {
            id = i;
            x = xx;
            y = yy;
        }
    };
    static List<file> moving;

    bool isValid(float x, float y)
    {
        bool check = false;

        for (int i = 0; i < moving.howMany(); i++)
        {
            //cout<<moving[i].x<<" "<<moving[i].y<<endl;
            if (moving[i].x == x && moving[i].y == y)
            {
                check = true;
                break;
            }
        }
        //cout<<"\n value came here is==>"<<x<<" "<<y<<" status is==>"<<check;
        return check;
    }

    void callable(sf::RenderWindow& window);





};
List<maze_1::file> maze_1::moving;

class coord
{
public:
    float x;
    float y;
    coord()
    {

    }
    coord(float xx, float yy)
    {
        x = xx;
        y = yy;
    }

};
class user
{
public:
    List<coord> asked;
    List<coord> original;
    int next;//for original
    float cx = 74;
    float cy = 60;
    mutex m;
    maze_1 gob;
    coord vect;
    user()
    {
        //asked.add_rear(coord(15,70));
        original.add_rear(coord(15, 70));
        next = 0;


    }
    void initialze()
    {
        original.add_rear(coord(15, 70));
        next = 0;
        vect.x = 15;
        vect.y = 70;
    }
    bool Ahead(Direction d)
    {
        //initializing Ahead list with the last value in original list if the asked list is empty
        if (asked.howMany() == 0)
            asked.add_rear(coord(original.rear->s.x, original.rear->s.y));
        float rx = asked.rear->s.x;
        float ry = asked.rear->s.y;
        bool check;

        if (d == North)
        {

            check = gob.isValid(rx, ry - cy);
            if (check == true)
            {
                asked.add_rear(coord(rx, ry - cy));
                return true;
            }
            else
                return false;
        }
        else if (d == South)
        {

            check = gob.isValid(rx, ry + cy);
            if (check == true)
            {
                asked.add_rear(coord(rx, ry + cy));
                return true;
            }
            else
                return false;
        }
        else if (d == East)
        {

            check = gob.isValid(rx + cx, ry);
            if (check == true)
            {
                asked.add_rear(coord(rx + cx, ry));
                return true;

            }
            else
                return false;
        }
        else if (d == West)
        {

            check = gob.isValid(rx - cx, ry);
            if (check == true)
            {
                asked.add_rear(coord(rx - cx, ry));
                return true;
            }
            else
                return false;
        }
    }
    void Reset_Ahead()
    {
        if (asked.howMany() == 0)
            return;
        else
        {
            for (int i = 0; i < asked.howMany(); i++)
            {
                asked.delAt(i);
            }
        }
    }
    bool Move(Direction d)
    {
        //checking if the the stuff sent is correct or not;
        m.lock();
        float rx = original.rear->s.x;
        float ry = original.rear->s.y;
        m.unlock();
        bool check;
        if (d == East)
        {
            check = gob.isValid(rx + cx, ry);
            if (check == true)
            {
                m.lock();
                original.add_rear(coord(rx + cx, ry));
                Reset_Ahead();
                m.unlock();

                return true;
            }
            else
            {
                return false;
            }

        }
        else if (d == West)
        {
            check = gob.isValid(rx - cx, ry);
            if (check == true)
            {
                m.lock();
                original.add_rear(coord(rx - cx, ry));
                Reset_Ahead();
                m.unlock();

                return true;
            }
            else
            {
                return false;
            }
        }
        else if (d == North)
        {
            check = gob.isValid(rx, ry - cy);
            if (check == true)
            {
                m.lock();
                original.add_rear(coord(rx, ry - cy));
                Reset_Ahead();
                m.unlock();

                return true;
            }
            else
            {
                return false;
            }
        }
        else if (d == South)
        {
            check = gob.isValid(rx, ry + cy);
            if (check == true)
            {
                m.lock();
                original.add_rear(coord(rx, ry + cy));
                Reset_Ahead();
                m.unlock();

                return true;
            }
            else
            {
                return false;
            }
        }

    }
    bool isEnd(int mode)//tells if the player has won the match
    {
        m.lock();
        if (mode == 0)
        {
            if (original.rear->s.x == 1865 && original.rear->s.y == 1030)
            {
                m.unlock();
                return true;
            }
            else
            {
                m.unlock();
                return false;
            }
        }
        else if (mode == 1)
        {
            if (asked.howMany() != 0)
            {
                if (asked.rear->s.x == 1865 && asked.rear->s.y == 1030)
                {
                    m.unlock();
                    return true;
                }
                else
                {
                    m.unlock();
                    return false;
                }
            }
            else
            {
                m.unlock();
                return false;
            }
        }

    }

};
//1st user function
void user1(user& ob, List<coords>& way)
{
    //a parser for parsing the directions

    List<Direction> dir;
    for (int i = 0; i < way.howMany() - 1; i++)
    {
        if (way[i].c - way[i + 1].c == -1)
        {
            dir.add_rear(East);
        }
        else if (way[i].c - way[i + 1].c == 1)
        {
            dir.add_rear(West);
        }
        else if (way[i].r - way[i + 1].r == -1)
            dir.add_rear(South);
        else if (way[i].r - way[i + 1].r == 1)
            dir.add_rear(North);
    }
    for (int i = 0; i < dir.howMany(); i++)
    {
        ob.Move(dir[i]);
    }
}
void user2(user& ob)
{

    bool check;
    Direction d;
    while (1)
    {
        if (ob.Ahead(East))
        {

            d = East;
            ob.Move(d);
            // ob.Reset_Ahead();

        }
        if (ob.Ahead(South))
        {

            d = South;
            ob.Move(d);
            // ob.Reset_Ahead();

        }
        if (ob.Ahead(West))
        {

            d = West;
            ob.Move(d);
            //ob.Reset_Ahead();

        }
        if (ob.Ahead(North))
        {

            d = North;
            ob.Move(d);
            //ob.Reset_Ahead();

        }



    }
}
void updateCost(List<vertex>& fora, int r, int c)
{
    for (int i = 0; i < fora.howMany(); i++)
    {
        for (int j = 0; j < fora[i].vert.howMany(); j++)
        {
            if (fora[i].vert[j].r == r && fora[i].vert[j].c == c)
            {
                fora[i].b.cost = 10;
                fora[i].vert[j].cost = 10;
            }
        }
    }
}
void maze_1::callable(sf::RenderWindow& window)
{


    List<int> encode;
    ifstream in;
    int code[100][100];
    file l;
    in.open("position_sorted.dat", ios::binary | ios::in);//using position_sorted file for the code of the maze

    while (in)
    {
        in.read((char*)&l, sizeof(l));
        if (in.eof())
            break;
        encode.add_rear(l.id);

    }
    in.close();

    in.open("moving.dat", ios::binary | ios::in);
    while (in)
    {
        in.read((char*)&l, sizeof(l));
        if (in.eof())
            break;
        moving.add_rear(l);
    }
    in.close();

    tileMap ob;

    sf::Texture tex;
    tex.loadFromFile("pack.png");
    sf::RenderStates s;
    s.texture = &tex;

    //inputting the number of players:
    cout << "\n input the number of players==>";
    int n;
    n = 1;





    //number of rectangles will be same as of the player

    sf::RectangleShape* rect = new sf::RectangleShape[n];
    for (int i = 0; i < n; i++)
    {
        rect[i].setSize(sf::Vector2f(74, 60));
        rect[i].setPosition(15, 70);
        if (i == 0)
        {
            rect[i].setFillColor(sf::Color::Red);

        }
        else if (i == 1)
        {
            rect[i].setFillColor(sf::Color(51, 102, 255));
        }
        else if (i == 2)
        {
            rect[i].setFillColor(sf::Color(179, 0, 179));
        }
        else if (i == 3)
        {
            rect[i].setFillColor(sf::Color(255, 102, 0));
        }
        else if (i == 4)
        {
            rect[i].setFillColor(sf::Color(204, 51, 255));
        }
        else if (i == 5)
        {
            rect[i].setFillColor(sf::Color(102, 102, 153));
        }
        else if (i == 6)
        {
            rect[i].setFillColor(sf::Color(0, 204, 102));
        }
    }

    //inputting the code of the maze



    int k = 0;
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            code[i][j] = encode[k];
            k++;
        }
    }
    cout << endl;
    int code1[17][26];
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (code[i][j] != 1)
            {
                code1[i][j] = 0;
                cout << 0 << "   ";
            }
            else
            {
                code1[i][j] = 1;
                cout << code[i][j] << "   ";

            }
        }
        cout << endl;

    }
    //code1[0][2] = 0;//put obstacles here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    List<vertex> fora = getMap(code1, 17, 26);
   //update the cost of the tile here
    //updateCost(fora, 0, 20);
    //updateCost(fora, 4, 7);
    List<coords> closed;
    List<coords> way = path(fora,closed);//this has the coordinates of the path
    cout << "\n total paths explored are==>\n";
    for (int i = 0; i < closed.howMany(); i++)
    {
        cout << closed[i].r << " " << closed[i].c << endl;
    }
    way.add_front(coords(0, 0));
    cout << "\n the path is==>";
    for (int i = 0; i < way.howMany(); i++)
    {
        cout << way[i].r <<" "<< way[i].c <<endl;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ob.setScene(26, 17, 15, 70, tex, code);
    float rx = 15;
    float ry = 70;
    float cx = 74;
    float cy = 60;
    bool check;

    //making the object of the user class
    user* object = new user[n];
    thread* th = new thread[n];
    for (int i = 0; i < n; i++)
    {
        object[i].initialze();
    }

    th[0] = thread(user1, ref(object[0]), ref(way));
    // th[1]=thread(user2,ref(object[1]));


    coord temp;
    int c = 0;
    while (window.isOpen())
    {

        sf::Event event;
        float temp = 0;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                window.setSize(sf::Vector2u(1920, 1080));
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));

            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
            {
                window.setSize(sf::Vector2u(1970, 1190));
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));

            }

            if (event.type == sf::Event::Resized)
            {
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));

            }


        }
        /*
        for(int i=0;i<n;i++)
        {
         if(object[i].m.try_lock())
        {
            if(object[i].original.howMany()-1>=object[i].next)
            {
                rect[i].setPosition(object[i].original[object[i].next].x,object[i].original[object[i].next].y);
            }
            object[i].next++;
            object[i].m.unlock();
        }
        }
        */
        for (int i = 0; i < n; i++)
        {

            Direction d;
            if (object[i].m.try_lock())
            {

                if (object[i].original.howMany() - 1 >= (object[i].next + 1) && object[i].vect.x == object[i].original[object[i].next].x && object[i].vect.y == object[i].original[object[i].next].y)
                {

                    float x1 = object[i].original[object[i].next + 1].x;
                    float y1 = object[i].original[object[i].next + 1].y;
                    float x2 = object[i].original[object[i].next].x;
                    float y2 = object[i].original[object[i].next].y;
                    if (x1 - x2 == 0 && y1 - y2 < 0)//North
                    {
                        d = North;
                    }
                    else if (x1 - x2 == 0 && y1 - y2 > 0)//South
                    {
                        d = South;
                    }
                    else if (x1 - x2 > 0 && y1 - y2 == 0)
                    {

                        d = West;
                    }
                    else if (x1 - x2 < 0 && y1 == y2)
                    {
                        d = East;
                    }

                    object[i].next++;
                }
                else
                {

                    if (d == North && object[i].original[object[i].next].y != object[i].vect.y)
                    {
                        object[i].vect.y--;
                        rect[i].setPosition(object[i].vect.x, object[i].vect.y);
                    }
                    else  if (d == South && object[i].original[object[i].next].y != object[i].vect.y)
                    {
                        object[i].vect.y++;
                        rect[i].setPosition(object[i].vect.x, object[i].vect.y);
                    }
                    else if (d == East && object[i].original[object[i].next].x != object[i].vect.x)
                    {
                        object[i].vect.x--;
                        rect[i].setPosition(object[i].vect.x, object[i].vect.y);
                    }
                    else if (d == West && object[i].original[object[i].next].x != object[i].vect.x)
                    {
                        object[i].vect.x++;
                        rect[i].setPosition(object[i].vect.x, object[i].vect.y);

                    }


                }
            }
            object[i].m.unlock();
        }




        if(closed.howMany()>0)
        {
            int tag = (closed[0].r * 26 + closed[0].c)*4;
            sf::Vertex *temp = &ob.tile[tag];
            temp[0].color = sf::Color::Blue;
            temp[1].color = sf::Color::Red;
            temp[2].color = sf::Color::Cyan;
            temp[3].color = sf::Color::Yellow;
            closed.del_front();
        }
        
        else if (c < way.howMany())
        {
            int tag = (way[c].r * 26 + way[c].c) * 4;
            sf::Vertex* temp = &ob.tile[tag];
            temp[0].color = sf::Color::White;
            temp[1].color = sf::Color::White;
            temp[2].color = sf::Color::Cyan;
            temp[3].color = sf::Color::Cyan;
            c++;
        }
        
        window.clear();

        window.draw(ob.tile, s);
        for (int i = 0; i < n; i++)
        {
            window.draw(rect[i]);
        }
        window.display();
    }
    //printing the moving..


}




int main()

{


    sf::RenderWindow window(sf::VideoMode(1970, 1190), "window", sf::Style::Default);//1970 1190
    window.setFramerateLimit(60);
    maze_1 ob1;
    
    ob1.callable(window);
    
     //cout<<ob2.Move(East)<<ob2.Ahead(South)<<ob2.Move(South)<<ob2.Move(South);

}

