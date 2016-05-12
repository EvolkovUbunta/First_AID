#include "func.h"
#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;

int inp(int &a)	//кошерный ввод числа с проверкой (будет просить ввести, пока не напишешь по нормальному), аргумент-число, которое нужно ввести
//возвращает введеное число
{
  cin>>a;
  while (cin.fail())
  {
    cout<<"Некорректный ввод, попробуйте еще раз"<<endl;
    cin.clear();
    cin.ignore();
    cin>>a;
  }
  return a;
}

float inp(float &a)
{
  cin>>a;
  while (cin.fail())
  {
    cout<<"Некорректный ввод, попробуйте еще раз"<<endl;
    cin.clear();
    cin.ignore();
    cin>>a;
  }
  return a;
}

float coord::def_dis(const coord &a) //определение расстояния для стационарной точки
{
  float k=a.x;
  float m=a.y;
  float p=pow(pow(k-x,2)+pow(m-y,2),0.5);
  return p;
}

call::call()
{
  float a,b;
  cout<<"Введите местоположение пациента"<<endl;
  cout<<"X: ";
  inp(a);
  cout<<"Y: ";
  inp(b);
  cloc.set_coord(a,b);
  cout<<"Введите тип пациента (1-обычный человек, 2-ребенок, 3-пенсионер):"<<endl;
  while(1)
  {
    int l=inp(ctype);
    if (l==1 || l==2 || l==3) break;
    else cout<<"Введите целое число от 1 до 3"<<endl;
  }
  cout<<"Введите тип необходимой помощи (1-оказать первую помощь, 2-довести до больницы):"<<endl;
  cin>>cserv;
  next=NULL;
}
                                                                      

car::car()
{
  cout<<"Введите тип машины (1-для обычного человека, 2-для ребенка, 3-для пенсионера):"<<endl;
  while(1)
  {
    int l=inp(type);
    if (l==1 || l==2 || l==3) break;
    else cout<<"Введите целое число от 1 до 3"<<endl;
  } 
  switch(type)
  {	
    case 1:
      sp=sp1;
      break;
    case 2:
      sp=sp2;
      break;
    case 3:
      sp=sp3;
      break;
  }
  cout<<"Введите номер машины:"<<endl;
  getline(cin,num);
  float a,b;
  cout<<"Введите координаты машины по умолчанию:"<<endl;
  cout<<"X: ";
  inp(a);
  cout<<"Y: ";
  inp(b);
  loc_prim.set_coord(a,b);
  loc1.set_coord(a,b);
  loc2.set_coord(a,b);
  loc3.set_coord(a,b);
  time1=time(NULL);
  time2=time1;
  time=time1;
  next=NULL;
}     

coord car::get_loc_cur()
{
  coord back;
  float dis,dis1;
  time_t t=time(NULL);
  if (t>=time1 && t<time2)
  {
    dis=pow(pow(loc2.get_x()-loc1.get_x(),2)+pow(loc2.get_y()-loc1.get_y(),2),0.5);
    dis1=(t-time1)*sp/3600;
    if (dis1>=dis) back.set_coord(loc2);
    else
    {
      float k=(dis1/dis)*loc2.get_x()+loc1.get_x()*(1-dis1/dis);
      float m=(dis1/dis)*loc2.get_y()+loc1.get_y()*(1-dis1/dis);
      back.set_coord(k,m);
    }
  }
  else
  {
    if (t>=time2 && t<time)
    {
      dis=pow(pow(loc3.get_x()-loc2.get_x(),2)+pow(loc3.get_y()-loc2.get_y(),2),0.5);
      dis1=(t-time2)*sp/3600;
      float k=(dis1/dis)*loc3.get_x()+loc2.get_x()*(1-dis1/dis);
      float m=(dis1/dis)*loc3.get_y()+loc2.get_y()*(1-dis1/dis);
      back.set_coord(k,m);
    }
    else
    {
      dis=pow(pow(loc_prim.get_x()-loc3.get_x(),2)+pow(loc_prim.get_y()-loc3.get_y(),2),0.5);
      dis1=(t-time)*sp/3600;
      if (dis1>=dis) back.set_coord(loc_prim);
      else
      {
        float k=(dis1/dis)*loc_prim.get_x()+loc3.get_x()*(1-dis1/dis);
        float m=(dis1/dis)*loc_prim.get_y()+loc3.get_y()*(1-dis1/dis);
        back.set_coord(k,m);
      }
    }
  }
  return back;
}

