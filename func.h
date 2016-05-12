#include <string>
#include <ctime>
#include <iostream>
#define sp1 80
#define sp2 70
#define sp3 50
using namespace std;

class control;

class coord
{
    float x,y;
  public:
    coord() { x=0; y=0; }
    coord(float a, float b) { x=a; y=b; }
    void set_coord(const coord &a) { x=a.x; y=a.y; }
    void set_coord(float a, float b) { x=a; y=b; }
    float get_x() { return x; }
    float get_y() { return y; }
    float def_dis(const coord &a);
    int operator== (const coord &ob) { return ((x==ob.x) && (y==ob.y)); }
    coord *next;
};

class call //класс - поступающий звонок
{
    coord cloc; //местоположение больного
    int ctype; //кому нужна помощь (1-взрослому человеку, 2-ребенку, 3-пожилому человеку)
    int cserv; //какая необходима помощь (1-оказать первую помощь, 2-довести до больницы)
  public:
    call();
    coord get_cloc() const { return cloc; }
    int get_ctype() const { return ctype; }
    int get_cserv() const { return cserv; }
    call *next;
};

class car // класс-машина
{
    time_t time1; // время начала движения
    time_t time2; //время промежуточной точки
    time_t time; // время, на которое машина занята
    int type;   // тип машины (1-для взрослого человека, 2-для ребенка, 3-для пожилых людей)
    string num;    // номер машины (ее ведь надо как-то идентифицировать)
    float sp; // скорость машины
  public:
    coord loc1; // точка начала последнего движения
    coord loc2; // промежуточная точка (если первая помощь-середина пути до больного, если довести до больницы-местоположение больного)
    coord loc3; // конечная точка (если первая помощь-местоположение больного, если довести до больницы-местоположение больницы)
    coord loc_prim; // положение машины по умолчанию
    coord get_loc_cur(); //возвращает текущие координаты машины, использовать только если машина свободна!
    void move(const call &a, const control &ctrl); /*эта функция двигает машину в указанную точку,  добавляет время на дорогу и оказываемую услугу (1-оказать первую помощь, 2-довести до больницы). 
    Если нужно оказать первую помощь, то добавляется время на дорогу и на оказание первой помощи (в среднем 20 минут), loc_st меняются на координаты последнего больного. 
    Если больного нужно довести до больницы, то время добавляется на дорогу до больного и дорогу от больного до больницы, соответственно, loc_st машины меняются на местоположение больницы. */
    time_t get_time() { return time; }
    int get_type() { return type; }
    string get_num() { return num; }
    float get_sp() { return sp; }
    car();
    car *next;
};

class control
{
  public:
    coord *bbeg;
    coord *bend;
    car *cbeg;
    car *cend;
    control();
};