/*функция move должна устанавливать 3 значения, если это первая помощь, то 1 значение: время и точка начала движения, 2 значение:
время и точка середины пути, 3 значение: точка конца пути и время занятости машины. Если нужно довести до больницы:
1 значение: точка и время начала пути, 2 значение: точка нахождения пациента и время, когда машина от него стартует,
3 значение: точка конца пути и время занятости машины. При этом местонахождение машины по умолчанию не меняется.*/

void car::move(const call &a, const control &ctrl)
{
  float dis,dis1;
  if (a.get_cserv()==1) // если оказать первую помощь
  {
    coord tab=get_loc_cur();
    coord bet=a.get_cloc();
    float dis=tab.def_dis(bet);
    float k=(tab.get_x()+bet.get_x())/2;
    float m=(tab.get_y()+bet.get_y())/2;
    loc1.set_coord(tab);
    loc2.set_coord(k,m);
    loc3.set_coord(bet);
    time1=time(NULL);
    time2=time1+dis/sp/2*3600;
    time=time1+dis/sp*3600+1200;	
  }
  if (a.get_cserv()==2) // если довести до больницы
  {
    coord tab=get_loc_cur();
    coord bet=a.get_cloc();
    float dis=tab.def_dis(bet);
    loc1.set_coord(tab);
    loc2.set_coord(bet);
    time1=time(NULL);
    time2=time1+dis/sp*3600+300;
    dis1=bet.def_dis(*((&ctrl)->bbeg));
    loc3.set_coord(*((&ctrl)->bbeg));
    coord *rex=(&ctrl)->bbeg->next;
    while(rex!=NULL)
    {
      if (dis1>bet.def_dis(*rex));
      {
        dis1=bet.def_dis(*rex);
        loc3.set_coord(*rex);
      }
      rex=rex->next;
    }
    time=time2+dis1/sp*3600;
  }
  return;
}

car &choose(call &zvon, control &ctrl)
{
  car *best; //лучшая машина (мы будем его возвращать
  car *rex=(&ctrl)->cbeg; // указатель на текущую машину( мы будем его переключать, идя по списку)
  time_t cur_time=time(NULL); // текущее время, определил его сразу, чтобы не была синхронизация времени 
  time_t min; // наименьшее время (оно потребуется лучшей машине, чтоб доехать до больного)
  time_t mur; // промежуточное значение, чтоб сравнивать с min
  coord pl; // промежуточный указатель (в него передается возвращаемое значение get_loc_cur() )
  // сейчас мы должны установить начальное значение min и best
  if (rex->get_time()-cur_time<=0) // если машина свободна
  {
    pl=rex->get_loc_cur(); // получаем текущее местоположение
    min=pl.def_dis(zvon.get_cloc())/rex->get_sp(); //устанавливаем начальное значение min (делим растояние от текущего местоположения до больного на скорость) 
  }
  else min=rex->get_time()-cur_time+rex->loc3.def_dis(zvon.get_cloc())/rex->get_sp(); /* если машина занята, получаем min (берем время, которое
  осталось на выполнение заказа и прибавляем к нему расстояние от точки завершения заказа до больного, поделенное на скорость) */
  best=rex; //устанавливаем начальное значение лучшей машины
  while (rex!=NULL) // пока список не кончится
  {
    if (rex->get_type()!=zvon.get_ctype()) rex=rex->next; // если тип машины не совпадает с типом звонка(например звонит пенсионер, а помочь надо ребенку)
    else //если совпадает
    {
      if (rex->get_time()-cur_time<=0) // если машина свободна  
      {
        pl=rex->get_loc_cur();
        mur=pl.def_dis(zvon.get_cloc())/rex->get_sp(); // получаем промежуточное значение времени для сравнения с min
      }
      else mur=rex->get_time()-cur_time+rex->loc3.def_dis(zvon.get_cloc())/rex->get_sp(); // если машина занята, тоже получаем значение для сравнения
      if (min>mur) // если полученное значение меньше минимального
      {
        min=mur;
        best=rex;
      }
      rex=rex->next;
    }
  }
  return *best;
}

int input_car(control &ctrl)
{
  while(1)
  {
    car *rex;
    if ((&ctrl)->cbeg==NULL && (&ctrl)->cend==NULL) (&ctrl)->cbeg=rex;
    else (&ctrl)->cend->next=rex;
    (&ctrl)->cend=rex;
    (&ctrl)->cend->next=NULL;
    cout<<"Хотите добавить еще один элемент? (1-да/2-нет)"<<endl;
    int a;
    inp(a);
    if (a==2) break;
  }
  return 0;
}

int main()
{
  call ob1;
  car ob2;
  return 0;
}